#!/usr/bin/env python3
"""Contracts for the fixed, aspect-corrected first-person viewmodel FOV."""

import math
from pathlib import Path
import unittest


ROOT = Path(__file__).resolve().parents[3]
BACKENDS = (
    ROOT / "code" / "renderergl1" / "tr_backend.c",
    ROOT / "code" / "renderergl2" / "tr_backend.c",
)
LAUNCHER = ROOT / "code" / "LauncherMac" / "Sources" / "LauncherMac"


def aspect_corrected_horizontal_fov(base_fov, width, height):
    ratio_from_4_3 = (width / height) * (3.0 / 4.0)
    return math.degrees(
        2.0 * math.atan(math.tan(math.radians(base_fov / 2.0)) * ratio_from_4_3)
    )


class ViewModelFovTests(unittest.TestCase):
    def test_original_fov_is_preserved_across_aspect_ratios(self):
        self.assertAlmostEqual(aspect_corrected_horizontal_fov(80.0, 4, 3), 80.0)
        self.assertAlmostEqual(
            aspect_corrected_horizontal_fov(80.0, 16, 9),
            96.42,
            places=2,
        )

    def test_both_renderers_use_the_fixed_projection_and_restore_world_fov(self):
        for path in BACKENDS:
            source = path.read_text()
            with self.subTest(renderer=path.parent.name):
                self.assertIn("RB_BuildFixedViewModelProjection", source)
                self.assertIn("tan(80.0f * M_PI / 360.0f)", source)
                self.assertIn("(3.0f / 4.0f)", source)
                self.assertIn("RF_FIRST_PERSON", source)
                self.assertIn("RF_CROSSHAIR", source)
                self.assertIn("oldViewModelProjection", source)
                self.assertIn("backEnd.viewParms.projectionMatrix", source)

    def test_no_setting_or_cvar_is_added(self):
        settings = (LAUNCHER / "LauncherSettings.swift").read_text()
        launcher = (LAUNCHER / "GameLauncher.swift").read_text()

        self.assertNotIn("viewmodelFov", settings)
        self.assertNotIn("viewmodel_fov", launcher)


if __name__ == "__main__":
    unittest.main()
