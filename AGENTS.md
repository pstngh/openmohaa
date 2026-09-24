# Agent continuity

This repository keeps passive agent memory in `.agent/`.

At the start of a fresh session:

1. Read `.agent/GOAL.md`.
2. Inspect `git status --short --branch`, `HEAD`, recent commits, and relevant
   diffs.
3. Read `.agent/STATE.md` and relevant entries in `.agent/DECISIONS.md`.
4. Inspect the project files involved in the active task.
5. Reconcile the notes with Git, code, tests, and authorized runtime evidence;
   reality wins if the notes are stale.
6. Continue from the exact next action.

Before leaving unfinished work, update `STATE.md` with what is actually complete,
what remains, unknowns, and one concrete next action. Update `DECISIONS.md` only
for rationale that code cannot explain. Do not silently rewrite the goal when
work drifts; flag and resolve the conflict. For concurrent work, prefer separate
branches or worktrees.

Continuity maintenance may change only `AGENTS.md` and the Markdown files in
`.agent/`. It must never participate in source, tests, dependencies, scripts,
builds, CI, configuration, infrastructure, deployment, generated files, or
runtime behavior. Add no automation. Deleting the continuity files must have
zero project effect. This restriction does not prevent normal project changes
during a separate, user-authorized development task.
