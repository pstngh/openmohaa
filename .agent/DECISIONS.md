# Decisions

Record only rationale a new agent cannot safely reconstruct from code. Preserve
superseded decisions rather than silently rewriting them.

## D-001: Unusual bot and admin behavior is intentional

Five bots are permanent melee roombas in real slots and are reported as players
with synthetic IPs and plausible pings. Humans do not replace them. Admin
passwords remain plaintext with no attempt throttling. Conventional changes to
these behaviors require explicit user approval.

## D-002: Simplicity and performance are hard requirements

Use the smallest existing event path. Avoid recurring work, allocations, and
network messages. Review hot-path and network cost for every meaningful feature.

## D-003: Rotation and announcements are native controls

Rotation needs repeatable per-entry durations and a configurable population lock
that counts all clients while preserving manual map changes. Announcements use
native configurable text/interval. Do not restore a PK3 announcement timer.

## D-004: Spectator and naming rules are gameplay policy

Voluntary spectator changes are delayed from binds and ESC; death during the
delay counts and restarts it, but nonfatal damage does not. Spectator targets are
retained when possible. Blank/default/FrontLine-prefixed names are replaced with
unique `ForteSoldier` identities.

## D-005: SP FFA support stays narrow and server-only

Stock clients are required. `m6l2a` keeps its own package; six other SP maps use
a separate package and are not auto-rotated. SP taunts resolve server-side only
when used, with no frame hook. A hard-coded 45-second cooldown was rejected in
favor of runtime configuration. Persistent taunt bans are paused and individual
opt-out is not approved.

## D-006: Git, deployment, and gameplay are separate evidence

A commit, deployed binary, reachable service, and live behavior may differ. The
old managed VPS became a redirect, so identify the authoritative server and
verify each claim directly before changing deployment or reporting success.

## D-007: Human name changes are limited per connection

Allow one exact name change for a human client, then log and kick on the second
with `too many name changes`. Record client-name messages in `qconsole.log`
without echoing them to the live console. Do not count the initial name,
identical userinfo updates, non-name updates, or bots. Preserve the count across
map changes but reset it on reconnect, and stop game-side userinfo handling
after the kick.

## D-008: Persistent chat bans are separate and connection-safe

Use `chatbans.dat`, never `serverbans.dat`, for persistent IPv4/IPv6 chat
restrictions. A matching client stays connected and fully playable; only text
and taunt commands are dropped. Cache the match on connection and recompute it
only when the list changes, rather than performing file work or polling in a
hot path. Keep temporary `ad_dischat` and `ad_distaunt` state independent. Do
not add exceptions, expiration timers, or a second enforcement layer in the
game module.

## D-009: Regular SMG bots stay hidden and idle-free

Regular SMG bots use game-only slots above `sv_maxclients`, so the master
server, GameSpy and the rotation population never count them; Roombas stay the
only reported bots. The fill counts connected humans plus Roombas, and no
regular bot runs while no human is connected. The user approved this on
2026-09-23 with game API 17, so the server and game module ship together.

## D-010: Fixes to fork commits are folded into them

The owner wants each fork commit to carry its own corrections. Fold a fix into
the commit it corrects and force-push `main` after pushing a backup branch,
rather than stacking separate fix commits on top. Fixes to upstream code stay
separate commits. A change to upstream files that only matters because of a
fork commit belongs to that commit: the SMG bot commit re-enabled Recast
navigation, so the obstacle-scan interval lives there, and the name-change
commit reworked `SV_PrintfClient`, so its format check lives there. The owner
approved the rewrite on 2026-09-24, asking for `Co-Authored-By: Claude`
trailers on the commits that absorbed fixes and for `dev` to move to the
rewritten commit.

Later that day, after a second audit, the owner superseded the trailers:
Claude is author and committer of every commit after `a2f3401`, dated the day
of the rewrite in history order. The same pass folded the owner's roomba
target fix into the roomba commit, put the upstream `Info_RemoveKey` fix
first, and left `main` as the only branch; `dev` still follows its commit.
