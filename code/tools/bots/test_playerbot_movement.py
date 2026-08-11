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
RECAST_PATH = ROOT / "code" / "fgame" / "navigation_recast_path.cpp"
RECAST_CONFIG_PATH = ROOT / "code" / "fgame" / "navigation_recast_config.h"
RECAST_HEADER_PATH = ROOT / "code" / "fgame" / "navigation_recast_path.h"
PATH_INTERFACE_PATH = ROOT / "code" / "fgame" / "navigation_path.h"
CONTROLLER_PATH = ROOT / "code" / "fgame" / "playerbot.cpp"


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
        self.assertIn("m_iLeanDirection", self.header)
        self.assertIn("BOT_LEAN_RELEASE_GRACE_MSEC", self.update)
        self.assertIn("BOT_LEAN_SWITCH_NEUTRAL_MSEC", self.update)
        self.assertIn("leanReleaseGrace", self.update)
        self.assertIn(
            "!suppressMovement && m_iLeanDirection", self.update
        )

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
        cls.recast = RECAST_PATH.read_text(encoding="utf-8")
        cls.recast_config = RECAST_CONFIG_PATH.read_text(encoding="utf-8")
        cls.path_interface = PATH_INTERFACE_PATH.read_text(encoding="utf-8")
        start = cls.recast.index(
            "bool RecastPather::BuildComfortInsetCorner"
        )
        end = cls.recast.index("void RecastPather::UpdatePos", start)
        cls.comfort_inset = cls.recast[start:end]
        start = cls.source.index(
            "bool BotMovement::AllowRouteComfortInset"
        )
        end = cls.source.index("void BotMovement::NewMove", start)
        cls.comfort_gate = cls.source[start:end]



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

        start = cls.source.index(
            "void BotMovement::ApplyRouteDirectionContinuity"
        )
        end = cls.source.index(
            "Vector BotMovement::CalculateRelativeWishDirection", start
        )
        cls.route_continuity = cls.source[start:end]


    def test_recast_comfort_insets_the_first_corner_inside_its_corridor(
        self,
    ) -> None:
        enter = constant(
            self.recast, "RECAST_COMFORT_ENTER_CLEARANCE"
        )
        release = constant(
            self.recast, "RECAST_COMFORT_EXIT_CLEARANCE"
        )
        target = constant(
            self.recast, "RECAST_COMFORT_TARGET_CLEARANCE"
        )
        lookahead = constant(
            self.recast, "RECAST_COMFORT_LOOKAHEAD"
        )
        self.assertLess(enter, release)
        self.assertLess(release, target)
        self.assertGreaterEqual(enter, 12.0)
        self.assertGreaterEqual(target, 24.0)
        self.assertLessEqual(lookahead, 96.0)
        self.assertEqual(
            self.comfort_inset.count("findDistanceToWall("), 2
        )
        self.assertIn("moveAlongSurface(", self.comfort_inset)
        self.assertIn("visited[i] == path[j]", self.comfort_inset)
        self.assertIn(
            "candidateClearance < "
            "RECAST_COMFORT_EXIT_CLEARANCE - 0.1f",
            self.comfort_inset,
        )
        self.assertIn("dtVcopy(corner, result)", self.comfort_inset)

        update_start = self.recast.index(
            "void RecastPather::UpdatePos"
        )
        update_end = self.recast.index(
            "void RecastPather::Clear", update_start
        )
        update = self.recast[update_start:update_end]
        self.assertIn(
            "VectorCopy(detourData->corners[0], steeringCorner)",
            update,
        )
        self.assertIn(
            "BuildComfortInsetCorner(steeringCorner)", update
        )
        self.assertIn(
            "ConvertRecastToGameCoord(steeringCorner, currentNodePos)",
            update,
        )

    def test_recast_comfort_preserves_special_and_tight_routes(
        self,
    ) -> None:
        self.assertRegex(
            self.recast_config, r"agentRadius\s*=\s*MAXS_X"
        )
        self.assertIn(
            "DT_STRAIGHTPATH_OFFMESH_CONNECTION",
            self.comfort_inset,
        )
        self.assertIn(
            "DT_STRAIGHTPATH_END", self.comfort_inset
        )
        self.assertIn(
            "routeDistance < RECAST_COMFORT_MIN_FORWARD",
            self.comfort_inset,
        )
        self.assertIn(
            "const dtPolyRef *path = corridor.getPath()",
            self.comfort_inset,
        )
        self.assertIn(
            "comfortInsetActive = false", self.comfort_inset
        )
        self.assertIn(
            "getTileAndPolyByRef(", self.comfort_inset
        )
        self.assertNotRegex(
            self.recast_config,
            r"agentRadius\s*=\s*MAXS_X\s*\+",
        )

        self.assertIn(
            "!comfortInsetEnabled", self.comfort_inset
        )
        self.assertIn(
            "SetRouteComfortInsetEnabled(bool) {}",
            self.path_interface,
        )
        self.assertEqual(
            self.source.count(
                "SetRouteComfortInsetEnabled(AllowRouteComfortInset())"
            ),
            2,
        )
        for bypass in (
            "m_bDirectMove",
            "m_bHasCombatTarget",
            "controlledEntity->GetLadder()",
            "m_bJump",
            "m_iTempAwayState != 2",
            "m_bAvoidCollision",
            "doorCommitActive",
        ):
            self.assertIn(bypass, self.comfort_gate)


    def test_fast_route_turns_have_narrowly_bypassed_continuity(self) -> None:
        self.assertIn(
            "BOT_ROUTE_TURN_RATE_DEGREES         = 720.0f",
            self.source,
        )
        self.assertIn(
            "BOT_ROUTE_TURN_LIMIT_SPEED          = 80.0f",
            self.source,
        )
        self.assertIn("AngleNormalize180", self.route_continuity)
        self.assertIn("bot_route_turn_limited", self.route_continuity)
        self.assertIn(
            "BOT_ROUTE_TURN_CLEARANCE_EPSILON",
            self.route_continuity,
        )
        for bypass in (
            "m_bDirectMove",
            "m_bHasCombatTarget",
            "controlledEntity->GetLadder()",
            "m_bJump",
            "m_iTempAwayState == 2",
            "m_bAvoidCollision",
            "m_vDoorPushApproachDirection",
        ):
            self.assertIn(bypass, self.route_continuity)
        self.assertEqual(
            self.source.count(
                "ApplyRouteDirectionContinuity(m_vCurrentDir);"
            ),
            1,
        )

    def test_open_panel_contact_commits_to_a_clear_panel_end(self) -> None:
        self.assertIn(
            "void   RecordOpenDoorPanelContact", self.header
        )
        self.assertIn("bool   EscapeOpenDoorPanel", self.header)
        self.assertIn("Door *tracedDoor = BotTraceDoor(trace)", self.guard)
        self.assertIn("bot_open_door_panel_contact", self.source)
        self.assertIn(
            "EscapeOpenDoorPanel(botcmd, tracedDoor, trace)", self.guard
        )
        start = self.source.index(
            "bool BotMovement::EscapeOpenDoorPanel"
        )
        end = self.source.index(
            "void BotMovement::CalculateBestFrontAvoidance", start
        )
        escape = self.source[start:end]
        self.assertIn("CalculateMoveProbeFraction", escape)
        self.assertIn("door->absmin + door->absmax", escape)
        self.assertIn("bot_open_door_panel_escape", escape)
        self.assertIn("SetCommandMoveVector", escape)
        self.assertIn("BOT_DOOR_PUSH_SIDE_COMMAND", escape)
        self.assertNotIn("FindPath", escape)

    def test_open_panel_exit_uses_bounded_geometric_completion(self) -> None:
        self.assertIn(
            "BOT_DOOR_OPEN_PANEL_EXIT_MAX_MSEC   = 3000",
            self.source,
        )
        self.assertIn(
            "BOT_DOOR_OPEN_PANEL_EXIT_DISTANCE   = 128.0f",
            self.source,
        )
        self.assertIn("bool   m_bOpenDoorPanelExit", self.header)
        self.assertIn("Vector m_vOpenDoorPanelExitOrigin", self.header)
        start = self.source.index(
            "void BotMovement::UpdateOpenDoorPanelExit"
        )
        end = self.source.index(
            "void BotMovement::ContinueDoorExit", start
        )
        completion = self.source[start:end]
        self.assertIn(
            "DotProduct(displacement, m_vDoorPushApproachDirection)",
            completion,
        )
        self.assertIn("bot_open_door_panel_exit_complete", completion)
        self.assertIn("bot_open_door_panel_exit_timeout", completion)
        self.assertNotIn("FindPath", completion)
        self.assertIn(
            "const bool continuingContact = m_bOpenDoorPanelExit",
            self.door_push,
        )
        self.assertIn(
            "const bool continuingContact = !m_bOpenDoorPanelExit",
            self.door_push,
        )
        self.assertLess(
            self.source.index("UpdateOpenDoorPanelExit();"),
            self.source.index("ContinueDoorExit(baseCommand);"),
        )

    def test_fully_open_door_panels_are_recast_obstacles(self) -> None:
        self.assertIn('#include "doors.h"', self.obstacles)
        self.assertIn(
            "if (door->isOpen())", self.source
        )
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
        self.assertIn("bot_door_push_open_edge", self.door_push)
        self.assertIn(
            "BotDoorOpeningEdgeDirection(door, openingEdge)",
            self.door_push,
        )
        self.assertIn("doorCenter - door->origin", self.source)
        self.assertIn("openingEdgeBlocked", self.door_push)
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
        self.assertIn(
            "BOT_COLLISION_PATH_PROBE_DISTANCE", self.source
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
        self.assertIn("route.retryTime = level.inttime;", response)


class RouteViewLeadContract(unittest.TestCase):
    @classmethod
    def setUpClass(cls) -> None:
        cls.controller = CONTROLLER_PATH.read_text(encoding="utf-8")
        cls.movement = SOURCE_PATH.read_text(encoding="utf-8")
        cls.header = HEADER_PATH.read_text(encoding="utf-8")
        cls.recast = RECAST_PATH.read_text(encoding="utf-8")
        cls.recast_header = RECAST_HEADER_PATH.read_text(encoding="utf-8")
        cls.path_interface = PATH_INTERFACE_PATH.read_text(encoding="utf-8")
        start = cls.controller.index("void BotController::AimAtAimNode")
        end = cls.controller.index("void BotController::CheckReload", start)
        cls.aim = cls.controller[start:end]
        start = cls.movement.index(
            "bool BotMovement::GetRouteLookAheadTarget"
        )
        end = cls.movement.index("void BotMovement::ResetTelemetry", start)
        cls.preview = cls.movement[start:end]

    def test_path_preview_is_bounded_and_observational(self) -> None:
        lookahead = constant(
            self.controller, "BOT_ROUTE_VIEW_LOOKAHEAD"
        )
        self.assertGreaterEqual(lookahead, 64.0)
        self.assertLessEqual(lookahead, 128.0)
        path_preview = self.path_interface[
            self.path_interface.index("GetLookAheadPoint"):
            self.path_interface.index("GetDestination")
        ]
        self.assertIn("GetLookAheadPoint", path_preview)
        self.assertIn("GetCurrentDelta()", path_preview)
        self.assertIn("GetNode(nextNodeIndex).origin", path_preview)
        self.assertIn(
            "GetLookAheadPoint(const Vector& origin, float distance)",
            self.recast_header,
        )
        self.assertIn(
            "Vector RecastPather::GetLookAheadPoint", self.recast
        )
        self.assertIn("detourData->corners[nextCornerIndex]", self.recast)
        self.assertIn("currentNodePos", self.recast)
        self.assertNotIn("GetNode(nextCornerIndex)", self.recast)

        self.assertNotIn("FindPath", path_preview)
        self.assertNotIn("UpdatePos", path_preview)

    def test_preview_keeps_explicit_movement_owners(self) -> None:
        self.assertIn(
            "GetRouteLookAheadTarget(float distance, Vector& target) const",
            self.header,
        )
        self.assertIn("!AllowRouteComfortInset()", self.preview)
        self.assertIn("m_iTempAwayState != 0", self.preview)
        self.assertIn("m_iLadderExitUntil", self.preview)
        self.assertIn("m_pPath->GetLookAheadPoint", self.preview)
        self.assertNotIn("usercmd_t", self.preview)
        self.assertNotIn("m_vCurrentDir =", self.preview)
        self.assertNotIn("FindPath", self.preview)

    def test_noncombat_view_leads_without_replacing_fallback_aim(self) -> None:
        self.assertIn("BOT_ROUTE_VIEW_LOOKAHEAD", self.aim)
        self.assertIn("rotation.AimAt(routeLookTarget)", self.aim)
        self.assertIn(
            "movement.GetCurrentMoveDirection().toAngles()", self.aim
        )
        self.assertIn("targetAngles.x      = 0", self.aim)
        self.assertIn("target.z += controlledEntity->viewheight", self.preview)
        self.assertNotIn("G_Random", self.aim)


class FocusedTraversalAndCombatContract(unittest.TestCase):
    @classmethod
    def setUpClass(cls) -> None:
        cls.controller = CONTROLLER_PATH.read_text(encoding="utf-8")
        cls.movement = SOURCE_PATH.read_text(encoding="utf-8")
        cls.header = HEADER_PATH.read_text(encoding="utf-8")
        start = cls.movement.index("void BotMovement::MoveThink")
        end = cls.movement.index(
            "void BotMovement::CheckAttractiveNodes", start
        )
        cls.ladder = cls.movement[start:end]
        start = cls.controller.index("void BotController::State_Attack")
        end = cls.controller.index(
            "bool BotController::CheckCondition_Grenade", start
        )
        cls.attack = cls.controller[start:end]

    def test_grounded_ladder_detach_commits_away_before_repath(self) -> None:
        self.assertIn("groundedLadderExit", self.ladder)
        self.assertIn(
            "(reachedLadderTop || groundedLadderExit)",
            self.ladder,
        )
        self.assertIn("m_vLadderExitDirection", self.ladder)
        self.assertIn("bot_ladder_ground_exit", self.ladder)
        self.assertIn("UnattachFromLadder(NULL)", self.ladder)
        self.assertIn("m_vLadderExitDirection *= -1.0f", self.ladder)
        self.assertLess(
            self.ladder.index("UnattachFromLadder(NULL)"),
            self.ladder.index("ContinueLadderExit(botcmd)"),
        )
        self.assertIn("ContinueLadderExit(botcmd)", self.ladder)

    def test_grounded_ladder_reacquisition_requires_clearance(self) -> None:
        self.assertIn(
            "BOT_LADDER_REATTACH_MAX_MSEC        = 2500",
            self.movement,
        )
        self.assertIn(
            "BOT_LADDER_REATTACH_CLEAR_DISTANCE = 128.0f",
            self.movement,
        )
        self.assertIn("m_iLadderReattachUntil", self.header)
        self.assertIn("m_bLadderReattachRecovery", self.header)
        self.assertIn(
            "m_iLadderExitUntil || m_iLadderReattachUntil",
            self.ladder,
        )
        self.assertIn("bot_ladder_reattach_blocked", self.ladder)
        self.assertIn("bot_ladder_reattach_clear", self.ladder)
        self.assertIn("bot_ladder_reattach_timeout", self.ladder)
        start = self.ladder.index(
            "bool BotMovement::ContinueLadderReattachGate"
        )
        gate = self.ladder[start:]
        self.assertIn(
            "DotProduct(displacement, m_vLadderExitDirection)",
            gate,
        )
        self.assertIn("BOT_LADDER_REATTACH_CLEAR_DISTANCE", gate)
        self.assertNotIn("FindPath", gate)
        self.assertLess(
            self.ladder.index("ContinueLadderExit(botcmd)"),
            self.ladder.index("ContinueLadderReattachGate(botcmd)"),
        )

    def test_close_pistol_alternates_cadenced_shots_with_bashes(self) -> None:
        self.assertIn(
            "BOT_PISTOL_BASH_SHOT_INTERVAL_MSEC = 1200",
            self.controller,
        )
        self.assertIn("m_iNextPistolBashShotTime", self.header)
        self.assertIn("pWeap->HasAmmoInClip(FIRE_PRIMARY)", self.attack)
        self.assertIn(
            "m_botCmd.buttons ^= BUTTON_ATTACKLEFT", self.attack
        )
        self.assertIn(
            "m_botCmd.buttons ^= BUTTON_ATTACKRIGHT", self.attack
        )
        self.assertIn(
            "+ BOT_PISTOL_BASH_SHOT_INTERVAL_MSEC",
            self.attack,
        )


if __name__ == "__main__":
    unittest.main()
