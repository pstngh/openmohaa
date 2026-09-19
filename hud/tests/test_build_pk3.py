#!/usr/bin/env python3
"""Checks that build_pk3.py packs every layout reproducibly."""

from pathlib import Path
import subprocess
import sys
import tempfile
import unittest
import zipfile


HUD = Path(__file__).resolve().parents[1]
BUILDER = HUD / "build_pk3.py"


def build(output: Path) -> None:
    subprocess.run(
        [sys.executable, str(BUILDER), "--output", str(output)],
        check=True,
        stdout=subprocess.DEVNULL,
    )


class BuildPk3Tests(unittest.TestCase):
    def test_build_is_reproducible_and_complete(self):
        with tempfile.TemporaryDirectory() as directory:
            first = Path(directory) / "a" / "zzz_openmohaa-hud.pk3"
            second = Path(directory) / "b" / "zzz_openmohaa-hud.pk3"
            build(first)
            build(second)

            self.assertEqual(first.read_bytes(), second.read_bytes())
            with zipfile.ZipFile(first) as archive:
                self.assertIsNone(archive.testzip())
                expected = sorted(
                    (f"ui/{path.name}" for path in (HUD / "ui").glob("*.urc")),
                    key=str.casefold,
                )
                self.assertEqual(archive.namelist(), expected)
                for name in expected:
                    self.assertEqual(
                        archive.read(name), (HUD / name).read_bytes(), name
                    )


if __name__ == "__main__":
    unittest.main()
