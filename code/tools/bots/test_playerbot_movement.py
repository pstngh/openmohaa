#!/usr/bin/env python3
"""Focused source contracts for bot movement and navigation recovery."""

from __future__ import annotations

import re
import unittest
from pathlib import Path


ROOT = Path(__file__).resolve().parents[3]
SOURCE_PATH = ROOT / "code" / "fgame" / "playerbot_movement.cpp"
HEADER_PATH = ROOT / "code" / "fgame" / "playerbot.h"
ROUTE_PATH = ROOT / "code" / "fgame" / "playerbot_route.cpp"
OBSTACLE_PATH = ROOT / "code" / "fgame" / "navigation_recast_obstacle.cpp"


def constant(source: str, name: str) -> float:
    match = re.search(rf"{name}\s*=\s*([0-9.]+)f", source)
    if not match:
        raise AssertionError(f"missing {name}")
    return float(match.group(1))


class RoamingStyleContract(unittest.TestCase):
    @classmethod
    def setUpClass(cls) -> None:
        cls.source = SOURCE_PATH.read_text(encoding="utf-8")
        cls.header = HEADER_PATH.read_text(encoding="utf-8")
        start = cls.source.index(
            "float BotMovement::CalculateMoveProbeFraction"
        )
        end = cls.source.index("// Pick a random dwell time", start)
        cls.probes = cls.source[start:end]
        start = cls.source.index(
            "void BotMovement::UpdateAggressiveMovement"
        )
        end = cls.source.index("void BotMovement::DirectMoveThink", start)
        cls.update = cls.source[start:end]
        start = cls.source.index(
            "void BotMovement::ResolveImminentCollision"
        )
        cls.guard = cls.source[start:]

    def test_noncombat_roaming_alternates_active_and_neutral(self) -> None:
        self.assertIn("bool m_bRoamStrafeActive", self.header)
        self.assertIn(
            "m_bRoamStrafeActive = !m_bRoamStrafeActive",
            self.update,
        )
        self.assertIn(
            "m_bRoamStrafeActive && baseTravelCommand",
            self.update,
        )

    def test_combat_strafe_remains_continuous(self) -> None:
        self.assertIn(
            "const bool strafePhaseActive = !nonCombatTravel",
            self.update,
        )
        self.assertIn(
            "} else {\n            m_iStrafeDirection = "
            "-m_iStrafeDirection;",
            self.update.replace("\r\n", "\n"),
        )

    def test_roaming_veto_compares_optional_and_base_clearance(self) -> None:
        distance = constant(
            self.source, "BOT_ROAM_STRAFE_VETO_DISTANCE"
        )
        epsilon = constant(
            self.source, "BOT_ROAM_STRAFE_CLEARANCE_EPSILON"
        )
        self.assertGreaterEqual(distance, 96.0)
        self.assertLessEqual(epsilon, 0.10)
        self.assertIn(
            "CalculateMoveProbeFraction(botcmd, probeDistance)",
            self.update,
        )
        self.assertIn(
            "probeFraction\n                + "
            "BOT_ROAM_STRAFE_CLEARANCE_EPSILON\n"
            "                < baseProbeFraction",
            self.update.replace("\r\n", "\n"),
        )

    def test_veto_cancels_without_reversing_or_centering(self) -> None:
        start = self.update.index("bool vetoRoamStrafe")
        end = self.update.index("if (!vetoRoamStrafe)", start)
        decision = self.update[start:end]
        self.assertNotIn("m_iStrafeDirection =", decision)
        self.assertNotIn("botcmd.rightmove =", decision)
        self.assertNotIn("const int probeRight", self.probes)
        self.assertNotIn("const int newRight", self.update)
        self.assertNotIn("LaneSteering", self.source)
        self.assertNotIn("CENTER_ENGAGE", self.source)

    def test_active_roaming_still_strafes_and_leans(self) -> None:
        self.assertIn("m_telemetry.strafeApplied = true", self.update)
        self.assertIn("m_bIsLeaning = true", self.update)
        self.assertIn("BUTTON_LEAN_LEFT", self.update)
        self.assertIn("BUTTON_LEAN_RIGHT", self.update)

    def test_existing_final_guard_still_owns_imminent_collisions(self) -> None:
        self.assertIn("TraceImminentMove(botcmd, trace)", self.guard)
        self.assertIn(
            "botcmd.forwardmove = baseCommand.forwardmove", self.guard
        )
        self.assertIn(
            "m_iMovementOverlaySuppressUntil", self.guard
        )


