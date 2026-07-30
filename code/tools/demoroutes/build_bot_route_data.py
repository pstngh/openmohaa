#!/usr/bin/env python3
"""Build compact runtime bot-route graphs from indexed human movement."""

from __future__ import annotations

import argparse
import csv
import io
import json
import math
import sqlite3
import zlib
import zipfile
from collections import Counter
from contextlib import contextmanager
from dataclasses import dataclass, field
from pathlib import Path
from typing import Any, Iterator, TextIO


OBJECTIVE_MAP_TEAMS = {
    "obj/obj_team2": {"attacker": 3, "defender": 4},
    "obj/obj_team4": {"attacker": 4, "defender": 3},
}
OBJECTIVE_PHASES = ("preplant", "postplant")
PRACTICE_FREE_FOR_ALL_MAPS = (
    "dm/crnodoors",
    "dm/downladder",
    "dm/main",
    "dm/vents",
)
FREE_FOR_ALL_MAPS = ("dm/mohdm6", *PRACTICE_FREE_FOR_ALL_MAPS)
CPP_ROUTE_MODES = {
    "objective": "BOT_DEMO_ROUTE_OBJECTIVE",
    "free_for_all": "BOT_DEMO_ROUTE_FREE_FOR_ALL",
}


@dataclass
class NodeStats:
    position_sum: list[float] = field(
        default_factory=lambda: [0.0, 0.0, 0.0]
    )
    visits: int = 0
    geometry_ends: int = 0
    normal_ends: int = 0
    smg_ends: int = 0


@dataclass
class EdgeStats:
    geometry_routes: int = 0
    normal_routes: int = 0
    smg_routes: int = 0


@dataclass
class GraphStats:
    map_name: str
    mode: str
    role: str
    team: int
    phase: str
    nodes: dict[tuple[int, int, int], NodeStats] = field(
        default_factory=dict
    )
    edges: dict[
        tuple[tuple[int, int, int], tuple[int, int, int]], EdgeStats
    ] = field(default_factory=dict)
    geometry_routes: int = 0
    normal_routes: int = 0
    smg_routes: int = 0
    demo_routes: int = 0
    telemetry_routes: int = 0

    def add(
        self,
        collapsed: list[
            tuple[tuple[int, int, int], tuple[float, float, float]]
        ],
        normal_behavior: bool,
        smg_behavior: bool,
        source: str = "demo",
    ) -> None:
        if len(collapsed) < 2:
            return

        self.geometry_routes += 1
        self.normal_routes += int(normal_behavior)
        self.smg_routes += int(smg_behavior)
        if source == "demo":
            self.demo_routes += 1
        elif source == "telemetry":
            self.telemetry_routes += 1
        else:
            raise ValueError(f"unsupported route source: {source!r}")

        for key, position in collapsed:
            node = self.nodes.setdefault(key, NodeStats())
            for axis in range(3):
                node.position_sum[axis] += position[axis]
            node.visits += 1

        endpoint = self.nodes[collapsed[-1][0]]
        endpoint.geometry_ends += 1
        endpoint.normal_ends += int(normal_behavior)
        endpoint.smg_ends += int(smg_behavior)

        seen_edges: set[
            tuple[tuple[int, int, int], tuple[int, int, int]]
        ] = set()
        for first, second in zip(collapsed, collapsed[1:]):
            edge_key = (first[0], second[0])
            if edge_key[0] == edge_key[1] or edge_key in seen_edges:
                continue
            seen_edges.add(edge_key)
            edge = self.edges.setdefault(edge_key, EdgeStats())
            edge.geometry_routes += 1
            edge.normal_routes += int(normal_behavior)
            edge.smg_routes += int(smg_behavior)


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("database", type=Path)
    parser.add_argument("output", type=Path)
    parser.add_argument("--summary", type=Path)
    parser.add_argument(
        "--telemetry",
        action="append",
        default=[],
        type=Path,
        help=(
            "movement telemetry directory or ZIP; may be repeated. "
            "FFA sessions affect behavior weights while team-match sessions "
            "contribute geometry only."
        ),
    )
    parser.add_argument("--xy-cell", type=float, default=512.0)
    parser.add_argument("--z-cell", type=float, default=192.0)
    return parser.parse_args()


