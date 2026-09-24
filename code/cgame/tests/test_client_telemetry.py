#!/usr/bin/env python3
"""Source-level contract tests for passive client movement telemetry."""

import ast
import csv
import io
from pathlib import Path
import re
import unittest


CGAME = Path(__file__).resolve().parents[1]
ROOT = CGAME.parents[1]
SOURCE = (CGAME / "cg_client_telemetry.cpp").read_text()


def csv_header(file_name):
    # The header is the string literal passed to the one multi-line Append().
    start = re.search(rf"Append\(\n\s*{file_name},", SOURCE).end()
    end = re.compile(r"\n\s*\);").search(SOURCE, start).start()
    literals = re.findall(r'"(?:[^"\\]|\\.)*"', SOURCE[start:end])
    text = "".join(ast.literal_eval(literal) for literal in literals).rstrip("\n")
    return next(csv.reader(io.StringIO(text)))


class ClientTelemetryTests(unittest.TestCase):
    def test_frame_and_input_schemas_are_stable(self):
        frame = csv_header("frameFile")
        inputs = csv_header("inputFile")

        self.assertEqual(len(frame), 113)
        self.assertEqual(len(inputs), 20)
        for name in (
            "lean_left",
            "lean_right",
            "command_distance",
            "crosshair_distance",
            "target_entity",
            "target_confidence",
            "nearest_visible_enemy_distance",
        ):
            self.assertIn(name, frame)

    def test_recorder_is_integrated_after_prediction_and_view_setup(self):
        view = (CGAME / "cg_view.c").read_text()
        self.assertLess(view.index("CG_PredictPlayerState();"), view.index("CG_ClientTelemetryFrame();"))
        self.assertLess(view.index("CG_CalcViewValues();"), view.index("CG_ClientTelemetryFrame();"))
        self.assertLess(view.index("CG_AddPacketEntities();"), view.index("CG_ClientTelemetryFrame();"))

    def test_recorder_is_local_and_omits_identity_data(self):
        self.assertIn("privacy=no_names_chat_or_network_addresses", SOURCE)
        for forbidden in ("cl_currentServerAddress", "CS_PLAYERS", "G_MoveLogChat"):
            self.assertNotIn(forbidden, SOURCE)

    def test_launcher_generates_a_unique_recording_token(self):
        launcher_dir = ROOT / "code" / "LauncherMac" / "Sources" / "LauncherMac"
        settings = (launcher_dir / "LauncherSettings.swift").read_text()
        launcher = (launcher_dir / "GameLauncher.swift").read_text()
        view = (launcher_dir / "ContentView.swift").read_text()

        self.assertIn("client_move_log", settings)
        self.assertIn('"cl_movelog", settings.clientMoveLog ? "1" : "0"', launcher)
        self.assertIn('"cl_movelog_session", UUID().uuidString', launcher)
        self.assertIn('Toggle("Record my movement"', view)


if __name__ == "__main__":
    unittest.main()