class NavigationFailureContract(unittest.TestCase):
    @classmethod
    def setUpClass(cls) -> None:
        cls.source = SOURCE_PATH.read_text(encoding="utf-8")
        cls.header = HEADER_PATH.read_text(encoding="utf-8")
        cls.route = ROUTE_PATH.read_text(encoding="utf-8")
        cls.obstacles = OBSTACLE_PATH.read_text(encoding="utf-8")

        start = cls.source.index(
            "void BotMovement::UpdateLocalLoopDetection"
        )
        end = cls.source.index(
            "bool BotMovement::ConsumeLocalOscillation", start
        )
        cls.loop = cls.source[start:end]

        start = cls.source.index(
            "Vector BotMovement::ChooseBlockedRecoveryGoal"
        )
        end = cls.source.index(
            "void BotMovement::PushThroughOpenableDoor", start
        )
        cls.recovery = cls.source[start:end]

        start = cls.source.index(
            "void BotMovement::PushThroughOpenableDoor"
        )
        end = cls.source.index(
            "void BotMovement::CalculateBestFrontAvoidance", start
        )
        cls.door_push = cls.source[start:end]

        start = cls.source.index(
            "void BotMovement::ResolveImminentCollision"
        )
        cls.guard = cls.source[start:]

    def test_fully_open_door_panels_are_recast_obstacles(self) -> None:
        self.assertIn('#include "doors.h"', self.obstacles)
        self.assertIn("return door->isOpen();", self.obstacles)
        self.assertEqual(
            self.obstacles.count(
                "!isDoor && radiusSqr < Square(100)"
            ),
            2,
        )
        self.assertEqual(
            self.obstacles.count("Square(128)"),
            2,
        )

    def test_openable_door_contact_preserves_player_movement(self) -> None:
        self.assertIn(
            '#include "movement_telemetry.h"', self.source
        )
        self.assertNotIn("m_pPath->FindPath", self.door_push)
        self.assertIn("bot_door_pushthrough", self.door_push)
        self.assertIn("bot_door_push_slide", self.door_push)
        self.assertIn(
            "BOT_DOOR_PUSH_STALL_MSEC            = 350", self.source
        )
        self.assertIn(
            "BOT_DOOR_PUSH_RECONTACT_MSEC        = 1500", self.source
        )
        self.assertIn(
            "BOT_DOOR_EXIT_COMMIT_MSEC           = 750", self.source
        )
        self.assertIn(
            "ApplyDoorPushThrough(botcmd);", self.door_push
        )
        self.assertIn("ContinueDoorExit(baseCommand);", self.source)
        self.assertIn("ContinueDoorExit(botcmd);", self.source)
        self.assertIn(
            "m_iTempAwayState == 2", self.door_push
        )
        self.assertIn(
            "BOT_DOOR_PUSH_SIDE_COMMAND / commandMax", self.door_push
        )
        self.assertIn("bot_door_push_blocked", self.guard)
        self.assertIn(
            "m_vDoorPushDirection = vec_zero;", self.guard
        )
        self.assertIn(
            "m_vDoorPushApproachDirection = vec_zero;", self.guard
        )
        self.assertIn("door->absmin + door->absmax", self.door_push)
        self.assertIn("CalculateMoveProbeFraction", self.door_push)
        self.assertIn("m_vDoorPushDirection", self.header)
        self.assertIn("m_vDoorPushApproachDirection", self.header)
        self.assertIn(
            "PushThroughOpenableDoor(botcmd, openableDoor, trace)", self.guard
        )
        door_start = self.guard.index("Door *openableDoor")
        door_contact = self.guard[
            door_start:
            self.guard.index("const bool hitSentient", door_start)
        ]
        self.assertIn("if (openableDoor)", door_contact)
        self.assertIn("return;", door_contact)
        self.assertNotIn("botcmd.forwardmove", door_contact)
        self.assertNotIn("botcmd.rightmove", door_contact)
        self.assertNotIn("FindPath", door_contact)

    def test_blocked_recovery_prefers_committed_lateral_clearance(self) -> None:
        self.assertIn(
            "CollisionAvoidanceTargetClear(candidates[j])",
            self.recovery,
        )
        self.assertIn(
            "bot_blocked_lateral_recovery", self.recovery
        )
        self.assertIn(
            "- forward * BOT_BLOCKED_RECOVERY_BACK_UNITS",
            self.recovery,
        )
        self.assertNotIn("G_CRandom", self.recovery)
        self.assertNotIn(
            "controlledEntity->origin + delta + dir * 128",
            self.source,
        )

    def test_loop_detector_requires_return_travel_and_stable_target(self) -> None:
        self.assertIn(
            "BOT_LOCAL_LOOP_MIN_AGE_MSEC        = 3000",
            self.source,
        )
        self.assertIn(
            "BOT_LOCAL_LOOP_MIN_TRAVEL_UNITS    = 320.0f",
            self.source,
        )
        self.assertIn(
            "origin - m_vLocalLoopOrigins[i]", self.loop
        )
        self.assertIn(
            "m_vTargetPos - m_vLocalLoopTargets[i]", self.loop
        )
        self.assertIn(
            "m_fLocalLoopTravelTotal - m_fLocalLoopTravel[i]",
            self.loop,
        )
        self.assertIn("m_bLocalOscillation       = true", self.loop)

    def test_demo_route_abandons_and_reseeds_detected_loop(self) -> None:
        start = self.route.index(
            "if (movement.ConsumeLocalOscillation())"
        )
        end = self.route.index(
            "// Replacing a path from a mid-ladder origin", start
        )
        response = self.route[start:end]
        self.assertIn("bot_objective_route_oscillation", response)
        self.assertIn("bot_ffa_route_oscillation", response)
        self.assertIn("movement.ClearMove();", response)
        self.assertIn("ResetDemoRoute(route, true);", response)


if __name__ == "__main__":
    unittest.main()
