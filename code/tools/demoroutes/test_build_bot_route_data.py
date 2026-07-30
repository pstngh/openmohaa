#!/usr/bin/env python3

import json
import sqlite3
import tempfile
import unittest
import zipfile
import zlib
from pathlib import Path

from build_bot_route_data import (
    GraphStats,
    add_telemetry_routes,
    build_graphs,
    collapse_route,
    render_cpp,
    route_phases,
)


class BotRouteDataTests(unittest.TestCase):
    def test_collapse_averages_cells_and_removes_boundary_jitter(self) -> None:
        points = [
            [0, 10, 0, 0],
            [1, 20, 0, 0],
            [2, 140, 0, 0],
            [3, 30, 0, 0],
        ]

        collapsed = collapse_route(points, 128.0, 96.0)

        self.assertEqual(len(collapsed), 1)
        self.assertEqual(collapsed[0][0], (0, 0, 0))
        self.assertEqual(collapsed[0][1], (20.0, 0.0, 0.0))

    def test_route_phases_split_at_the_first_plant(self) -> None:
        points = [[100, 0, 0, 0], [200, 1, 0, 0], [300, 2, 0, 0]]

        phases = route_phases(points, 200)

        self.assertEqual(phases["preplant"], [points[0]])
        self.assertEqual(phases["postplant"], points[1:])

    def test_graph_counts_each_edge_once_per_route(self) -> None:
        graph = GraphStats(
            "obj/obj_team2", "objective", "attacker", 3, "preplant"
        )
        first = ((0, 0, 0), (0.0, 0.0, 0.0))
        second = ((1, 0, 0), (128.0, 0.0, 0.0))

        graph.add([first, second, first, second], True, True)

        self.assertEqual(graph.geometry_routes, 1)
        self.assertEqual(graph.normal_routes, 1)
        self.assertEqual(graph.smg_routes, 1)
        self.assertEqual(graph.edges[(first[0], second[0])].smg_routes, 1)

    def test_rendered_data_records_all_evidence_tiers(self) -> None:
        graph = GraphStats(
            "obj/obj_team4", "objective", "defender", 3, "postplant"
        )
        graph.add(
            [
                ((0, 0, 0), (0.0, 0.0, 0.0)),
                ((1, 0, 0), (512.0, 0.0, 0.0)),
            ],
            True,
            False,
        )

        output = render_cpp([graph], 512.0, 192.0)

        self.assertIn('"obj/obj_team4"', output)
        self.assertIn("false, true", output)
        self.assertIn("BOT_DEMO_ROUTE_OBJECTIVE", output)
        self.assertIn("1u, 1u, 0u", output)

    def test_rendered_ffa_graph_has_explicit_mode(self) -> None:
        graph = GraphStats(
            "dm/mohdm6", "free_for_all", "roamer", 0, "roam"
        )
        graph.add(
            [
                ((0, 0, 0), (0.0, 0.0, 0.0)),
                ((1, 0, 0), (512.0, 0.0, 0.0)),
            ],
            True,
            True,
        )

        output = render_cpp([graph], 512.0, 192.0)

        self.assertIn('"dm/mohdm6"', output)
        self.assertIn("BOT_DEMO_ROUTE_FREE_FOR_ALL", output)
        self.assertIn("false, false", output)

    def test_ffa_graph_combines_teams_but_excludes_team_deathmatch(
        self,
    ) -> None:
        with tempfile.TemporaryDirectory() as directory:
            database = Path(directory) / "routes.sqlite3"
            connection = sqlite3.connect(database)
            connection.executescript(
                """
                CREATE TABLE meta (
                    key TEXT PRIMARY KEY,
                    value TEXT NOT NULL
                );
                CREATE TABLE demos (
                    demo_id TEXT PRIMARY KEY,
                    status TEXT NOT NULL
                );
                CREATE TABLE rounds (
                    round_id TEXT PRIMARY KEY,
                    first_plant_time INTEGER
                );
                CREATE TABLE routes (
                    route_id TEXT PRIMARY KEY,
                    demo_id TEXT NOT NULL,
                    map_name TEXT NOT NULL,
                    mode TEXT NOT NULL,
                    role TEXT NOT NULL,
                    team INTEGER,
                    round_id TEXT,
                    route_scope TEXT NOT NULL,
                    analysis_usage TEXT NOT NULL,
                    weapon_category TEXT NOT NULL,
                    points_codec TEXT NOT NULL,
                    points_blob BLOB NOT NULL,
                    eligible_route INTEGER NOT NULL
                );
                """
            )
            connection.execute(
                "INSERT INTO meta VALUES ('schema_version', '3')"
            )
            connection.execute(
                "INSERT INTO demos VALUES ('demo', 'complete')"
            )
            points = zlib.compress(
                json.dumps(
                    [[0, 0, 0, 0], [100, 600, 0, 0]]
                ).encode("utf-8")
            )
            for route_id, map_name, mode, team in (
                ("ffa-allies", "dm/mohdm6", "free_for_all", 3),
                ("ffa-axis", "dm/mohdm6", "free_for_all", 4),
                ("tdm", "dm/mohdm6", "team_deathmatch", 3),
                ("practice-ffa", "dm/vents", "free_for_all", 3),
                ("practice-tdm", "dm/vents", "team_deathmatch", 4),
            ):
                connection.execute(
                    """
                    INSERT INTO routes VALUES (
                        ?, 'demo', ?, ?, 'not_objective', ?,
                        NULL, 'recorder_life', 'behavior_candidate',
                        'smg', 'zlib-json-v1', ?, 1
                    )
                    """,
                    (route_id, map_name, mode, team, points),
                )
            connection.commit()
            connection.close()

            graphs = build_graphs(database, 512.0, 192.0)

        ffa_graphs = [
            graph
            for graph in graphs
            if graph.map_name == "dm/mohdm6"
        ]
        self.assertEqual(len(ffa_graphs), 1)
        graph = ffa_graphs[0]
        self.assertEqual(graph.mode, "free_for_all")
        self.assertEqual(graph.role, "roamer")
        self.assertEqual(graph.team, 0)
        self.assertEqual(graph.phase, "roam")
        self.assertEqual(graph.geometry_routes, 2)
        self.assertEqual(graph.normal_routes, 2)
        self.assertEqual(graph.smg_routes, 2)

        practice = next(
            graph
            for graph in graphs
            if graph.map_name == "dm/vents"
        )
        self.assertEqual(practice.geometry_routes, 2)
        self.assertEqual(practice.normal_routes, 1)
        self.assertEqual(practice.smg_routes, 1)

    def test_telemetry_splits_lives_and_uses_tdm_as_geometry(
        self,
    ) -> None:
        with tempfile.TemporaryDirectory() as directory:
            archive_path = Path(directory) / "movement.zip"
            metadata = "\n".join(
                (
                    "[session ffa]",
                    "session_id=ffa",
                    "map=dm/main",
                    "g_gametype=1",
                    "",
                    "[session tdm]",
                    "session_id=tdm",
                    "map=dm/main",
                    "g_gametype=2",
                    "",
                )
            )
            header = (
                "session_id,session_ms,map,client_id,name,is_bot,"
                "spectator,alive,origin_x,origin_y,origin_z,weapon"
            )
            frames = "\n".join(
                (
                    header,
                    "ffa,0,dm/main,0,human,0,0,1,0,0,0,mp40",
                    "ffa,50,dm/main,0,human,0,0,1,600,0,0,mp40",
                    "ffa,100,dm/main,0,human,0,0,0,600,0,0,mp40",
                    "ffa,0,dm/main,1,bot,1,0,1,0,0,0,mp40",
                    "ffa,50,dm/main,1,bot,1,0,1,600,0,0,mp40",
                    "tdm,0,dm/main,0,human,0,0,1,0,0,0,bar",
                    "tdm,50,dm/main,0,human,0,0,1,600,0,0,bar",
                    "tdm,100,dm/main,0,human,0,0,0,600,0,0,bar",
                    "",
                )
            )
            with zipfile.ZipFile(archive_path, "w") as archive:
                archive.writestr("movement_meta.txt", metadata)
                archive.writestr("movement_frames.csv", frames)

            key = ("dm/main", "free_for_all", "roamer", 0, "roam")
            graph = GraphStats(*key)
            add_telemetry_routes(
                {key: graph}, [archive_path], 512.0, 192.0
            )

        self.assertEqual(graph.geometry_routes, 2)
        self.assertEqual(graph.normal_routes, 1)
        self.assertEqual(graph.smg_routes, 1)
        self.assertEqual(graph.demo_routes, 0)
        self.assertEqual(graph.telemetry_routes, 2)


if __name__ == "__main__":
    unittest.main()
