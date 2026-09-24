#!/usr/bin/env python3
"""Source contracts for the macOS launcher's fullscreen switch."""

from pathlib import Path
import unittest


CLIENT = Path(__file__).resolve().parents[1]
ROOT = CLIENT.parents[1]
LAUNCHER = ROOT / "code" / "LauncherMac" / "Sources" / "LauncherMac"


class LauncherFullscreenTests(unittest.TestCase):
    def test_switch_is_persisted_and_defaults_to_existing_behavior(self):
        settings = (LAUNCHER / "LauncherSettings.swift").read_text()
        content = (LAUNCHER / "ContentView.swift").read_text()
        view = (LAUNCHER / "BotsView.swift").read_text()

        self.assertIn("@Published var fullscreenEnabled: Bool = true", settings)
        self.assertIn('case "fullscreen_enabled"', settings)
        self.assertIn('lines.append("fullscreen_enabled=', settings)
        self.assertIn(
            ".onChange(of: settings.fullscreenEnabled) { _ in settings.save() }",
            content,
        )
        self.assertIn(
            'Toggle("Fullscreen", isOn: $settings.fullscreenEnabled)',
            view,
        )

    def test_both_launch_paths_pass_r_fullscreen(self):
        launcher = (LAUNCHER / "GameLauncher.swift").read_text()

        self.assertIn(
            '"r_fullscreen", settings.fullscreenEnabled ? "1" : "0"',
            launcher,
        )
        self.assertEqual(
            launcher.count("appendResolutionArgs(&args, settings: settings)"),
            2,
        )


if __name__ == "__main__":
    unittest.main()
