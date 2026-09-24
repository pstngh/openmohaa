# Goal

## North star

Maintain this OpenMoHAA fork as a lean, predictable, robust Allied Assault FFA
dedicated server. Preserve the operator's exact gameplay and server-browser
behavior without lag, freezes, micro-freezes, needless recurring work, or
avoidable network traffic.

## Why

This is a deliberately narrow public-server build. Exact behavior,
stock-client compatibility, simplicity, and low operational risk matter more
than conventional defaults or feature breadth.

## Success criteria

- The configured bots and public player reporting remain stable with humans.
- Admin, rotation, announcements, spectator handling, names, and supported SP
  maps behave as intended on stock clients.
- Changes are narrow, understandable, tested, and negligible when idle.
- Git, deployment, and live-game claims are verified separately.

## Constraints

- Exactly five permanent melee-only roomba bots use real client slots. Humans
  never replace them. They appear as players with intentional synthetic IPs and
  player-like fake pings.
- Plaintext admin passwords and no login-attempt throttling are intentional.
- Prefer existing event paths and one-time work; avoid new polling, per-frame
  work, allocations, timers, or messages unless required.
- Native rotation accepts repeated `map:minutes` entries. Population includes
  every connected client. Below the configurable threshold (default 12), hold
  the next entry on `dm/mohdm6:20`; manual admin map changes remain allowed.
- Announcements are native and configurable, not dependent on a PK3 timer.
- Voluntary spectating from binds or ESC is delayed. Fatal damage counts and
  restarts the delay; nonfatal damage does not. Retain spectator targets through
  death when possible.
- Blank, `unnamedsoldier`, and `-=[FrontLine]=-`-prefixed names become unique
  `ForteSoldier` names.
- A human may change their exact name once per connection. Record name messages
  in `qconsole.log` without echoing them to the live console, and kick on the
  second change; bots remain exempt.
- Require no custom client. Keep `m6l2a` in its narrow server-only package and
  other supported SP FFA maps in a separate package, outside automatic rotation.
- Taunt cooldown is runtime policy; keep the source default at 1000 ms.
- Store no secrets or private server addresses here.

## Non-goals

- General OpenMoHAA modernization or feature-rich bot AI.
- The broad Reborn package, custom clients, or automatic SP-map rotation.
- Persistent IP taunt bans (paused) or individual taunt opt-out (exploratory).
- Any application, build, test, deployment, or runtime role for continuity.
