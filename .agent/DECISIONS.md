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
