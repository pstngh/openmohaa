#!/usr/bin/env python3
"""
demoscan.py - Scan MoHAA .dm3 demo files for player names and loadout info.

Reads all .dm3 files in a folder (recursively) and outputs a text file
listing every demo and the players who appear in it, including mid-game joins.
For AA demos (protocol 6/8), also extracts the recording player's primary
weapon(s) and grenade count at spawn (6 = default, 3 = realism mode).

Usage:
    python demoscan.py <folder> [-o output.txt] [-p PlayerName]

Examples:
    python demoscan.py C:\mohaa\demos
    python demoscan.py C:\mohaa\demos -o results.txt
    python demoscan.py C:\mohaa\demos -p "SomeName"
    python demoscan.py C:\mohaa\demos -p "SomeName" -p "OtherPlayer"

Copyright (C) 2026 the OpenMoHAA team

This file is part of OpenMoHAA source code.

OpenMoHAA source code is free software; you can redistribute it
and/or modify it under the terms of the GNU General Public License as
published by the Free Software Foundation; either version 2 of the License,
or (at your option) any later version.

OpenMoHAA source code is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with OpenMoHAA source code; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
"""

import argparse
import os
import struct
import sys

# ---------------------------------------------------------------------------
# Quake 3 / MoHAA adaptive Huffman codec
# ---------------------------------------------------------------------------

HMAX = 256
NYT = HMAX
INTERNAL_NODE = HMAX + 1


class HuffNode:
    """A node in the adaptive Huffman tree."""

    __slots__ = (
        "left",
        "right",
        "parent",
        "next",
        "prev",
        "head_ref",
        "weight",
        "symbol",
    )

    def __init__(self):
        self.left = None
        self.right = None
        self.parent = None
        self.next = None
        self.prev = None
        self.head_ref = None  # index into head_ptrs list
        self.weight = 0
        self.symbol = 0


class HuffTree:
    """Adaptive Huffman tree matching the Quake 3 / MoHAA implementation."""

    def __init__(self):
        self.node_list = [HuffNode() for _ in range(768)]
        self.bloc_node = 0
        self.head_ptrs = [None] * 768  # stores node indices or None
        self.bloc_ptrs = 0
        self.free_list = []
        self.tree = None
        self.lhead = None
        self.loc = [None] * (HMAX + 1)

        # Initialize with NYT node
        nyt = self.node_list[self.bloc_node]
        self.bloc_node += 1
        nyt.symbol = NYT
        nyt.weight = 0
        nyt.next = None
        nyt.prev = None
        nyt.parent = None
        nyt.left = None
        nyt.right = None
        hp = self._get_head_ptr()
        nyt.head_ref = hp
        self.head_ptrs[hp] = nyt
        self.tree = nyt
        self.lhead = nyt
        self.loc[NYT] = nyt

    def _get_head_ptr(self):
        if self.free_list:
            return self.free_list.pop()
        idx = self.bloc_ptrs
        self.bloc_ptrs += 1
        return idx

    def _free_head_ptr(self, idx):
        self.free_list.append(idx)

    def _swap(self, node1, node2):
        par1 = node1.parent
        par2 = node2.parent
        if par1:
            if par1.left is node1:
                par1.left = node2
            else:
                par1.right = node2
        else:
            self.tree = node2
        if par2:
            if par2.left is node2:
                par2.left = node1
            else:
                par2.right = node1
        else:
            self.tree = node1
        node1.parent = par2
        node2.parent = par1

    def _swap_list(self, node1, node2):
        par1 = node1.next
        node1.next = node2.next
        node2.next = par1
        par1 = node1.prev
        node1.prev = node2.prev
        node2.prev = par1
        if node1.next is node1:
            node1.next = node2
        if node2.next is node2:
            node2.next = node1
        if node1.next:
            node1.next.prev = node1
        if node2.next:
            node2.next.prev = node2
        if node1.prev:
            node1.prev.next = node1
        if node2.prev:
            node2.prev.next = node2

    def _increment(self, node):
        if not node:
            return
        if node.next is not None and node.next.weight == node.weight:
            lnode = self.head_ptrs[node.head_ref]
            if lnode is not node.parent:
                self._swap(lnode, node)
            self._swap_list(lnode, node)
        if node.prev and node.prev.weight == node.weight:
            self.head_ptrs[node.head_ref] = node.prev
        else:
            self.head_ptrs[node.head_ref] = None
            self._free_head_ptr(node.head_ref)
        node.weight += 1
        if node.next and node.next.weight == node.weight:
            node.head_ref = node.next.head_ref
        else:
            hp = self._get_head_ptr()
            node.head_ref = hp
            self.head_ptrs[hp] = node
        if node.parent:
            self._increment(node.parent)
            if node.prev is node.parent:
                self._swap_list(node, node.parent)
                if self.head_ptrs[node.head_ref] is node:
                    self.head_ptrs[node.head_ref] = node.parent

    def add_ref(self, ch):
        if self.loc[ch] is None:
            tnode = self.node_list[self.bloc_node]
            self.bloc_node += 1
            tnode2 = self.node_list[self.bloc_node]
            self.bloc_node += 1

            tnode2.symbol = INTERNAL_NODE
            tnode2.weight = 1
            tnode2.next = self.lhead.next
            if self.lhead.next:
                self.lhead.next.prev = tnode2
                if self.lhead.next.weight == 1:
                    tnode2.head_ref = self.lhead.next.head_ref
                else:
                    hp = self._get_head_ptr()
                    tnode2.head_ref = hp
                    self.head_ptrs[hp] = tnode2
            else:
                hp = self._get_head_ptr()
                tnode2.head_ref = hp
                self.head_ptrs[hp] = tnode2
            self.lhead.next = tnode2
            tnode2.prev = self.lhead

            tnode.symbol = ch
            tnode.weight = 1
            tnode.next = self.lhead.next
            if self.lhead.next:
                self.lhead.next.prev = tnode
                if self.lhead.next.weight == 1:
                    tnode.head_ref = self.lhead.next.head_ref
                else:
                    hp = self._get_head_ptr()
                    tnode.head_ref = hp
                    self.head_ptrs[hp] = tnode2
            else:
                hp = self._get_head_ptr()
                tnode.head_ref = hp
                self.head_ptrs[hp] = tnode
            self.lhead.next = tnode
            tnode.prev = self.lhead
            tnode.left = None
            tnode.right = None

            if self.lhead.parent:
                if self.lhead.parent.left is self.lhead:
                    self.lhead.parent.left = tnode2
                else:
                    self.lhead.parent.right = tnode2
            else:
                self.tree = tnode2

            tnode2.right = tnode
            tnode2.left = self.lhead
            tnode2.parent = self.lhead.parent
            self.lhead.parent = tnode2
            tnode.parent = tnode2

            self.loc[ch] = tnode
            self._increment(tnode2.parent)
        else:
            self._increment(self.loc[ch])


