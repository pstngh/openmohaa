# Active branch contract

This repository has one GitHub remote and two independent, long-lived product branches. The branch checked out in the current worktree owns the mutable state for that workstream.

| Branch | Previous name | Canonical responsibility |
| --- | --- | --- |
| `bots/server-ai` | `agent/bot-demo-route-graphs` | Linux x64 dedicated-server-only bot AI, telemetry, objective/FFA route graphs, live tuning, and VPS operation. |
| `bots/macos-client` | `claude/bot-movement-shooting-slowdown-anbint` | arm64 macOS client and launcher, local bot controls, normal online-client behavior, UI/input features, and passive client telemetry. |

The branches shared history through `8f614e61` (`fix(fgame): add human-like bot spacing`) at the 2026-08-09 retrofit. The server branch inherited earlier client/launcher files, but its active build and future ownership are server-only. Historical overlap does not authorize whole-branch merges.

## Ownership rules

- Develop server navigation, objectives, server telemetry, demo routes, and Linux deployment on `bots/server-ai` first.
- Develop macOS packaging, launcher UI, client telemetry, client HUD/input, and online-client behavior on `bots/macos-client` first.
- A bot gameplay change needed by both products originates on the branch where it can be measured, then moves as a reviewed commit.
- `docs/STATE.md` and `.agent/plans/active/` are branch-local and must never be copied wholesale between branches.
- Stable common documentation may be synchronized only after comparing both versions.

## Cross-branch inspection

Fetch before reasoning about the peer branch:

```sh
git fetch origin
git show origin/bots/server-ai:docs/STATE.md
git show origin/bots/macos-client:docs/STATE.md
git log --left-right --cherry-pick --oneline origin/bots/macos-client...origin/bots/server-ai
```

Record the peer tip inspected and any candidate commits in the current branch's state or active plan. Do not claim parity from filenames alone.

## Transfer protocol

1. Identify the smallest source commit or intentionally prepared commit series.
2. Inspect `git show --stat --patch <sha>` and verify it contains no branch-only infrastructure.
3. Record why the receiving product needs it.
4. Cherry-pick the explicit hash; do not merge the source branch by default.
5. Resolve receiving-branch semantics deliberately, especially telemetry schemas, cvars, CI, and documentation.
6. Run the receiving branch's focused tests and build.
7. Record source hash, resulting hash, validation, and any omissions in both handoffs.

If a change cannot be separated cleanly, prepare a new focused port instead of importing unrelated history.
