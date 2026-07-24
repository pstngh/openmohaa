#!/usr/bin/env python3
"""Behavioral contracts for nullbinds, run against the real cl_input.cpp code.

The button helpers and their command handlers are lifted verbatim from
cl_input.cpp, compiled with minimal engine stubs and driven through ctypes.
"""

import ctypes
import os
import re
import shlex
import subprocess
import tempfile
import unittest
from pathlib import Path


CLIENT = Path(__file__).resolve().parents[1]
FUNCTIONS = (
    "IN_KeyDown", "IN_KeyUp", "CL_KeyState",
    "CL_NullbindHeld", "CL_NullbindSuppress", "CL_NullbindKey",
    "IN_ForwardDown", "IN_ForwardUp", "IN_BackDown", "IN_BackUp",
    "IN_MoveleftDown", "IN_MoveleftUp", "IN_MoverightDown", "IN_MoverightUp",
    "IN_LeanLeftDown", "IN_LeanLeftUp", "IN_LeanRightDown", "IN_LeanRightUp",
    "IN_Button4Down", "IN_Button4Up", "IN_Button5Down", "IN_Button5Up",
)

PRELUDE = r"""
#include <cstdio>
#include <cstdlib>
#include <cstring>

typedef int qboolean;
enum { qfalse, qtrue };

static char argv1[16], argv2[16];
char *Cmd_Argv(int arg) { return arg == 1 ? argv1 : arg == 2 ? argv2 : (char *)""; }
void Com_Printf(const char *, ...) {}

unsigned frame_msec;
int com_frameTime;
"""

HARNESS = r"""
static kbutton_t *pairs[3][2] = {
    { &in_forward, &in_back }, { &in_moveleft, &in_moveright }, { &in_buttons[4], &in_buttons[5] },
};

extern "C" {
void nb_reset(void) {
    for (int i = 0; i < 3; i++) {
        memset(pairs[i][0], 0, sizeof(kbutton_t));
        memset(pairs[i][1], 0, sizeof(kbutton_t));
    }
    frame_msec = 8;
    com_frameTime = 0;
}
void nb_args(int key, int time) {
    argv1[0] = argv2[0] = 0;
    if (key) {
        snprintf(argv1, sizeof(argv1), "%i", key);
        snprintf(argv2, sizeof(argv2), "%i", time);
    }
}
void nb_frame(int time, unsigned msec) { com_frameTime = time; frame_msec = msec; }
float nb_sample(int pair, int side) { return CL_KeyState(pairs[pair][side]); }
int nb_active(int pair, int side) { return pairs[pair][side]->active; }
// the button bits CL_CmdButtons reports for the lean pair
int nb_lean_bits(void) {
    int bits = 0;
    for (int side = 0; side < 2; side++) {
        kbutton_t *b = pairs[2][side];
        if (b->active || b->wasPressed) {
            bits |= 1 << side;
        }
        b->wasPressed = qfalse;
    }
    return bits;
}
}
"""


def definition(source, name):
    match = re.search(rf"^[\w ]*\b{name}\s*\([^)]*\)\s*\{{", source, re.M)
    if not match:
        raise AssertionError(f"{name} not found in cl_input.cpp")
    depth, end = 0, match.end() - 1
    for end in range(end, len(source)):
        depth += {"{": 1, "}": -1}.get(source[end], 0)
        if not depth:
            break
    return source[match.start():end + 1]


def build(directory):
    cl_input = (CLIENT / "cl_input.cpp").read_text()
    kbutton = re.search(r"typedef struct \{[^}]*\} kbutton_t;", (CLIENT / "client.h").read_text())
    source = "\n".join([
        PRELUDE,
        kbutton.group(0),
        "kbutton_t in_forward, in_back, in_moveleft, in_moveright, in_buttons[16];",
        'extern "C" {',
        *(definition(cl_input, name) for name in FUNCTIONS),
        "}",
        HARNESS,
    ])
    (directory / "nullbind.cpp").write_text(source)

    compiler = shlex.split(os.environ.get("CXX") or "c++")
    library = directory / "nullbind.so"
    subprocess.run(
        [*compiler, "-std=c++11", "-shared", "-fPIC", "-o", str(library), str(directory / "nullbind.cpp")],
        check=True,
    )
    lib = ctypes.CDLL(str(library))
    lib.nb_frame.argtypes = (ctypes.c_int, ctypes.c_uint)
    lib.nb_sample.restype = ctypes.c_float
    return lib


FORWARD, STRAFE, LEAN = range(3)
LEFT, RIGHT = 1, 2
HANDLERS = {
    FORWARD: (("IN_ForwardDown", "IN_ForwardUp"), ("IN_BackDown", "IN_BackUp")),
    STRAFE: (("IN_MoveleftDown", "IN_MoveleftUp"), ("IN_MoverightDown", "IN_MoverightUp")),
    LEAN: (("IN_LeanLeftDown", "IN_LeanLeftUp"), ("IN_LeanRightDown", "IN_LeanRightUp")),
}


def setUpModule():
    global lib
    directory = tempfile.TemporaryDirectory()
    unittest.addModuleCleanup(directory.cleanup)
    lib = build(Path(directory.name))


