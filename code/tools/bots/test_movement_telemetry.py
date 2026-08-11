#!/usr/bin/env python3
"""Focused source contracts for server telemetry rotation."""

from __future__ import annotations

import unittest
from pathlib import Path


ROOT = Path(__file__).resolve().parents[3]
SOURCE_PATH = ROOT / "code" / "fgame" / "movement_telemetry.cpp"


class TelemetryRotationContract(unittest.TestCase):
    @classmethod
    def setUpClass(cls) -> None:
        cls.source = SOURCE_PATH.read_text(encoding="utf-8")
        start = cls.source.index("static void RotateTelemetryIfNeeded")
        end = cls.source.index("} // namespace", start)
        cls.rotation = cls.source[start:end]

    def test_large_capture_rotates_before_signed_32_bit_boundary(self) -> None:
        self.assertIn("MOVELOG_SEGMENT_LIMIT", self.source)
        self.assertIn("1536ULL * 1024ULL * 1024ULL", self.source)
        self.assertIn("framesBytesWritten += gi.FS_Write", self.source)
        self.assertIn("framesBuffer.size()", self.rotation)

    def test_rotation_preserves_complete_standard_triplets(self) -> None:
        self.assertIn('"telemetry/segments/"', self.source)
        self.assertIn('"/movement_frames.csv"', self.source)
        self.assertIn('"/movement_events.csv"', self.source)
        self.assertIn('"/movement_meta.txt"', self.source)
        self.assertIn('AppendEventRow("session_end"', self.source)
        self.assertLess(
            self.rotation.index("CloseTelemetrySession()"),
            self.rotation.index("SelectFreshSegmentPaths()"),
        )
        self.assertLess(
            self.rotation.index("SelectFreshSegmentPaths()"),
            self.rotation.index("EnsureOpen()"),
        )
        shutdown = self.source[self.source.index("void G_MoveLogShutdown"):]
        self.assertIn("CloseTelemetrySession();", shutdown)


if __name__ == "__main__":
    unittest.main()
