# Current State

Reconciled: 2026-09-24

## Active task

None in progress. On 2026-09-24, with the owner's approval and after a second
audit of every commit after `a2f3401`, `main` was rewritten so each fork
commit carries its own fixes (D-010). Next work is the owner's pick from the
recommendations below.

## Why this serves the goal

The fixes remove a map-dropping crash, false name-change kicks, admin and
chat-restriction bypasses, a raw `say` flood path, stalled timers after
restarts, chat-ban file hazards and an upstream pre-auth stack overflow,
without adding recurring work. Folding them in keeps each feature commit
correct on its own.

## Status

`main` is 18 commits on `a2f3401` (upstream): the upstream `Info_RemoveKey`
fix, the 16 fork commits, and this notes commit. Nothing is deployed.

## History

- The upstream `Info_RemoveKey` fix comes first, so every fork commit has it.
- Folded into the fork commits they correct: the first audit's fixes and the
  SMG bot cost changes (game API 17), the roomba target fix (into the roomba
  commit), the `PF_MSG_SetClient` range guard (into the first server commit,
  where upstream bots could already reach it), and the second audit's fixes
  listed under Completed.
- Owner's choices: Claude is author and committer of every commit after
  `a2f3401`, dated 2026-09-24 in history order, with no co-author trailers.
  `main` is the only branch; `backup/main-pre-rewrite` and the claude branch
  are deleted once the new `main` is verified.
- The `dev` tag still points at the old continuity commit `43d3ce2`, which
  keeps the old history reachable. This environment cannot push tags, so the
  owner moves it (see Exact next action).
- Clones and deployed checkouts made before the rewrite must reset to the new
  `main`.

## Completed

- Game-only SMG bots no longer call `SetUserinfo` on a name collision (the
  call raised `ERR_DROP`); roomba bots report their fixed names to the server.
- Name-change counting compares the name the client sends, so forced
  `ForteSoldier#` and `(2)` names no longer get players kicked, and every
  requested rename counts even while the game has left the server copy of
  the name empty.
- Names of clients dropped while loading are released, so rejoiners keep
  their name instead of getting a `(2)` suffix.
- `ad_logout` keeps mutes and chat bans; `dmmessage` is classified from
  argument 2 like the game; `ad_rcon` is queued and keeps quoting; admin slot
  arguments must be digits.
- Raw client `say` is console-only in multiplayer again.
- The spectator bind or ESC button also works for a player who is spectating
  while still on a team, e.g. after a map change without a weapon.
- Announcement and bot-ping timers recover when `svs.time` restarts.
- `maprotation_start` applies the population lock to the first entry.
- Chat bans reject single-colon addresses (DNS), protected extensions and
  uncomparable `sv_banFile` values, and save only to the file last loaded.
- Upstream `Info_RemoveKey` buffers cover the whole info string.
- `PF_MSG_SetClient` ignores client numbers without a server slot.
- Roombas acquire targets on maps without a farplane.
- SMG bots fill from connected humans plus Roombas (new game import) and do
  not run while no human is connected; with `sv_maxregularbots 0` nothing
  counts connected humans each frame; stalled side searches repeat at the
  stock 250 ms after one quick recheck; the jump checks share one forward
  trace; the obstacle scan runs every 250 ms.

## Remaining

Audit recommendations that need an owner decision or game assets:

- `sp_ffa`: `removeclass Trigger NIL 1` also removes items, script objects and
  models, doors (their area portals stay closed) and speakers. `m4l3` also
  removes its doors explicitly, so fixing only that line will not reopen its
  portals.
- `m6l2a`: a kept `trigger_changelevel` could end the match; `cg_rain_*`
  cvars never reach clients; neither pack precaches `dm_50_healthbox.tik`; the
  package holds CRLF copies of the scripts, so it cannot be rebuilt
  byte-for-byte from source; the "dm" sound-alias rule applies on every
  multiplayer map, not only single-player ones as documented.
- SP taunts: if the stock taunt aliases are streamed, their first use on an
  SP map adds a sound configstring mid-game (confirm with the game files).
- SMG bots: fire while still turning; the navigation mesh is still built at
  every map load while capacity is nonzero.
- Roombas: a visible but unreachable target locks them; obituary lines are
  no longer logged; names shift after a bot is kicked; `sv_maxclients` cannot
  be lowered while bots hold top slots.
- Names: "Your name is now" never reaches a player renamed on joining (it is
  sent before the client can receive commands); a userinfo padded close to
  1350 bytes loses its name when the game writes back a fixed name, leaving
  a blank server-side name.
- Spectating: `join_team` from the ESC menu still escapes a death through the
  upstream respawn path; only `g_teamswitchdelay` limits it.
- Rotation: `autoexec.cfg` runs before the commands exist; a missing map at
  load drops the server; `maprotation_advance_internal` can be run by hand.
- Chat bans: reads can come from another XDG home path; saves are not atomic
  and drop `//` comment lines and rejected records; a UTF-8 BOM makes the
  first record invalid; `callvote` text still broadcasts; `::ffff:` entries
  never match.
- Admin: ban commands always report success; `ad_banip` wildcards widen
  silently; `ad_banip`, `ad_banipr`, `ad_banipsilent` and `ad_unbanip` accept
  bare numbers, which the engine treats as a slot or ban index; admin login
  names are broadcast.
- Docs: the compiling guide still lists SDL2/OpenAL; some new files have short
  license headers.

## Blockers / unknowns

- CI compiles and passes unit tests, but the Builds workflow fails at
  `upload-artifact` because the account's Actions artifact storage quota is
  full, so no `out-linux-amd64` package is produced until old artifacts are
  deleted or usage is recalculated. CodeQL fails because code scanning is not
  enabled in the repository settings (since 2026-08-31, before this work).
- This environment's git access refuses tag pushes (HTTP 403); branch pushes
  work.
- The authoritative live game server is not identified; deployment is outside
  this change.
- The SP-map findings need the map files to confirm.

## Verified

- Debug and RelWithDebInfo builds; CTest (LZ77, chat bans) passes.
- Dedicated-server smoke tests: startup, chat-ban path refusals, rotation
  start holding on the lock map, no DNS traffic for rejected addresses.
- ASan/UBSan harnesses: chat-ban load/write refusals, name-change scenarios,
  the connect-userinfo overflow repro, and old-versus-new SMG jump checks
  (identical results on 240,000 random states).
- The game module loads under the API 17 handshake; SMG bot behavior itself
  was not run without map assets.
- Second audit: seven reviewers read every commit after `a2f3401` at its own
  point in history, and each finding was re-checked against the code. The
  final code differs from the first rewrite only by the four second-audit
  fixes.
- A harness replaying the rename path kicks the padded-userinfo client on its
  second rename, and never kicks for game-forced names or settings changes.
- Every commit passes a clean GCC Debug build and CTest; tests are registered
  only from the chat-ban commit on.

## Exact next action

The owner moves `dev` to the rewritten continuity commit: `git fetch origin &&
git push --force-with-lease=refs/tags/dev:43d3ce2a0dabcb1e9c8971261fe8e185334f7f32
origin ee341e15133c171d7b70a2e125810b1e0a24e53d:refs/tags/dev`. Then reset every existing clone and deployed
checkout to the new `main` (after saving any local work), and deploy only from
it, server and game module together (game API 17).