# Pre-built frequency table from msg.cpp
MSG_HDATA = [
    250315, 41193, 6292, 7106, 3730, 3750, 6110, 23283,
    33317, 6950, 7838, 9714, 9257, 17259, 3949, 1778,
    8288, 1604, 1590, 1663, 1100, 1213, 1238, 1134,
    1749, 1059, 1246, 1149, 1273, 4486, 2805, 3472,
    21819, 1159, 1670, 1066, 1043, 1012, 1053, 1070,
    1726, 888, 1180, 850, 960, 780, 1752, 3296,
    10630, 4514, 5881, 2685, 4650, 3837, 2093, 1867,
    2584, 1949, 1972, 940, 1134, 1788, 1670, 1206,
    5719, 6128, 7222, 6654, 3710, 3795, 1492, 1524,
    2215, 1140, 1355, 971, 2180, 1248, 1328, 1195,
    1770, 1078, 1264, 1266, 1168, 965, 1155, 1186,
    1347, 1228, 1529, 1600, 2617, 2048, 2546, 3275,
    2410, 3585, 2504, 2800, 2675, 6146, 3663, 2840,
    14253, 3164, 2221, 1687, 3208, 2739, 3512, 4796,
    4091, 3515, 5288, 4016, 7937, 6031, 5360, 3924,
    4892, 3743, 4566, 4807, 5852, 6400, 6225, 8291,
    23243, 7838, 7073, 8935, 5437, 4483, 3641, 5256,
    5312, 5328, 5370, 3492, 2458, 1694, 1821, 2121,
    1916, 1149, 1516, 1367, 1236, 1029, 1258, 1104,
    1245, 1006, 1149, 1025, 1241, 952, 1287, 997,
    1713, 1009, 1187, 879, 1099, 929, 1078, 951,
    1656, 930, 1153, 1030, 1262, 1062, 1214, 1060,
    1621, 930, 1106, 912, 1034, 892, 1158, 990,
    1175, 850, 1121, 903, 1087, 920, 1144, 1056,
    3462, 2240, 4397, 12136, 7758, 1345, 1307, 3278,
    1950, 886, 1023, 1112, 1077, 1042, 1061, 1071,
    1484, 1001, 1096, 915, 1052, 995, 1070, 876,
    1111, 851, 1059, 805, 1112, 923, 1103, 817,
    1899, 1872, 976, 841, 1127, 956, 1159, 950,
    7791, 954, 1289, 933, 1127, 3207, 1020, 927,
    1355, 768, 1040, 745, 952, 805, 1073, 740,
    1013, 805, 1008, 796, 996, 1057, 11457, 13504,
]

# Scrambled string conversion table (network byte -> plain char)
NET_BYTE_TO_STR_CHAR = bytes([
    101, 115, 97, 114, 100, 108, 120, 109, 50, 92,
    105, 47, 103, 3, 113, 107, 117, 68, 82, 52,
    85, 10, 110, 112, 67, 34, 89, 4, 79, 88,
    119, 39, 51, 76, 99, 36, 66, 94, 87, 69,
    20, 41, 84, 70, 30, 42, 48, 102, 57, 72,
    91, 25, 22, 35, 31, 111, 74, 75, 58, 18,
    19, 90, 17, 118, 77, 71, 116, 38, 131, 141,
    60, 175, 124, 5, 8, 26, 12, 133, 7, 96,
    78, 63, 40, 73, 21, 29, 13, 33, 16, 2,
    93, 61, 106, 137, 146, 55, 15, 121, 139, 43,
    53, 104, 9, 6, 62, 83, 135, 59, 98, 65,
    54, 122, 45, 211, 32, 49, 56, 11, 64, 23,
    1, 27, 127, 218, 205, 243, 223, 212, 95, 168,
    245, 255, 188, 176, 180, 239, 199, 192, 216, 231,
    206, 250, 232, 252, 143, 215, 228, 251, 148, 166,
    197, 165, 193, 238, 173, 241, 225, 249, 194, 224,
    81, 242, 219, 244, 142, 248, 222, 200, 174, 233,
    149, 236, 171, 182, 158, 191, 128, 183, 207, 202,
    214, 229, 187, 190, 80, 210, 144, 237, 169, 220,
    151, 126, 28, 203, 86, 132, 178, 125, 217, 253,
    170, 240, 155, 221, 172, 152, 247, 227, 140, 161,
    198, 201, 226, 208, 186, 254, 163, 177, 204, 184,
    213, 195, 145, 179, 37, 159, 160, 189, 136, 246,
    156, 167, 134, 44, 153, 185, 162, 164, 14, 157,
    138, 235, 234, 196, 150, 129, 147, 230, 123, 209,
    130, 24, 154, 181, 0, 46,
])

