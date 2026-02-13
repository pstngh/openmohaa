# Rebuild Guide (from scratch) for 7 selected gameplay/client tweaks

This document is intentionally scoped to only these commits and is written so another AI can recreate them quickly and reliably:

- `bd994e9` - Hide local team icon from HUD
- `274d467` - Force spectate follow forward/up offsets to zero and skip lean for dead targets
- `229dcd1` - Set default `cl_maxpackets` to 125 and `com_maxfps` to 250
- `4871605` - Enable automatic demo recording by default
- `c86b1d1` - Disable spectator follow camera lean offset
- `150fbec` - Add 5-second movement delay at objective round start
- `d5f1798` - Force spectator follow camera lean angle to zero

---

## 0) Fast execution strategy for another AI

1. Edit exactly these files:
   - `code/cgame/cg_drawtools.cpp`
   - `code/fgame/player.cpp`
   - `code/client/cl_main.cpp`
   - `code/qcommon/common.c`
2. Apply the 7 changes below in order.
3. Prefer small surgical diffs (single-function edits) to avoid merge conflicts.
4. Validate with grep checks listed at the end.

---

## 1) `bd994e9` — Hide local team icon from HUD

### File
- `code/cgame/cg_drawtools.cpp`

### Function
- `CG_DrawPlayerTeam()`

### Intent
Remove rendering of the local team badge (allies/axis) from HUD.

### Exact implementation pattern
Replace the function body with a no-op comment:

```cpp
void CG_DrawPlayerTeam()
{
    // Hide local team badge HUD icon.
}
```

### Notes
- This intentionally removes all conditional checks and draw calls in that function.
- Keep the function symbol present to avoid link/callsite changes.

---

## 2) `274d467` — Force spectate follow forward/up offsets to zero + skip lean for dead targets

### File
- `code/fgame/player.cpp`

### Function
- `Player::GetSpectateFollowOrientation(Player *pPlayer, Vector& vPos, Vector& vAng)`

### Intent
- Ignore runtime nonzero values for `g_spectatefollow_forward` and `g_spectatefollow_up`.
- Keep right offset behavior unchanged.
- Do not apply lean-based side shift when target is dead.

### Exact edits

1. Near top of function, force cvars back to zero if changed:

```cpp
if (g_spectatefollow_forward->value != 0.0f) {
    gi.cvar_set("g_spectatefollow_forward", "0");
}

if (g_spectatefollow_up->value != 0.0f) {
    gi.cvar_set("g_spectatefollow_up", "0");
}
```

2. In camera offset math:
- Change forward contribution to `0.0f`
- Keep right using existing cvar
- Change up contribution to `0.0f`

```cpp
vCamOfs += forward * 0.0f;
vCamOfs += right * g_spectatefollow_right->value;
vCamOfs += up * 0.0f;
```

3. Lean offset gate should include alive check:

```cpp
if (!pPlayer->IsDead() && pPlayer->client->ps.fLeanAngle != 0.0f) {
    vCamOfs += pPlayer->client->ps.fLeanAngle * 0.65f * right;
}
```

---

## 3) `229dcd1` — Defaults: `cl_maxpackets=125`, `com_maxfps=250`

### Files
- `code/client/cl_main.cpp`
- `code/qcommon/common.c`

### Intent
Raise baseline networking/frame defaults.

### Exact edits

1. In `CL_Init` (`cl_main.cpp`), change:

```cpp
cl_maxpackets = Cvar_Get ("cl_maxpackets", "30", CVAR_ARCHIVE );
```

to:

```cpp
cl_maxpackets = Cvar_Get ("cl_maxpackets", "125", CVAR_ARCHIVE );
```

2. In `Com_Init` (`common.c`), change:

```c
com_maxfps = Cvar_Get( "com_maxfps", "85", CVAR_ARCHIVE );
```

to:

```c
com_maxfps = Cvar_Get( "com_maxfps", "250", CVAR_ARCHIVE );
```

---

## 4) `4871605` — Enable auto demo recording by default

### File
- `code/client/cl_main.cpp`

### Function
- `CL_Init`

### Intent
Turn on auto demo recording for users by default.

### Exact edit
Change:

