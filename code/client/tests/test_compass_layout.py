#!/usr/bin/env python3
"""Source contracts for automatic compass-safe message layout."""

from pathlib import Path
import unittest


CLIENT = Path(__file__).resolve().parents[1]
ROOT = CLIENT.parents[1]
SOURCE = (CLIENT / "cl_ui.cpp").read_text()
LAUNCHER = ROOT / "code" / "LauncherMac" / "Sources" / "LauncherMac"


class CompassLayoutTests(unittest.TestCase):
    def test_layout_uses_real_compass_bounds(self):
        cache_start = SOURCE.index("static void cacheCompassMessageLayout(void)")
        cache_end = SOURCE.index("static UIRect2D getDefaultGMBoxRectangle", cache_start)
        cache = SOURCE[cache_start:cache_end]

        self.assertIn("widget->getFrame()", cache)
        self.assertIn("frame.getMaxX()", cache)
        self.assertIn("frame.getMaxY()", cache)

    def test_hidden_compass_uses_normal_left_margin(self):
        # Match the definitions, not the forward declarations.
        dm_start = SOURCE.index("static UIRect2D getDefaultDMBoxRectangle(void)\n{")
        dm_end = SOURCE.index("static void updateCompassAwareMessageBoxLayout(void)\n{", dm_start)
        dm_layout = SOURCE[dm_start:dm_end]

        self.assertIn("if (!ui_compass || !ui_compass->integer)", dm_layout)
        self.assertIn("left = 20.0f * scaleX;", dm_layout)

    def test_expensive_layout_only_refreshes_when_needed(self):
        self.assertIn(
            "ui_compass->modificationCount != s_compassLayoutModificationCount",
            SOURCE,
        )
        self.assertIn(
            "menuManager.RealignMenus();\n    updateCompassAwareMessageBoxLayout();",
            SOURCE,
        )

    def test_launcher_persists_and_passes_compass_toggle(self):
        settings = (LAUNCHER / "LauncherSettings.swift").read_text()
        launcher = (LAUNCHER / "GameLauncher.swift").read_text()
        view = (LAUNCHER / "ContentView.swift").read_text()

        self.assertIn("@Published var compassEnabled: Bool = true", settings)
        self.assertIn('case "compass_enabled"', settings)
        self.assertIn('"ui_compass", settings.compassEnabled ? "1" : "0"', launcher)
        self.assertIn('Toggle("Compass", isOn: $settings.compassEnabled)', view)


if __name__ == "__main__":
    unittest.main()