@dataclass
class TelemetryLife:
    map_name: str
    game_type: int
    points: list[list[Any]] = field(default_factory=list)
    weapon_counts: Counter[str] = field(default_factory=Counter)


@contextmanager
def open_telemetry_member(
    source: Path, member: str
) -> Iterator[TextIO]:
    if source.is_dir():
        path = source / member
        with path.open(encoding="utf-8-sig", newline="") as stream:
            yield stream
        return

    if not source.is_file():
        raise FileNotFoundError(f"telemetry source not found: {source}")

    with zipfile.ZipFile(source) as archive:
        try:
            raw = archive.open(member)
        except KeyError as error:
            raise ValueError(
                f"{source} does not contain {member}"
            ) from error
        with raw:
            with io.TextIOWrapper(
                raw, encoding="utf-8-sig", newline=""
            ) as stream:
                yield stream


def telemetry_sessions(source: Path) -> dict[str, tuple[str, int]]:
    sessions: dict[str, tuple[str, int]] = {}
    section: dict[str, str] = {}

    def store_section() -> None:
        session_id = section.get("session_id")
        map_name = section.get("map")
        game_type = section.get("g_gametype")
        if not session_id or not map_name or game_type is None:
            return
        try:
            sessions[session_id] = (map_name, int(game_type))
        except ValueError:
            return

    with open_telemetry_member(source, "movement_meta.txt") as stream:
        for raw_line in stream:
            line = raw_line.strip()
            if line.startswith("[session ") and line.endswith("]"):
                store_section()
                section = {
                    "session_id": line[len("[session ") : -1]
                }
            elif "=" in line:
                key, value = line.split("=", 1)
                section[key.strip()] = value.strip()
    store_section()
    return sessions


def dominant_weapon_is_smg(weapon_counts: Counter[str]) -> bool:
    total = sum(weapon_counts.values())
    if not total:
        return False
    smg_frames = sum(
        count
        for weapon, count in weapon_counts.items()
        if any(
            marker in weapon
            for marker in ("mp40", "thompson", "sten")
        )
    )
    return smg_frames * 2 >= total


def add_telemetry_routes(
    graphs: dict[tuple[str, str, str, int, str], GraphStats],
    sources: list[Path],
    xy_cell: float,
    z_cell: float,
) -> None:
    for source in sources:
        sessions = telemetry_sessions(source)
        active: dict[tuple[str, str], TelemetryLife] = {}
        current_session = ""

        def flush(key: tuple[str, str]) -> None:
            life = active.pop(key, None)
            if not life:
                return
            graph = graphs.get(
                (
                    life.map_name,
                    "free_for_all",
                    "roamer",
                    0,
                    "roam",
                )
            )
            if not graph:
                return
            normal_behavior = life.game_type == 1
            graph.add(
                collapse_route(life.points, xy_cell, z_cell),
                normal_behavior,
                normal_behavior
                and dominant_weapon_is_smg(life.weapon_counts),
                source="telemetry",
            )

        with open_telemetry_member(
            source, "movement_frames.csv"
        ) as stream:
            for row in csv.DictReader(stream):
                session_id = row.get("session_id", "")
                session = sessions.get(session_id)
                if (
                    not session
                    or session[0] not in PRACTICE_FREE_FOR_ALL_MAPS
                    or session[1] not in (1, 2)
                ):
                    continue

                if current_session and current_session != session_id:
                    for key in list(active):
                        flush(key)
                current_session = session_id

                client_id = row.get("client_id", "")
                key = (session_id, client_id)
                if (
                    row.get("is_bot") != "0"
                    or row.get("alive") != "1"
                    or row.get("spectator") != "0"
                ):
                    flush(key)
                    continue

                try:
                    point = [
                        int(row.get("session_ms") or 0),
                        float(row["origin_x"]),
                        float(row["origin_y"]),
                        float(row["origin_z"]),
                    ]
                except (KeyError, TypeError, ValueError):
                    flush(key)
                    continue

                life = active.get(key)
                if life and life.points:
                    previous = life.points[-1]
                    time_gap = point[0] - previous[0]
                    distance_squared = sum(
                        (point[axis] - previous[axis]) ** 2
                        for axis in range(1, 4)
                    )
                    if (
                        time_gap < 0
                        or time_gap > 1000
                        or distance_squared > 1024.0**2
                    ):
                        flush(key)
                        life = None

                if not life:
                    life = TelemetryLife(session[0], session[1])
                    active[key] = life
                life.points.append(point)
                weapon = row.get("weapon", "").strip().lower()
                if weapon:
                    life.weapon_counts[weapon] += 1

        for key in list(active):
            flush(key)


