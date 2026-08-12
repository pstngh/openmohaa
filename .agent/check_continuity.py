#!/usr/bin/env python3
"""Validate the small, branch-local repository continuity contract."""

from __future__ import annotations

import re
import subprocess
import sys
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
REQUIRED = (
    ".agent/GOAL.md",
    "AGENTS.md",
    "docs/PROJECT.md",
    "docs/BRANCHES.md",
    "docs/STATE.md",
    "docs/DECISIONS.md",
    "docs/SERVER_OPERATIONS.md",
)
SECRET_PATTERNS = (
    re.compile(r"-----BEGIN [A-Z ]*PRIVATE KEY-----"),
    re.compile(r"\b(?:ghp|github_pat|sk)-[A-Za-z0-9_-]{16,}\b"),
)


def fail(message: str, errors: list[str]) -> None:
    errors.append(message)


def read_text(relative: str) -> str:
    return (ROOT / relative).read_text(encoding="utf-8")


def frontmatter(text: str, errors: list[str]) -> dict[str, str]:
    lines = text.splitlines()
    if not lines or lines[0].strip() != "---":
        fail("docs/STATE.md must start with simple YAML front matter", errors)
        return {}
    try:
        end = lines.index("---", 1)
    except ValueError:
        fail("docs/STATE.md front matter is not closed", errors)
        return {}

    values: dict[str, str] = {}
    for line in lines[1:end]:
        if not line.strip() or line.lstrip().startswith("#"):
            continue
        if ":" not in line:
            fail(f"invalid front-matter line: {line!r}", errors)
            continue
        key, value = line.split(":", 1)
        values[key.strip()] = value.strip().strip("\"'")
    return values


def git_branch() -> str:
    result = subprocess.run(
        ["git", "branch", "--show-current"],
        cwd=ROOT,
        check=True,
        text=True,
        capture_output=True,
    )
    return result.stdout.strip()


def main() -> int:
    errors: list[str] = []
    warnings: list[str] = []

    for relative in REQUIRED:
        if not (ROOT / relative).is_file():
            fail(f"missing required file: {relative}", errors)

    if errors:
        for error in errors:
            print(f"ERROR: {error}")
        return 1

    state = read_text("docs/STATE.md")
    metadata = frontmatter(state, errors)
    expected = metadata.get("branch", "")
    peer = metadata.get("peer_branch", "")

    if not expected:
        fail("docs/STATE.md front matter is missing branch", errors)
    if not peer:
        fail("docs/STATE.md front matter is missing peer_branch", errors)

    actual = git_branch()
    if actual and expected and actual != expected:
        fail(f"checked-out branch is {actual!r}, expected {expected!r}", errors)
    elif not actual:
        warnings.append(
            "detached HEAD: verify that HEAD was created from the expected branch before editing"
        )

    if state.count("## Next action") != 1:
        fail("docs/STATE.md must contain exactly one '## Next action' heading", errors)

    branches = read_text("docs/BRANCHES.md")
    for branch in (expected, peer):
        if branch and branch not in branches:
            fail(f"docs/BRANCHES.md does not mention {branch}", errors)

    continuity_files = [ROOT / item for item in REQUIRED]
    for path in continuity_files:
        content = path.read_text(encoding="utf-8")
        for pattern in SECRET_PATTERNS:
            if pattern.search(content):
                fail(f"possible credential in {path.relative_to(ROOT)}", errors)

    for warning in warnings:
        print(f"WARNING: {warning}")
    for error in errors:
        print(f"ERROR: {error}")
    if errors:
        return 1

    print(f"Continuity OK: branch={expected}, peer={peer}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