# ---------------------------------------------------------------------------
# Constants
# ---------------------------------------------------------------------------

CS_SERVERINFO = 0
CS_SYSTEMINFO = 1
CS_PLAYERS = 1684
CS_WEAPONS = 1748  # CS_PLAYERS + MAX_CLIENTS
MAX_CLIENTS = 64
MAX_WEAPONS = 64
MAX_STATS = 32
MAX_ACTIVE_ITEMS = 8
MAX_AMMO = 16
MAX_AMMOCOUNT = 16
GENTITYNUM_BITS = 10
FLOAT_INT_BITS = 13
FLOAT_INT_BIAS = 1 << 12  # 4096
ITEM_WEAPON = 1  # activeItems slot for current weapon

# svc_ops_e
SVC_BAD = 0
SVC_NOP = 1
SVC_GAMESTATE = 2
SVC_CONFIGSTRING = 3
SVC_BASELINE = 4
SVC_SERVERCOMMAND = 5
SVC_DOWNLOAD = 6
SVC_SNAPSHOT = 7
SVC_CENTERPRINT = 8
SVC_LOCPRINT = 9
SVC_CGAMEMESSAGE = 10
SVC_EOF = 11

# ---------------------------------------------------------------------------
# PlayerState field table for protocol 6 (AA)
# Each entry: (name, bits, field_type)
# bits: 0 = float encoding, >0 = unsigned N bits, <0 = signed N bits
# field_type: how the field is encoded on the wire
# ---------------------------------------------------------------------------

FTYPE_REGULAR = 0
FTYPE_ANGLE = 1
FTYPE_COORD = 2
FTYPE_VELOCITY = 3

# Matches playerStateFields_ver_6[] in msg.cpp exactly (57 fields)
PS_FIELDS_VER_6 = [
    ("commandTime", 32, FTYPE_REGULAR),
    ("origin[0]", 0, FTYPE_COORD),
    ("origin[1]", 0, FTYPE_COORD),
    ("viewangles[1]", 0, FTYPE_REGULAR),
    ("velocity[1]", 0, FTYPE_VELOCITY),
    ("velocity[0]", 0, FTYPE_VELOCITY),
    ("viewangles[0]", 0, FTYPE_REGULAR),
    ("pm_time", -16, FTYPE_REGULAR),
    ("origin[2]", 0, FTYPE_COORD),
    ("velocity[2]", 0, FTYPE_VELOCITY),
    ("iViewModelAnimChanged", 2, FTYPE_REGULAR),
    ("damage_angles[0]", -13, FTYPE_ANGLE),
    ("damage_angles[1]", -13, FTYPE_ANGLE),
    ("damage_angles[2]", -13, FTYPE_ANGLE),
    ("speed", 16, FTYPE_REGULAR),
    ("delta_angles[1]", 16, FTYPE_REGULAR),
    ("viewheight", -8, FTYPE_REGULAR),
    ("groundEntityNum", GENTITYNUM_BITS, FTYPE_REGULAR),
    ("delta_angles[0]", 16, FTYPE_REGULAR),
    ("iNetViewModelAnim", 4, FTYPE_REGULAR),
    ("fov", 0, FTYPE_REGULAR),
    ("current_music_mood", 8, FTYPE_REGULAR),
    ("gravity", 16, FTYPE_REGULAR),
    ("fallback_music_mood", 8, FTYPE_REGULAR),
    ("music_volume", 0, FTYPE_REGULAR),
    ("net_pm_flags", 16, FTYPE_REGULAR),
    ("clientNum", 8, FTYPE_REGULAR),
    ("fLeanAngle", 0, FTYPE_REGULAR),
    ("blend[3]", 0, FTYPE_REGULAR),
    ("blend[0]", 0, FTYPE_REGULAR),
    ("pm_type", 8, FTYPE_REGULAR),
    ("feetfalling", 8, FTYPE_REGULAR),
    ("camera_angles[0]", 16, FTYPE_ANGLE),
    ("camera_angles[1]", 16, FTYPE_ANGLE),
    ("camera_angles[2]", 16, FTYPE_ANGLE),
    ("camera_origin[0]", 0, FTYPE_COORD),
    ("camera_origin[1]", 0, FTYPE_COORD),
    ("camera_origin[2]", 0, FTYPE_COORD),
    ("camera_posofs[0]", 0, FTYPE_COORD),
    ("camera_posofs[2]", 0, FTYPE_COORD),
    ("camera_time", 0, FTYPE_REGULAR),
    ("bobCycle", 8, FTYPE_REGULAR),
    ("delta_angles[2]", 16, FTYPE_REGULAR),
    ("viewangles[2]", 0, FTYPE_REGULAR),
    ("music_volume_fade_time", 0, FTYPE_REGULAR),
    ("reverb_type", 6, FTYPE_REGULAR),
    ("reverb_level", 0, FTYPE_REGULAR),
    ("blend[1]", 0, FTYPE_REGULAR),
    ("blend[2]", 0, FTYPE_REGULAR),
    ("camera_offset[0]", 0, FTYPE_REGULAR),
    ("camera_offset[1]", 0, FTYPE_REGULAR),
    ("camera_offset[2]", 0, FTYPE_REGULAR),
    ("camera_posofs[1]", 0, FTYPE_COORD),
    ("camera_flags", 16, FTYPE_REGULAR),
]


