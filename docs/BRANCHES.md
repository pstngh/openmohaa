# Repository and branch contract

This repository has one long-lived branch, `main`. The Linux dedicated-server bot work lives in a separate repository.

| Repository and branch | Canonical responsibility |
| --- | --- |
| `pstngh/openmohaa-macos-client`, `main` | arm64 macOS client and launcher, local bot controls, normal online-client behavior, UI/input features, and passive client telemetry. |
| `pstngh/openmohaa`, `bots/server-ai` | Linux x64 dedicated-server-only bot AI, server telemetry, objective/FFA route graphs, live tuning, and VPS operation. |

Both lines grew from the same bot work and shared commits through `8f614e61` (`fix(fgame): add human-like bot spacing`). On 2026-09-24 this repository's history after the upstream base `a2f3401` (`docs: clarify contribution guidelines`) was rewritten into a clean series with the 2026-09-23 audit fixes folded into the commits they belong to. The previous history, including `8f614e61`, remains at the tag `dev`; keep that tag. Shared history does not authorize wholesale merges.

## Ownership rules

- Develop server navigation, objectives, server telemetry, demo routes, and Linux deployment in the server repository first.
- Develop macOS packaging, launcher UI, client telemetry, client HUD/input, and online-client behavior here first.
- A bot gameplay change needed by both products originates where it can be measured, then moves as a reviewed commit.
- `docs/STATE.md` and `.agent/plans/active/` belong to one repository each and must never be copied wholesale between them.
- Stable common documentation may be synchronized only after comparing both versions.

## Cross-repository inspection

Fetch the peer branch before reasoning about it:

```sh
git fetch https://github.com/pstngh/openmohaa.git bots/server-ai
git show FETCH_HEAD:docs/STATE.md
git log --oneline -20 FETCH_HEAD
```

Record the peer tip inspected and any candidate commits in `docs/STATE.md` or the active plan. Do not claim parity from filenames alone.

## Transfer protocol

1. Identify the smallest source commit or intentionally prepared commit series.
2. Inspect `git show --stat --patch <sha>` and verify it contains no server-only infrastructure.
3. Record why the receiving product needs it.
4. Cherry-pick the explicit hash; do not merge the source branch.
5. Resolve receiving-side semantics deliberately, especially telemetry schemas, cvars, CI, and documentation.
6. Run the receiving side's focused tests and build.
7. Record the source hash, resulting hash, validation, and any omissions in both handoffs.

If a change cannot be separated cleanly, prepare a new focused port instead of importing unrelated history.