def decode_points(blob: bytes, codec: str) -> list[list[Any]]:
    if codec != "zlib-json-v1":
        raise ValueError(f"unsupported point codec: {codec!r}")
    points = json.loads(zlib.decompress(blob))
    if not isinstance(points, list):
        raise ValueError("route points are not a list")
    return points


def collapse_route(
    points: list[list[Any]], xy_cell: float, z_cell: float
) -> list[tuple[tuple[int, int, int], tuple[float, float, float]]]:
    collapsed: list[
        tuple[tuple[int, int, int], tuple[float, float, float]]
    ] = []
    sample_counts: list[int] = []

    for point in points:
        if len(point) < 4:
            continue
        try:
            position = (
                float(point[1]),
                float(point[2]),
                float(point[3]),
            )
        except (TypeError, ValueError):
            continue
        if not all(math.isfinite(value) for value in position):
            continue

        key = (
            math.floor(position[0] / xy_cell),
            math.floor(position[1] / xy_cell),
            math.floor(position[2] / z_cell),
        )
        if collapsed and collapsed[-1][0] == key:
            previous = collapsed[-1][1]
            old_count = sample_counts[-1]
            count = old_count + 1
            collapsed[-1] = (
                key,
                tuple(
                    (previous[axis] * old_count + position[axis]) / count
                    for axis in range(3)
                ),
            )
            sample_counts[-1] = count
            continue

        # Demo jitter can produce A-B-A cell oscillation at a boundary.
        if len(collapsed) >= 2 and collapsed[-2][0] == key:
            collapsed.pop()
            sample_counts.pop()
            previous = collapsed[-1][1]
            old_count = sample_counts[-1]
            count = old_count + 1
            collapsed[-1] = (
                key,
                tuple(
                    (previous[axis] * old_count + position[axis]) / count
                    for axis in range(3)
                ),
            )
            sample_counts[-1] = count
            continue

        collapsed.append((key, position))
        sample_counts.append(1)

    return collapsed


def route_phases(
    points: list[list[Any]], plant_time: int | None
) -> dict[str, list[list[Any]]]:
    if plant_time is None:
        return {"preplant": points, "postplant": []}

    preplant: list[list[Any]] = []
    postplant: list[list[Any]] = []
    for point in points:
        if not point:
            continue
        try:
            server_time = int(point[0])
        except (TypeError, ValueError):
            continue
        if server_time < plant_time:
            preplant.append(point)
        else:
            postplant.append(point)
    return {"preplant": preplant, "postplant": postplant}