class PairContract:
    """Nullbind behavior every opposing pair shares; subclasses pick the pair."""

    pair = None

    def setUp(self):
        lib.nb_reset()

    def run_command(self, name, key, time):
        lib.nb_args(key, time)
        getattr(lib, name)()

    def press(self, side, key, time):
        self.run_command(HANDLERS[self.pair][side][0], key, time)

    def release(self, side, key, time):
        self.run_command(HANDLERS[self.pair][side][1], key, time)

    def sample(self, time, msec=8):
        lib.nb_frame(time, msec)
        return lib.nb_sample(self.pair, 0), lib.nb_sample(self.pair, 1)

    def active(self):
        return bool(lib.nb_active(self.pair, 0)), bool(lib.nb_active(self.pair, 1))

    def test_last_press_wins_with_partial_frame_timing(self):
        self.press(0, 1, 100)
        self.assertEqual(self.sample(108), (1.0, 0.0))
        self.press(1, 2, 110)
        self.assertEqual(self.sample(116), (0.25, 0.75))
        self.release(1, 2, 118)
        self.assertEqual(self.sample(124), (0.75, 0.25))
        self.press(1, 2, 126)
        self.press(0, 3, 128)
        self.assertEqual(self.sample(132), (0.75, 0.25))

    def test_suppression_credits_time_beyond_the_previous_frame_length(self):
        self.press(0, 1, 100)
        self.press(1, 2, 112)
        self.assertEqual(self.sample(116, msec=16), (0.75, 0.25))

    def test_releasing_a_suppressed_key_is_not_credited_twice(self):
        for side in (0, 1):
            with self.subTest(side=side):
                lib.nb_reset()
                self.press(side, 1, 100)
                self.press(side ^ 1, 2, 104)
                self.assertEqual(self.sample(108), (0.5, 0.5))
                self.release(side, 1, 110)
                fractions = self.sample(116)
                self.assertEqual((fractions[side], fractions[side ^ 1]), (0.0, 1.0))

    def test_repeats_third_keys_and_unmatched_releases_change_nothing(self):
        self.press(0, 1, 100)
        self.press(1, 2, 102)
        for event, side, key in ((self.press, 0, 1), (self.press, 1, 3), (self.press, 1, 4), (self.release, 0, 9)):
            event(side, key, 104)
            self.assertEqual(self.active(), (False, True))
        self.release(1, 2, 106)
        self.release(1, 3, 106)
        self.assertEqual(self.active(), (True, False))

    def test_new_binding_on_the_suppressed_side_wins(self):
        self.press(0, 1, 100)
        self.press(1, 2, 102)
        self.press(0, 3, 104)
        self.assertEqual(self.active(), (True, False))
        self.release(0, 3, 106)
        self.assertEqual(self.active(), (True, False))
        self.release(0, 1, 108)
        self.assertEqual(self.active(), (False, True))

    def test_console_commands_keep_stock_timeless_handling(self):
        self.press(0, 1, 100)
        self.sample(108)
        self.press(1, 0, 0)
        self.assertEqual(self.sample(116), (0.5, 1.0))
        self.release(0, 0, 0)
        self.release(1, 0, 0)
        self.assertEqual(self.active(), (False, False))

    def test_timer_wrap_counts_forward_and_out_of_order_time_counts_nothing(self):
        self.press(0, 1, -4)
        self.press(1, 2, 3)
        self.assertEqual(self.sample(3), (7 / 8, 0.0))
        lib.nb_reset()
        self.press(0, 1, 100)
        self.press(1, 2, 99)
        self.release(1, 2, 101)
        self.assertEqual(self.sample(108)[0], 7 / 8)


class ForwardBackTests(PairContract, unittest.TestCase):
    pair = FORWARD


class StrafeTests(PairContract, unittest.TestCase):
    pair = STRAFE


class LeanTests(PairContract, unittest.TestCase):
    pair = LEAN

    def tap(self, side, key, time):
        self.press(side, key, time)
        self.release(side, key, time + 1)

    def test_lean_reports_one_side_per_command(self):
        self.tap(0, 1, 100)
        self.tap(1, 2, 102)
        self.assertEqual(lib.nb_lean_bits(), RIGHT)

        self.press(1, 2, 110)
        self.assertEqual(lib.nb_lean_bits(), RIGHT)
        self.tap(0, 1, 112)
        self.assertEqual(lib.nb_lean_bits(), RIGHT)
        self.press(0, 1, 120)
        self.assertEqual(lib.nb_lean_bits(), LEFT)

    def test_button4_and_button5_share_the_lean_pair(self):
        self.run_command("IN_LeanRightDown", 1, 100)
        self.run_command("IN_Button4Down", 2, 102)
        self.assertEqual(self.active(), (True, False))
        self.run_command("IN_Button4Up", 2, 104)
        self.assertEqual(self.active(), (False, True))
        self.run_command("IN_LeanRightUp", 1, 106)
        self.run_command("IN_LeanLeftDown", 3, 108)
        self.run_command("IN_Button5Down", 4, 110)
        self.assertEqual(self.active(), (False, True))
        self.run_command("IN_Button5Up", 4, 112)
        self.assertEqual(self.active(), (True, False))


if __name__ == "__main__":
    unittest.main()