# ---------------------------------------------------------------------------
# Bitstream reader with Huffman decoding
# ---------------------------------------------------------------------------

def _build_huffman_tree():
    """Build the static Huffman tree once from the frequency table."""
    tree = HuffTree()
    for i in range(256):
        for _ in range(MSG_HDATA[i]):
            tree.add_ref(i)
    return tree


# Global singleton: the Huffman tree is the same for every demo file
_GLOBAL_HUFF = None


def _get_huffman_tree():
    global _GLOBAL_HUFF
    if _GLOBAL_HUFF is None:
        _GLOBAL_HUFF = _build_huffman_tree()
    return _GLOBAL_HUFF


class MsgReader:
    """Reads Huffman-encoded bitstream data from a demo message buffer."""

    def __init__(self, data):
        self.data = data
        self.bit = 0
        self.size = len(data) * 8
        self.huff = _get_huffman_tree()

    def _get_bit(self):
        if self.bit >= self.size:
            return 0
        byte_idx = self.bit >> 3
        bit_idx = self.bit & 7
        self.bit += 1
        return (self.data[byte_idx] >> bit_idx) & 1

    def _huff_receive(self):
        """Decode one byte using the Huffman tree."""
        node = self.huff.tree
        while node and node.symbol == INTERNAL_NODE:
            if self.bit >= self.size:
                return 0
            if self._get_bit():
                node = node.right
            else:
                node = node.left
        if not node:
            return 0
        return node.symbol

    def read_bits(self, bits):
        """Read N bits from the Huffman-encoded stream."""
        if self.bit + bits > self.size + 64:
            return -1

        value = 0
        nbits = bits & 7
        # Read sub-byte bits as raw bits
        for i in range(nbits):
            value |= self._get_bit() << i
        # Read remaining full bytes via Huffman
        remaining = bits - nbits
        for i in range(0, remaining, 8):
            b = self._huff_receive()
            value |= b << (i + nbits)
        return value

    def read_byte(self):
        return self.read_bits(8) & 0xFF

    def read_short(self):
        v = self.read_bits(16)
        if v & 0x8000:
            v -= 0x10000
        return v

    def read_long(self):
        v = self.read_bits(32)
        if v & 0x80000000:
            v -= 0x100000000
        return v

    def read_float(self):
        """Read a 32-bit IEEE float."""
        v = self.read_bits(32)
        return struct.unpack("<f", struct.pack("<I", v & 0xFFFFFFFF))[0]

    def read_signed_bits(self, bits):
        """Read N bits with sign extension (for negative bit-width fields)."""
        nbits = abs(bits)
        value = self.read_bits(nbits)
        # Sign-extend: if MSB is set, extend to negative
        if value & (1 << (nbits - 1)):
            value |= -1 ^ ((1 << nbits) - 1)
        return value

    def read_data(self, byte_count):
        """Read raw bytes, advancing the bitstream by byte_count*8 bits."""
        result = bytearray()
        for _ in range(byte_count):
            result.append(self.read_byte())
        return bytes(result)

    # -----------------------------------------------------------------------
    # Protocol 6 field type decoders
    # -----------------------------------------------------------------------

    def skip_ps_field_ver6(self, bits, ftype):
        """Read (and discard) a single playerState field value.

        This correctly advances the bitstream past the field encoding
        used by MSG_ReadDeltaPlayerstate for protocol 6.
        """
        if ftype == FTYPE_REGULAR:
            self._read_regular_simple_ver6(bits)
        elif ftype == FTYPE_ANGLE:
            self._read_packed_angle_ver6(bits)
        elif ftype == FTYPE_COORD:
            self._read_packed_coord_ver6()
        elif ftype == FTYPE_VELOCITY:
            self._read_packed_velocity()

    def _read_regular_simple_ver6(self, bits):
        """MSG_ReadRegularSimple_ver_6: used for playerState fields."""
        if bits == 0:
            # Float encoding
            if self.read_bits(1) == 0:
                # Integral float: 13-bit biased integer
                self.read_bits(FLOAT_INT_BITS)
            else:
                # Full 32-bit float
                self.read_bits(32)
        else:
            # Integer: read abs(bits) bits
            self.read_bits(abs(bits))

    def _read_packed_angle_ver6(self, bits):
        """MSG_ReadPackedAngle_ver_6."""
        if bits < 0:
            # Sign bit + magnitude
            self.read_bits(1)  # sign
            self.read_bits(~bits)  # magnitude (bitwise NOT of negative = positive)
        else:
            self.read_bits(bits)

    def _read_packed_coord_ver6(self):
        """MSG_ReadPackedCoord_ver_6: always 19 bits."""
        self.read_bits(19)

    def _read_packed_velocity(self):
        """MSG_ReadPackedVelocity: always 17 bits."""
        self.read_bits(17)

    def read_string(self):
        """Read a null-terminated string (no descrambling)."""
        chars = []
        while True:
            c = self.read_byte()
            if c == 0xFF or c == 0:  # -1 or null
                break
            if c == ord("%"):
                c = ord(".")
            chars.append(c)
            if len(chars) >= 2048:
                break
        return bytes(chars).decode("latin-1", errors="replace")

    def read_scrambled_string(self):
        """Read a null-terminated string with protocol 15+ descrambling."""
        chars = []
        while True:
            c = self.read_byte()
            if c == 0xFF:  # -1
                break
            c = NET_BYTE_TO_STR_CHAR[c]
            if c == 0:
                break
            if c == ord("%"):
                c = ord(".")
            chars.append(c)
            if len(chars) >= 2048:
                break
        return bytes(chars).decode("latin-1", errors="replace")

    def past_end(self):
        return (self.bit >> 3) > len(self.data)