```cpp
cl_autoRecordDemo = Cvar_Get ("cl_autoRecordDemo", "0", CVAR_ARCHIVE);
```

to:

```cpp
cl_autoRecordDemo = Cvar_Get ("cl_autoRecordDemo", "1", CVAR_ARCHIVE);
```

---

## 5) `c86b1d1` — Disable spectator follow camera lean offset

### File
- `code/fgame/player.cpp`

### Function
- `Player::GetSpectateFollowOrientation(...)`

### Intent
Remove side camera shift caused by target lean while spectating.

### Exact implementation pattern
Delete or neutralize the lean-offset block:

```cpp
if (pPlayer->client->ps.fLeanAngle != 0.0f) {
    vCamOfs += pPlayer->client->ps.fLeanAngle * 0.65f * right;
}
```

and replace with a comment/no-op, e.g.:

```cpp
// Keep spectator follow camera stable: do not apply target lean offset.
```

### Important interaction
- This commit supersedes the conditional lean handling introduced earlier (alive-check path).
- End state after applying both commits should be: **no lean offset applied at all** in this function.

---

## 6) `150fbec` — Add 5-second movement delay at objective round start

### File
- `code/fgame/player.cpp`

### Function
- `Player::ClientMove(usercmd_t *ucmd)`

### Intent
In objective-family game types, freeze movement briefly at round start.

### Exact behavior to implement
- Apply only if `g_gametype->integer >= GT_OBJECTIVE`.
- Detect transition into `level.RoundStarted()`.
- On round start, set unlock timestamp to `level.inttime + 5000`.
- While before unlock time and round started:
  - set `PMF_NO_MOVE`
  - set `PMF_NO_PREDICTION`
- Handle map/restart time reset (`level.inttime` decreasing) by resetting static trackers.

### Implementation template
Use function-local statics exactly for persistence across frames:

```cpp
static bool s_objectiveRoundWasStarted = false;
static int  s_objectiveMoveUnlockTime  = 0;
static int  s_lastLevelIntTime         = 0;
```

Then implement the round-state/update logic and pm_flags assignment as in the original patch behavior.

---

## 7) `d5f1798` — Force spectator follow camera lean angle to zero

### File
- `code/fgame/player.cpp`

### Function
- `Player::CopyStats(Player *player)`

### Intent
Ensure spectator camera does not inherit target lean angle.

### Exact edit
Replace:

```cpp
client->ps.fLeanAngle = player->client->ps.fLeanAngle;
```

with:

```cpp
client->ps.fLeanAngle = 0.0f;
```

Optional comment:

```cpp
// Keep spectator camera independent from target leaning.
```

---

## Final expected end-state checklist

Run these checks after editing:

1. Team icon function is a no-op:
   - Search `CG_DrawPlayerTeam` and verify no draw calls remain.

2. Spectate follow orientation:
   - `g_spectatefollow_forward` forced to `0` via `gi.cvar_set`
   - `g_spectatefollow_up` forced to `0` via `gi.cvar_set`
   - forward/up camera offset multipliers are `0.0f`
   - **no active lean offset addition** to `vCamOfs` remains

3. Defaults:
   - `cl_maxpackets` default string is `"125"`
   - `com_maxfps` default string is `"250"`
   - `cl_autoRecordDemo` default string is `"1"`

4. Objective round-start delay:
   - logic present in `ClientMove`
   - 5000 ms window
   - sets `PMF_NO_MOVE` and `PMF_NO_PREDICTION`

5. CopyStats lean behavior:
   - `client->ps.fLeanAngle = 0.0f;`

---

## Suggested grep verification commands

```bash
rg -n "void CG_DrawPlayerTeam|Hide local team badge" code/cgame/cg_drawtools.cpp
rg -n "g_spectatefollow_forward|g_spectatefollow_up|vCamOfs \+= forward \* 0.0f|vCamOfs \+= up \* 0.0f|fLeanAngle" code/fgame/player.cpp
rg -n "cl_maxpackets|cl_autoRecordDemo" code/client/cl_main.cpp
rg -n "com_maxfps" code/qcommon/common.c
rg -n "s_objectiveMoveUnlockTime|PMF_NO_MOVE|PMF_NO_PREDICTION|5000" code/fgame/player.cpp
```

