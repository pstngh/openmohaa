# Demo-derived bot routes

`build_bot_route_data.py` converts the completed demo SQLite index and optional
movement-telemetry captures into the compact
`code/fgame/playerbot_route_data.cpp` runtime graphs.

The generated graphs use 512-unit XY cells and 192-unit Z cells. Every eligible
normal, partial, remote-visible, realism, and supplied human-telemetry
trajectory can contribute geometry. Route selection prefers complete
normal-mode FFA SMG lives, falls back to other complete normal-mode FFA lives
when SMG support is sparse, and uses geometry counts only when no reliable
behavior sample exists. Team-match evidence on practice maps contributes
connectivity but never FFA behavior weights.

The output contains separate pre-plant and post-plant graphs for attackers and
defenders on `obj/obj_team2` and `obj/obj_team4`. It also contains dedicated
free-for-all roaming graphs for `dm/mohdm6` and the `obj_team2` practice arenas
`dm/main`, `dm/crnodoors`, `dm/downladder`, and `dm/vents`. Each practice graph
uses only trajectories recorded under that map name, keeping bots inside its
observed 1v1 area.

```powershell
python code\tools\demoroutes\build_bot_route_data.py `
  D:\misc\demo-datasets\all-demos\derived\demo-index.sqlite3 `
  code\fgame\playerbot_route_data.cpp `
  --telemetry C:\path\to\movement_meta1.zip `
  --telemetry C:\path\to\movement_meta2.zip `
  --telemetry C:\path\to\latest-telemetry-directory `
  --summary code\tools\demoroutes\playerbot_route_data_summary.json
```

The generator is deterministic for fixed index and telemetry inputs. Run its
unit tests with:

```powershell
python code\tools\demoroutes\test_build_bot_route_data.py
```