# ---------------------------------------------------------------------------
# Info string parser
# ---------------------------------------------------------------------------

def parse_info_string(s):
    """Parse a Q3-style info string (\\key\\value\\key\\value...) into a dict."""
    result = {}
    if not s or s[0] != "\\":
        return result
    parts = s[1:].split("\\")
    for i in range(0, len(parts) - 1, 2):
        result[parts[i].lower()] = parts[i + 1]
    return result


# ---------------------------------------------------------------------------
# Configstring index normalization (protocol 6 wire -> actual)
# ---------------------------------------------------------------------------

def normalize_cs_ver6(num):
    if 6 <= num <= 25:
        return num - 2
    return num


def normalize_cs_ver15(num):
    return num


# ---------------------------------------------------------------------------
# Demo scanner
# ---------------------------------------------------------------------------

class DemoScanner:
    """Scans a single .dm3 file and extracts player names and loadout info."""

    def __init__(self, filepath):
        self.filepath = filepath
        self.players = {}  # slot -> name
        self.protocol = 0
        self.scrambled = False
        self.error = None
        self.configstrings = {}  # cs_idx -> value
        # Snapshot-derived data (AA protocol 6/8 only)
        self.weapons_seen = []  # list of weapon configstring names seen
        self.grenade_count = None  # grenade count from first spawn snapshot
        self.recording_client = None  # clientNum of the recording player
        # Internal state for delta decoding across snapshots
        self._prev_ps_arrays = None  # previous playerState arrays for delta

    def scan(self):
        try:
            with open(self.filepath, "rb") as f:
                data = f.read()
        except (IOError, OSError) as e:
            self.error = str(e)
            return

        pos = 0
        msg_count = 0

        while pos + 8 <= len(data):
            # Read sequence number
            seq = struct.unpack_from("<i", data, pos)[0]
            pos += 4

            # Read message length
            msg_len = struct.unpack_from("<I", data, pos)[0]
            pos += 4

            if msg_len == 0xFFFFFFFF:
                break  # end of demo

            if msg_len > 49152 or pos + msg_len > len(data):
                break  # invalid or truncated

            msg_data = data[pos : pos + msg_len]
            pos += msg_len

            try:
                if msg_count == 0:
                    self._parse_gamestate(msg_data)
                else:
                    self._parse_message(msg_data)
            except Exception:
                pass  # Skip malformed messages

            msg_count += 1

    def _read_string(self, reader):
        if self.scrambled:
            return reader.read_scrambled_string()
        return reader.read_string()

    def _parse_gamestate(self, data):
        reader = MsgReader(data)

        # reliable sequence ack
        reader.read_long()

        cmd = reader.read_byte()
        if cmd != SVC_GAMESTATE:
            self.error = "First message is not gamestate (got %d)" % cmd
            return

        # server command sequence
        reader.read_long()

        # First pass: try without scrambling (protocol 6/8)
        # We'll detect protocol from the configstrings
        self.scrambled = False
        configstrings = {}

        while not reader.past_end():
            cmd = reader.read_byte()
            if cmd == SVC_EOF:
                break
            if cmd == SVC_CONFIGSTRING:
                idx = reader.read_short() & 0xFFFF
                value = self._read_string(reader)
                cs_idx = normalize_cs_ver6(idx)
                configstrings[cs_idx] = value
            elif cmd == SVC_BASELINE:
                # Can't parse baselines without full entity delta code,
                # but all configstrings come before baselines in the gamestate
                break
            else:
                break

        # Check if we got valid configstrings
        # CS_SERVERINFO should be an info string starting with backslash
        serverinfo = configstrings.get(CS_SERVERINFO, "")
        if serverinfo and serverinfo[0] == "\\":
            # Looks valid, check protocol
            info = parse_info_string(serverinfo)
            self.protocol = int(info.get("protocol", "8"))
            if self.protocol >= 15:
                self.scrambled = True
                # Need to re-parse with scrambling
                self._reparse_gamestate_scrambled(data)
                return
        else:
            # Try scrambled (protocol 15+)
            self.scrambled = True
            self._reparse_gamestate_scrambled(data)
            return

        # Store configstrings for weapon/ammo lookups
        self.configstrings = configstrings

        # Extract player names from configstrings
        self._extract_players(configstrings)

    def _reparse_gamestate_scrambled(self, data):
        reader = MsgReader(data)
        reader.read_long()  # reliable ack
        reader.read_byte()  # svc_gamestate
        reader.read_long()  # server cmd seq

        self.scrambled = True
        configstrings = {}

        while not reader.past_end():
            cmd = reader.read_byte()
            if cmd == SVC_EOF:
                break
            if cmd == SVC_CONFIGSTRING:
                idx = reader.read_short() & 0xFFFF
                value = self._read_string(reader)
                cs_idx = normalize_cs_ver15(idx)
                configstrings[cs_idx] = value
            elif cmd == SVC_BASELINE:
                break
            else:
                break

        serverinfo = configstrings.get(CS_SERVERINFO, "")
        if serverinfo and serverinfo[0] == "\\":
            info = parse_info_string(serverinfo)
            self.protocol = int(info.get("protocol", "17"))
        else:
            self.error = "Could not parse gamestate"
            return

        self.configstrings = configstrings
        self._extract_players(configstrings)

    def _extract_players(self, configstrings):
        for cs_idx, value in configstrings.items():
            if CS_PLAYERS <= cs_idx < CS_PLAYERS + MAX_CLIENTS:
                slot = cs_idx - CS_PLAYERS
                name = self._extract_name(value)
                if name:
                    self.players[slot] = name

    def _extract_name(self, cs_value):
        """Extract player name from a player configstring like 'name\\Player\\team\\1'."""
        if not cs_value:
            return None
        # Try info-string format first
        if "\\" in cs_value:
            info = parse_info_string("\\" + cs_value if not cs_value.startswith("\\") else cs_value)
            name = info.get("name", "")
            if name:
                return name
        # If no backslash, the whole value might be the name (some mods)
        return cs_value.strip() if cs_value.strip() else None

    def _parse_message(self, data):
        """Parse a non-gamestate message looking for serverCommand cs updates and snapshots."""
        reader = MsgReader(data)

        # reliable sequence ack
        reader.read_long()

        while not reader.past_end():
            cmd = reader.read_byte()
            if cmd == SVC_EOF:
                break
            if cmd == SVC_NOP:
                continue
            if cmd == SVC_SERVERCOMMAND:
                reader.read_long()  # sequence
                cmd_str = self._read_string(reader)
                self._handle_server_command(cmd_str)
            elif cmd == SVC_SNAPSHOT and self.protocol < 15:
                try:
                    self._parse_snapshot_ver6(reader)
                except Exception:
                    pass
                # After snapshot, remaining data may be cgame messages
                # which we can't parse, so stop
                break
            elif cmd == SVC_CENTERPRINT:
                # Read and discard the string
                self._read_string(reader)
                continue
            elif cmd == SVC_LOCPRINT:
                # Read localized print: short + string
                reader.read_short()
                self._read_string(reader)
                continue
            elif cmd == SVC_GAMESTATE:
                break
            else:
                # Can't parse cgame messages, etc.
                break

    def _handle_server_command(self, cmd_str):
        """Handle a server command string, looking for 'cs' configstring updates."""
        if not cmd_str:
            return
        parts = cmd_str.split(None, 2)  # split into max 3 parts
        if len(parts) < 3:
            return
        if parts[0] != "cs":
            return

        try:
            raw_idx = int(parts[1])
        except ValueError:
            return

        value = parts[2]

        # Normalize
        if self.protocol < 15:
            cs_idx = normalize_cs_ver6(raw_idx)
        else:
            cs_idx = normalize_cs_ver15(raw_idx)

        # Update stored configstrings
        self.configstrings[cs_idx] = value

        if CS_PLAYERS <= cs_idx < CS_PLAYERS + MAX_CLIENTS:
            slot = cs_idx - CS_PLAYERS
            name = self._extract_name(value)
            if name:
                self.players[slot] = name
            elif slot in self.players and not value.strip():
                # Player disconnected (empty configstring)
                pass  # Keep them in the list - they were in the game


    # -------------------------------------------------------------------
    # Snapshot parsing (protocol 6 / AA only)
    # -------------------------------------------------------------------

    def _parse_snapshot_ver6(self, reader):
        """Parse a protocol 6 svc_snapshot, extracting playerState arrays."""
        server_time = reader.read_long()
        server_time_residual = reader.read_byte()
        delta_num = reader.read_byte()
        snap_flags = reader.read_byte()

        # Read areamask
        areamask_len = reader.read_byte()
        reader.read_data(areamask_len)

        # Parse playerState
        ps_arrays = self._parse_playerstate_ver6(reader, delta_num == 0)

        if ps_arrays is not None:
            self._process_ps_arrays(ps_arrays)
            self._prev_ps_arrays = ps_arrays

        # Don't parse entities or sounds - we have what we need

    def _parse_playerstate_ver6(self, reader, is_full):
        """Parse playerState delta for protocol 6.

        Returns dict with arrays (activeItems, ammo_name_index, ammo_amount,
        max_ammo_amount) or None on failure.
        """
        fields = PS_FIELDS_VER_6
        num_fields = len(fields)

        # Read last-changed field index
        lc = reader.read_byte()
        if lc > num_fields:
            return None

        # Skip through field-by-field delta section
        for i in range(lc):
            name, bits, ftype = fields[i]
            if reader.read_bits(1):
                # Field changed - decode to advance bitstream
                reader.skip_ps_field_ver6(bits, ftype)

        # Now read the arrays section
        if not reader.read_bits(1):
            # No arrays changed
            if self._prev_ps_arrays is not None:
                return dict(self._prev_ps_arrays)
            return {
                "activeItems": [0] * MAX_ACTIVE_ITEMS,
                "ammo_name_index": [0] * MAX_AMMO,
                "ammo_amount": [0] * MAX_AMMOCOUNT,
                "max_ammo_amount": [0] * MAX_AMMOCOUNT,
            }

        # Start from previous state or zeros
        if self._prev_ps_arrays is not None and not is_full:
            arrays = {k: list(v) for k, v in self._prev_ps_arrays.items()}
        else:
            arrays = {
                "activeItems": [0] * MAX_ACTIVE_ITEMS,
                "ammo_name_index": [0] * MAX_AMMO,
                "ammo_amount": [0] * MAX_AMMOCOUNT,
                "max_ammo_amount": [0] * MAX_AMMOCOUNT,
            }

        # Parse stats (we skip these but must advance the bitstream)
        if reader.read_bits(1):
            stat_bits = reader.read_bits(32)
            for i in range(MAX_STATS):
                if stat_bits & (1 << i):
                    reader.read_short()

        # Parse activeItems
        if reader.read_bits(1):
            item_bits = reader.read_byte()
            for i in range(MAX_ACTIVE_ITEMS):
                if item_bits & (1 << i):
                    arrays["activeItems"][i] = reader.read_short()

        # Parse ammo_amount
        if reader.read_bits(1):
            ammo_bits = reader.read_short()
            for i in range(MAX_AMMOCOUNT):
                if ammo_bits & (1 << i):
                    arrays["ammo_amount"][i] = reader.read_short()

        # Parse ammo_name_index
        if reader.read_bits(1):
            ammo_bits = reader.read_short()
            for i in range(MAX_AMMO):
                if ammo_bits & (1 << i):
                    arrays["ammo_name_index"][i] = reader.read_short()

        # Parse max_ammo_amount
        if reader.read_bits(1):
            ammo_bits = reader.read_short()
            for i in range(MAX_AMMOCOUNT):
                if ammo_bits & (1 << i):
                    arrays["max_ammo_amount"][i] = reader.read_short()

        return arrays

    def _process_ps_arrays(self, arrays):
        """Extract weapon and ammo info from playerState arrays."""
        # Extract current weapon
        weapon_idx = arrays["activeItems"][ITEM_WEAPON]
        if weapon_idx > 0 and weapon_idx < MAX_WEAPONS:
            cs_idx = CS_WEAPONS + weapon_idx
            weapon_name = self._strip_quotes(self.configstrings.get(cs_idx, ""))
            if weapon_name and weapon_name not in self.weapons_seen:
                self.weapons_seen.append(weapon_name)

        # Extract grenade count - track max across all snapshots
        if weapon_idx > 0:
            self._extract_grenade_count(arrays)

    def _extract_grenade_count(self, arrays):
        """Find the grenade ammo slot and track the maximum count seen.

        The ammo_name_index values are relative offsets within CS_WEAPONS
        (from SV_FindIndex), so we look up configstrings[CS_WEAPONS + value].
        We track the maximum grenade count seen across all snapshots, since
        the count at spawn is the highest it will be (before any are thrown).
        """
        for i in range(MAX_AMMO):
            name_idx = arrays["ammo_name_index"][i]
            if name_idx <= 0:
                continue
            # ammo_name_index stores relative offset within CS_WEAPONS
            cs_idx = CS_WEAPONS + name_idx
            ammo_name = self._strip_quotes(self.configstrings.get(cs_idx, ""))
            if ammo_name.lower() in ("grenade", "agrenade"):
                count = arrays["ammo_amount"][i]
                if count > 0:
                    if self.grenade_count is None:
                        self.grenade_count = count
                    else:
                        self.grenade_count = max(self.grenade_count, count)

    @staticmethod
    def _strip_quotes(s):
        """Strip surrounding double quotes from a string."""
        if s and len(s) >= 2 and s[0] == '"' and s[-1] == '"':
            return s[1:-1]
        return s

    @staticmethod
    def _is_grenade_weapon(name):
        """Check if a weapon name refers to a grenade (not a primary weapon)."""
        lower = name.lower()
        # Match common grenade weapon names from MoHAA
        grenade_names = (
            "frag grenade", "stielhandgranate", "grenade",
            "nebelhandgranate", "smoke grenade",
            "m18 smoke grenade", "m2 frag grenade",
            "mills bomb", "no 36m mk1",
        )
        for gn in grenade_names:
            if gn in lower:
                return True
        # Also catch generic patterns
        if "grenade" in lower or "granate" in lower:
            return True
        return False