def build_graphs(
    database: Path,
    xy_cell: float,
    z_cell: float,
    telemetry_sources: list[Path] | None = None,
) -> list[GraphStats]:
    connection = sqlite3.connect(
        f"file:{database.resolve()}?mode=ro", uri=True
    )
    connection.row_factory = sqlite3.Row
    try:
        schema = connection.execute(
            "SELECT value FROM meta WHERE key = 'schema_version'"
        ).fetchone()
        if not schema or schema[0] != "3":
            raise RuntimeError("demo index schema 3 is required")

        plant_times = {
            row["round_id"]: row["first_plant_time"]
            for row in connection.execute(
                """
                SELECT round_id, first_plant_time
                FROM rounds
                WHERE first_plant_time IS NOT NULL
                """
            )
        }

        graphs: dict[tuple[str, str, str, int, str], GraphStats] = {
            (map_name, "objective", role, team, phase): GraphStats(
                map_name, "objective", role, team, phase
            )
            for map_name, roles in OBJECTIVE_MAP_TEAMS.items()
            for role, team in roles.items()
            for phase in OBJECTIVE_PHASES
        }
        graphs.update(
            {
                (map_name, "free_for_all", "roamer", 0, "roam"): GraphStats(
                    map_name, "free_for_all", "roamer", 0, "roam"
                )
                for map_name in FREE_FOR_ALL_MAPS
            }
        )

        rows = connection.execute(
            """
            SELECT
                r.map_name,
                r.mode,
                r.role,
                r.team,
                r.round_id,
                r.route_scope,
                r.analysis_usage,
                r.weapon_category,
                r.points_codec,
                r.points_blob,
                d.status AS demo_status
            FROM routes AS r
            JOIN demos AS d ON d.demo_id = r.demo_id
            WHERE r.eligible_route = 1
              AND (
                    (r.mode = 'objective'
                     AND r.map_name IN ('obj/obj_team2', 'obj/obj_team4'))
                 OR (r.mode = 'free_for_all'
                     AND r.map_name IN (
                         'dm/mohdm6',
                         'dm/crnodoors',
                         'dm/downladder',
                         'dm/main',
                         'dm/vents'
                     ))
                 OR (r.mode = 'team_deathmatch'
                     AND r.map_name IN (
                         'dm/crnodoors',
                         'dm/downladder',
                         'dm/main',
                         'dm/vents'
                     ))
              )
            ORDER BY r.route_id
            """
        )

        for row in rows:
            normal_behavior = (
                row["mode"] in ("objective", "free_for_all")
                and row["demo_status"] == "complete"
                and row["route_scope"] == "recorder_life"
                and row["analysis_usage"] == "behavior_candidate"
            )
            smg_behavior = (
                normal_behavior and row["weapon_category"] == "smg"
            )
            points = decode_points(
                row["points_blob"], row["points_codec"]
            )

            if row["mode"] == "objective":
                key_prefix = (
                    row["map_name"],
                    "objective",
                    row["role"],
                    row["team"],
                )
                if key_prefix + ("preplant",) not in graphs:
                    continue

                phases = route_phases(
                    points, plant_times.get(row["round_id"])
                )
                for phase in OBJECTIVE_PHASES:
                    collapsed = collapse_route(
                        phases[phase], xy_cell, z_cell
                    )
                    graphs[key_prefix + (phase,)].add(
                        collapsed, normal_behavior, smg_behavior
                    )
            else:
                graph = graphs[
                    (row["map_name"], "free_for_all", "roamer", 0, "roam")
                ]
                graph.add(
                    collapse_route(points, xy_cell, z_cell),
                    normal_behavior,
                    smg_behavior,
                )
    finally:
        connection.close()

    add_telemetry_routes(
        graphs, telemetry_sources or [], xy_cell, z_cell
    )

    return [
        graphs[key]
        for key in sorted(graphs)
        if graphs[key].nodes and graphs[key].edges
    ]


def cpp_name(graph: GraphStats) -> str:
    map_token = "".join(
        part.title() for part in graph.map_name.replace("/", "_").split("_")
    )
    return f"k{map_token}{graph.role.title()}{graph.phase.title()}"


def mean_position(node: NodeStats) -> tuple[float, float, float]:
    return tuple(value / node.visits for value in node.position_sum)


