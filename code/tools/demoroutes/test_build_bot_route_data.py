#!/usr/bin/env python3

import unittest

from build_bot_route_data import (
    GraphStats,
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
            "obj/obj_team2", "attacker", 3, "preplant"
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
            "obj/obj_team4", "defender", 3, "postplant"
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
        self.assertIn("1u, 1u, 0u", output)


if __name__ == "__main__":
    unittest.main()
