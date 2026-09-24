# Current State

Reconciled: 2026-08-28

## Active task

None in source. Persistent chat bans and the requested history cleanup are
complete; GitHub, Pages, deployment, and live behavior remain separate evidence.

## Why this serves the goal

The fork now has a compact history for the name-change policy and a dedicated,
connection-safe chat restriction that adds no polling or per-frame work.

## Status

Source implementation and local validation are complete.

## Completed

- The three adjacent name-change changes are one atomic
  `feat(server): limit repeated name changes` commit, followed by the unchanged
  lightweight hidden-SMG-bot patch.
- Persistent IPv4/IPv6 chat bans are stored only in `chatbans.dat`; normalized
  paths that collide with `sv_banFile` are refused.
- Exact addresses and CIDR ranges support reasons, safe parsing, subsumption,
  exact deletion, pagination, reload, and flush operations up to 1024 entries.
- Admin add/remove/list commands use access right `64`. Console equivalents are
  registered with the server operator commands.
- Matching clients remain connected and playable. Text and taunt commands are
  dropped before the game module, while admin and gameplay commands remain
  available. Persistent state is cached on connect and refreshed only after a
  list change.
- Affected clients receive one private notice with the reason. Named add/remove
  announcements omit addresses and reasons, while server logs retain details.
- Configuration and operator behavior are documented. GitHub Pages is configured
  to publish through GitHub Actions.

## Remaining

No source work. Check current GitHub Actions and Pages state independently when
remote publication status matters.

## Blockers / unknowns

- The authoritative live game server is not identified; deployment is outside
  this change.
- Live stock-client behavior of chat-ban notices and suppression is unverified.
- Runtime rotation and server configuration remain operator-owned and untracked.

## Assumptions

- `GOAL.md` remains current until the user explicitly changes it.
- A committed or CI-passing change is not assumed deployed or live-tested.

## Verified

- Debug dedicated-server and game-module targets build successfully with GCC.
- CTest passes the LZ77 and chat-ban suites. The chat-ban core also passes
  AddressSanitizer and UndefinedBehaviorSanitizer.
- Dedicated-server smoke tests cover empty/startup loading, IPv4/IPv6 records,
  malformed records, normalized ban-file collision refusal, and persistent
  add/reload/delete/flush command flows without a crash.
- The replayed hidden-SMG-bot source matches its prior tree, excluding deliberate
  continuity-note normalization.

## Relevant locations

- Chat-ban runtime: `code/server/sv_chatban.c`
- Chat-ban parsing and matching: `code/server/sv_chatban_core.c`
- Unit tests: `code/server/tests/test_chatban.c`
- Configuration docs: `docs/markdown/03-configuration/01-configuration.md`

## Exact next action

For a publication check, inspect the workflows for the current `main` commit and
the Pages deployment rather than inferring status from this file. For live-game
work, first identify the authoritative server and obtain explicit deployment
scope.
