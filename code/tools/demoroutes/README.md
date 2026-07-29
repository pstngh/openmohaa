# Demo-derived bot routes

`build_bot_route_data.py` converts the completed demo SQLite index into the
compact `code/fgame/playerbot_route_data.cpp` runtime graphs.

The generated graphs use 512-unit XY cells and 192-unit Z cells. Every eligible
normal, partial, remote-visible, and realism trajectory can contribute
geometry. Route selection prefers complete normal-mode SMG recorder lives,
falls back to complete normal-mode recorder lives when SMG support is sparse,
and uses geometry counts only when no reliable behavior sample exists.

The output contains separate pre-plant and post-plant graphs for attackers and
defenders on `obj/obj_team2` and `obj/obj_team4`.

```powershell
python code\tools\demoroutes\build_bot_route_data.py `
  D:\misc\demo-datasets\all-demos\derived\demo-index.sqlite3 `
  code\fgame\playerbot_route_data.cpp `
  --summary code\tools\demoroutes\playerbot_route_data_summary.json
```

The generator is deterministic for a fixed index. Run its unit tests with:

```powershell
python code\tools\demoroutes\test_build_bot_route_data.py
```
