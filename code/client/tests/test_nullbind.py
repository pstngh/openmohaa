#!/usr/bin/env python3
"""Behavioral specification for cl_nullbind's event-level state machine."""

from pathlib import Path
import unittest


UINT_MASK = 0xFFFFFFFF


class Button:
    def __init__(self):
        self.holders = []
        self.downtime = 0
        self.msec = 0
        self.active = False
        self.was_pressed = False


class Pair:
    def __init__(self):
        self.buttons = (Button(), Button())
        self.last_pressed = -1


def held(button):
    return bool(button.holders)


def suppress(button, time, frame_msec):
    if not button.active:
        return
    if not time or not button.downtime:
        button.msec += frame_msec // 2
    else:
        elapsed = (time - button.downtime) & UINT_MASK
        if 0 < elapsed < 0x80000000:
            button.msec += min(elapsed, frame_msec)
    button.active = False


def engage(button, time):
    if not button.active and held(button):
        button.downtime = time
        button.active = True


def preferred_side(pair):
    states = (held(pair.buttons[0]), held(pair.buttons[1]))
    if pair.last_pressed >= 0 and states[pair.last_pressed]:
        return pair.last_pressed
    if states[0] != states[1]:
        return 0 if states[0] else 1
    if states[0]:
        active = (pair.buttons[0].active, pair.buttons[1].active)
        if active[0] != active[1]:
            return 0 if active[0] else 1
        return 0
    return -1


def apply(pair, time, frame_msec, enabled):
    preferred = preferred_side(pair) if enabled else -1
    for side, button in enumerate(pair.buttons):
        should_be_active = side == preferred if enabled else held(button)
        if should_be_active:
            engage(button, time)
        else:
            suppress(button, time, frame_msec)


def press(pair, side, key, time, frame_msec, enabled):
    button = pair.buttons[side]
    if key in button.holders or len(button.holders) == 2:
        return False
    button.holders.append(key)
    if not button.active:
        button.downtime = time
        button.active = True
        button.was_pressed = True
    pair.last_pressed = side
    apply(pair, time, frame_msec, enabled)
    return True


def release(pair, side, key, time, frame_msec, enabled):
    button = pair.buttons[side]
    if key not in button.holders:
        return False
    suppressed = not button.active and held(button)
    old_msec = button.msec
    button.holders.remove(key)
    if not held(button):
        button.active = False
        button.msec += ((time - button.downtime) & UINT_MASK) if time else frame_msec // 2
    if suppressed:
        button.msec = old_msec
    apply(pair, time, frame_msec, enabled)
    return True


def sample(button, time, frame_msec):
    msec = button.msec
    button.msec = 0
    if button.active:
        if not button.downtime:
            msec = time
        else:
            msec += (time - button.downtime) & UINT_MASK
        button.downtime = time
    return min(msec, frame_msec)


class NullbindTests(unittest.TestCase):
    frame_msec = 8

    def test_last_press_wins_with_exact_fallback_timing(self):
        pair = Pair()
        press(pair, 0, 1, 100, self.frame_msec, True)
        self.assertEqual(sample(pair.buttons[0], 108, self.frame_msec), 8)

        press(pair, 1, 2, 110, self.frame_msec, True)
        self.assertEqual(sample(pair.buttons[0], 116, self.frame_msec), 2)
        self.assertEqual(sample(pair.buttons[1], 116, self.frame_msec), 6)

        release(pair, 1, 2, 118, self.frame_msec, True)
        self.assertEqual(sample(pair.buttons[0], 124, self.frame_msec), 6)
        self.assertEqual(sample(pair.buttons[1], 124, self.frame_msec), 2)

    def test_new_second_binding_wins_but_repeat_does_not(self):
        pair = Pair()
        press(pair, 0, 1, 100, self.frame_msec, True)
        press(pair, 1, 2, 102, self.frame_msec, True)
        self.assertFalse(press(pair, 0, 1, 104, self.frame_msec, True))
        self.assertEqual(pair.last_pressed, 1)

        self.assertTrue(press(pair, 0, 3, 106, self.frame_msec, True))
        self.assertEqual(pair.last_pressed, 0)
        release(pair, 0, 3, 108, self.frame_msec, True)
        self.assertTrue(pair.buttons[0].active)
        self.assertFalse(pair.buttons[1].active)
        release(pair, 0, 1, 110, self.frame_msec, True)
        self.assertFalse(pair.buttons[0].active)
        self.assertTrue(pair.buttons[1].active)

    def test_enabling_mid_press_uses_recorded_order(self):
        pair = Pair()
        press(pair, 0, 1, 100, self.frame_msec, False)
        press(pair, 1, 2, 102, self.frame_msec, False)
        self.assertTrue(pair.buttons[0].active and pair.buttons[1].active)

        apply(pair, 108, self.frame_msec, True)
        self.assertFalse(pair.buttons[0].active)
        self.assertTrue(pair.buttons[1].active)

    def test_timer_wrap_is_treated_as_forward_progress(self):
        button = Button()
        button.holders.append(1)
        button.active = True
        button.downtime = 0xFFFFFFFC
        suppress(button, 3, self.frame_msec)
        self.assertEqual(button.msec, 7)

        button.active = True
        button.downtime = 100
        button.msec = 0
        suppress(button, 99, self.frame_msec)
        self.assertEqual(button.msec, 0)

    def test_last_subframe_lean_tap_breaks_button_tie(self):
        pair = Pair()
        press(pair, 0, 1, 100, self.frame_msec, True)
        release(pair, 0, 1, 102, self.frame_msec, True)
        press(pair, 1, 2, 103, self.frame_msec, True)
        release(pair, 1, 2, 105, self.frame_msec, True)
        self.assertEqual(preferred_side(pair), -1)
        self.assertEqual(pair.last_pressed, 1)

    def test_all_lean_command_aliases_use_event_wrappers(self):
        source = (Path(__file__).resolve().parents[1] / "cl_input.cpp").read_text()
        for command, handler in (
            ("+leanleft", "IN_NullbindLeanLeftDown"),
            ("-leanleft", "IN_NullbindLeanLeftUp"),
            ("+leanright", "IN_NullbindLeanRightDown"),
            ("-leanright", "IN_NullbindLeanRightUp"),
            ("+button4", "IN_NullbindLeanLeftDown"),
            ("-button4", "IN_NullbindLeanLeftUp"),
            ("+button5", "IN_NullbindLeanRightDown"),
            ("-button5", "IN_NullbindLeanRightUp"),
        ):
            self.assertIn(f'Cmd_AddCommand("{command}", {handler});', source)


if __name__ == "__main__":
    unittest.main()