# ---------------------------------------------------------------------------
# Main
# ---------------------------------------------------------------------------

def find_demos(folder):
    """Recursively find all .dm3 files in a folder."""
    demos = []
    for root, _dirs, files in os.walk(folder):
        for f in files:
            if f.lower().endswith(".dm3"):
                demos.append(os.path.join(root, f))
    demos.sort()
    return demos


def main():
    parser = argparse.ArgumentParser(
        description="Scan MoHAA .dm3 demo files for player names."
    )
    parser.add_argument("folder", help="Folder containing .dm3 files (scanned recursively)")
    parser.add_argument(
        "-o", "--output",
        default="demoscan_results.txt",
        help="Output text file (default: demoscan_results.txt)",
    )
    parser.add_argument(
        "-p", "--player",
        action="append",
        dest="players",
        help="Filter: only show demos where this player appears (case-insensitive, can specify multiple times)",
    )
    parser.add_argument(
        "-v", "--verbose",
        action="store_true",
        help="Show extra debug info (CS_WEAPONS configstrings, raw ammo data)",
    )
    args = parser.parse_args()

    if not os.path.isdir(args.folder):
        print("Error: '%s' is not a directory" % args.folder, file=sys.stderr)
        sys.exit(1)

    demos = find_demos(args.folder)
    if not demos:
        print("No .dm3 files found in '%s'" % args.folder)
        sys.exit(0)

    print("Found %d demo file(s), scanning..." % len(demos))

    filter_names = None
    if args.players:
        filter_names = [n.lower() for n in args.players]

    results = []
    errors = 0

    for i, demo_path in enumerate(demos):
        rel_path = os.path.relpath(demo_path, args.folder)
        if (i + 1) % 50 == 0 or i == 0:
            print("  [%d/%d] %s" % (i + 1, len(demos), rel_path))

        scanner = DemoScanner(demo_path)
        scanner.scan()

        if scanner.error:
            errors += 1
            results.append((rel_path, None, scanner.error, [], None, scanner))
            continue

        player_names = sorted(set(scanner.players.values()))

        if filter_names:
            # Check if any filtered player appears
            lower_names = [n.lower() for n in player_names]
            if not any(fn in lower_names for fn in filter_names):
                continue

        # Collect weapon/grenade info, filtering out grenade items
        weapons = [
            DemoScanner._strip_quotes(w)
            for w in scanner.weapons_seen
            if not DemoScanner._is_grenade_weapon(w)
        ]
        # Deduplicate while preserving order
        seen = set()
        unique_weapons = []
        for w in weapons:
            if w not in seen:
                seen.add(w)
                unique_weapons.append(w)
        weapons = unique_weapons
        grenade_count = scanner.grenade_count

        results.append((rel_path, player_names, None, weapons, grenade_count, scanner))

    # Write output
    with open(args.output, "w", encoding="utf-8") as f:
        if filter_names:
            f.write("# Filter: %s\n" % ", ".join(args.players))
            f.write("#\n")

        matches = 0
        for rel_path, players, error, weapons, grenade_count, scanner in results:
            if error:
                f.write("%s\n  ERROR: %s\n\n" % (rel_path, error))
                continue
            if players is None:
                continue
            matches += 1
            f.write("%s\n" % rel_path)
            if players:
                for name in players:
                    f.write("  %s\n" % name)
            else:
                f.write("  (no players found)\n")
            if weapons:
                f.write("  Weapon(s): %s\n" % ", ".join(weapons))
            if grenade_count is not None:
                mode = "realism" if grenade_count <= 3 else "default"
                f.write("  Grenades: %d (%s)\n" % (grenade_count, mode))
            if args.verbose and scanner:
                # Dump CS_WEAPONS configstrings
                weapon_cs = {}
                for cs_idx in range(CS_WEAPONS, CS_WEAPONS + MAX_WEAPONS):
                    val = scanner.configstrings.get(cs_idx, "")
                    if val:
                        weapon_cs[cs_idx] = val
                if weapon_cs:
                    f.write("  [verbose] CS_WEAPONS configstrings:\n")
                    for cs_idx in sorted(weapon_cs):
                        f.write("    [%d] %s\n" % (cs_idx, weapon_cs[cs_idx]))
                # Dump last known ammo arrays
                if scanner._prev_ps_arrays:
                    pa = scanner._prev_ps_arrays
                    f.write("  [verbose] ammo_name_index: %s\n" % pa["ammo_name_index"])
                    f.write("  [verbose] ammo_amount: %s\n" % pa["ammo_amount"])
                    f.write("  [verbose] activeItems: %s\n" % pa["activeItems"])
                    # Resolve ammo names (ammo_name_index stores relative
                    # offsets within CS_WEAPONS)
                    for i in range(MAX_AMMO):
                        ni = pa["ammo_name_index"][i]
                        if ni > 0:
                            cs_idx = CS_WEAPONS + ni
                            name = scanner.configstrings.get(cs_idx, "(unknown cs %d)" % cs_idx)
                            f.write("    ammo[%d]: idx=%d -> cs[%d]='%s', amount=%d\n"
                                    % (i, ni, cs_idx, name, pa["ammo_amount"][i]))
                else:
                    f.write("  [verbose] No snapshot arrays parsed\n")
            f.write("\n")

        f.write("# ---\n")
        f.write("# Total demos scanned: %d\n" % len(demos))
        if filter_names:
            f.write("# Demos matching filter: %d\n" % matches)
        f.write("# Errors: %d\n" % errors)

    print("\nDone! %d demos scanned, %d errors." % (len(demos), errors))
    if filter_names:
        matching = sum(1 for _, p, e in results if p is not None and e is None)
        print("Demos matching filter: %d" % matching)
    print("Results written to: %s" % args.output)


if __name__ == "__main__":
    main()