def render_cpp(
    graphs: list[GraphStats], xy_cell: float, z_cell: float
) -> str:
    lines = [
        "/*",
        "===========================================================================",
        "Generated by code/tools/demoroutes/build_bot_route_data.py.",
        "Do not edit by hand.",
        f"Grid: {xy_cell:g} XY units, {z_cell:g} Z units.",
        "Geometry uses eligible demos and human telemetry. FFA behavior",
        "weights use complete normal-mode FFA lives; team-match practice-map",
        "movement contributes connectivity only.",
        "===========================================================================",
        "*/",
        "",
        '#include "playerbot_route.h"',
        "",
        "namespace {",
        "",
    ]

    graph_rows: list[str] = []
    for graph in graphs:
        name = cpp_name(graph)
        node_keys = sorted(graph.nodes)
        node_indexes = {
            key: index for index, key in enumerate(node_keys)
        }

        lines.append(
            f"static const bot_demo_route_node_t {name}Nodes[] = {{"
        )
        for key in node_keys:
            node = graph.nodes[key]
            x, y, z = mean_position(node)
            lines.append(
                "    {"
                f"{x:.3f}f, {y:.3f}f, {z:.3f}f, "
                f"{node.geometry_ends}u, {node.normal_ends}u, "
                f"{node.smg_ends}u"
                "},"
            )
        lines.extend(["};", ""])

        lines.append(
            f"static const bot_demo_route_edge_t {name}Edges[] = {{"
        )
        for (first, second), edge in sorted(graph.edges.items()):
            lines.append(
                "    {"
                f"{node_indexes[first]}u, {node_indexes[second]}u, "
                f"{edge.geometry_routes}u, {edge.normal_routes}u, "
                f"{edge.smg_routes}u"
                "},"
            )
        lines.extend(["};", ""])

        mode = CPP_ROUTE_MODES[graph.mode]
        graph_rows.append(
            "    {"
            f'"{graph.map_name}", '
            f"{mode}, "
            f"{str(graph.role == 'attacker').lower()}, "
            f"{str(graph.phase == 'postplant').lower()}, "
            f"{name}Nodes, "
            f"sizeof({name}Nodes) / sizeof({name}Nodes[0]), "
            f"{name}Edges, "
            f"sizeof({name}Edges) / sizeof({name}Edges[0])"
            "},"
        )

    lines.extend(
        [
            "} // namespace",
            "",
            "const bot_demo_route_graph_t g_botDemoRouteGraphs[] = {",
            *graph_rows,
            "};",
            "",
            "const unsigned int g_botDemoRouteGraphCount =",
            "    sizeof(g_botDemoRouteGraphs)"
            " / sizeof(g_botDemoRouteGraphs[0]);",
            "",
        ]
    )
    return "\n".join(lines)


def graph_summary(
    graphs: list[GraphStats], xy_cell: float, z_cell: float
) -> dict[str, Any]:
    return {
        "schema_version": 3,
        "xy_cell_units": xy_cell,
        "z_cell_units": z_cell,
        "evidence_policy": {
            "geometry": (
                "eligible demo routes plus human FFA/team-match telemetry"
            ),
            "normal_behavior": (
                "complete normal-mode FFA recorder lives and human FFA lives"
            ),
            "smg_behavior": (
                "normal behavior candidates with dominant SMG weapon"
            ),
        },
        "graphs": [
            {
                "map": graph.map_name,
                "mode": graph.mode,
                "role": graph.role,
                "team": graph.team,
                "phase": graph.phase,
                "nodes": len(graph.nodes),
                "edges": len(graph.edges),
                "geometry_routes": graph.geometry_routes,
                "normal_routes": graph.normal_routes,
                "smg_routes": graph.smg_routes,
                "demo_routes": graph.demo_routes,
                "telemetry_routes": graph.telemetry_routes,
            }
            for graph in graphs
        ],
    }


def main() -> int:
    args = parse_args()
    if args.xy_cell <= 0 or args.z_cell <= 0:
        raise SystemExit("cell sizes must be positive")

    graphs = build_graphs(
        args.database,
        args.xy_cell,
        args.z_cell,
        args.telemetry,
    )
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_text(
        render_cpp(graphs, args.xy_cell, args.z_cell), encoding="utf-8"
    )

    summary = graph_summary(graphs, args.xy_cell, args.z_cell)
    if args.summary:
        args.summary.parent.mkdir(parents=True, exist_ok=True)
        args.summary.write_text(
            json.dumps(summary, indent=2) + "\n", encoding="utf-8"
        )
    print(json.dumps(summary, indent=2))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
