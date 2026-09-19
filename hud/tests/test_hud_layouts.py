#!/usr/bin/env python3
"""Structural and geometry checks for the custom HUD URC layouts."""

from pathlib import Path
import re
import unittest


HUD = Path(__file__).resolve().parents[1]
UI = HUD / "ui"

MENU_RE = re.compile(
    r'^menu\s+"([^"]*)"\s+(\d+)\s+(\d+)\s+(\S+)\s+(\S+)', re.MULTILINE
)
RESOURCE_RE = re.compile(r"resource\s+Label\s*\{(.*?)\}", re.DOTALL)
NAME_RE = re.compile(r'^name\s+"([^"]*)"', re.MULTILINE)
RECT_RE = re.compile(r"^rect\s+(-?\d+)\s+(-?\d+)\s+(\d+)\s+(\d+)", re.MULTILINE)

AMMO_HEIGHTS = {
    "hud_ammo_M2grenade.urc": 200,
    "hud_ammo_bar.urc": 248,
    "hud_ammo_bazooka.urc": 200,
    "hud_ammo_colt45.urc": 136,
    "hud_ammo_empty.urc": 128,
    "hud_ammo_garand.urc": 136,
    "hud_ammo_kar98.urc": 136,
    "hud_ammo_kar98sniper.urc": 136,
    "hud_ammo_mp40.urc": 264,
    "hud_ammo_mp44.urc": 248,
    "hud_ammo_p38.urc": 136,
    "hud_ammo_panzerschreck.urc": 200,
    "hud_ammo_shotgun.urc": 136,
    "hud_ammo_silencedpistol.urc": 136,
    "hud_ammo_springfield.urc": 136,
    "hud_ammo_steilhandgranate.urc": 200,
    "hud_ammo_thompson.urc": 248,
}


def source(path: Path) -> str:
    return re.sub(r"//.*", "", path.read_text())


def menu(text: str) -> tuple[str, int, int]:
    match = MENU_RE.search(text)
    if not match:
        raise AssertionError("missing or malformed menu declaration")
    return match.group(1), int(match.group(2)), int(match.group(3))


def resources(text: str) -> dict[str, tuple[int, int, int, int]]:
    found = {}
    for block in RESOURCE_RE.findall(text):
        name_match = NAME_RE.search(block)
        rect_match = RECT_RE.search(block)
        if name_match and rect_match:
            found[name_match.group(1)] = tuple(map(int, rect_match.groups()))
    return found


class HudLayoutTests(unittest.TestCase):
    def test_all_stock_ammo_variants_are_overridden(self):
        actual = {path.name for path in UI.glob("hud_ammo_*.urc")}
        self.assertEqual(actual, set(AMMO_HEIGHTS))

    def test_ammo_names_are_removed_and_bottom_shift_is_bounded(self):
        for filename, expected_height in AMMO_HEIGHTS.items():
            with self.subTest(filename=filename):
                text = source(UI / filename)
                _, width, height = menu(text)
                self.assertEqual(height, expected_height)
                self.assertIn("align right bottom", text)
                self.assertNotIn("weaponname", text.lower())
                self.assertNotRegex(text, r"\bitemstat\s+1\b")

                widgets = resources(text)
                for name, (x, y, widget_width, widget_height) in widgets.items():
                    self.assertGreaterEqual(x, 0, name)
                    self.assertGreaterEqual(y, 0, name)
                    self.assertLessEqual(x + widget_width, width, name)
                    self.assertLessEqual(y + widget_height, height, name)

                count_y = [
                    rect[1]
                    for name, rect in widgets.items()
                    if name in {"ammoseperator", "ammocount", "clipcount"}
                ]
                graphic_y = [
                    rect[1]
                    for name, rect in widgets.items()
                    if name in {"ammobar", "clipbulletse", "clipbulletso"}
                ]
                if count_y and graphic_y:
                    self.assertLess(min(graphic_y), min(count_y))

    def test_health_geometry(self):
        text = source(UI / "hud_health.urc")
        name, width, height = menu(text)
        self.assertEqual((name, width, height), ("hud_health", 128, 136))
        self.assertIn("align left bottom", text)
        widgets = resources(text)
        self.assertEqual(widgets["Default"], (16, 56, 16, 64))
        self.assertEqual(widgets["healthmeter"], (16, 56, 16, 64))
        self.assertEqual(widgets["healthnumber"], (0, 120, 48, 16))
        self.assertEqual(text.count('name "healthnumber"'), 1)
        self.assertIn("textalign centerx", text)

    def test_timer_is_top_right(self):
        text = source(UI / "hud_timelimit.urc")
        self.assertEqual(menu(text), ("hud_timelimit", 256, 24))
        self.assertIn("align right top", text)
        self.assertIn("textalign right", text)
        self.assertIn('linkcvar "ui_timemessage"', text)
        self.assertEqual(resources(text)["Default"], (0, 0, 246, 24))

    def test_layout_tokens_are_balanced(self):
        for path in UI.glob("*.urc"):
            with self.subTest(path=path.name):
                text = source(path)
                self.assertEqual(text.count("{"), text.count("}"))
                self.assertEqual(text.count("menu "), 1)
                self.assertEqual(text.count("end."), 1)


if __name__ == "__main__":
    unittest.main()
