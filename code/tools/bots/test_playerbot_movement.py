#!/usr/bin/env python3
"""Focused source contract for human-frequency roaming strafe and wall veto."""

from __future__ import annotations

import re
import unittest
from pathlib import Path


ROOT = Path(__file__).resolve().parents[3]
SOURCE_PATH = ROOT / "code" / "fgame" / "playerbot_movement.cpp"
HEADER_PATH = ROOT / "code" / "fgame" / "playerbot.h"


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


if __name__ == "__main__":
    unittest.main()
