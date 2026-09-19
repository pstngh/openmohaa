#!/usr/bin/env python3
"""Build a byte-for-byte reproducible PK3 containing the editable HUD layouts."""

from __future__ import annotations

import argparse
import hashlib
from pathlib import Path
import zipfile


ROOT = Path(__file__).resolve().parent
UI_DIR = ROOT / "ui"
FIXED_TIMESTAMP = (1980, 1, 1, 0, 0, 0)


def build(output: Path) -> str:
    layouts = sorted(UI_DIR.glob("*.urc"), key=lambda path: path.name.casefold())
    if not layouts:
        raise SystemExit(f"no HUD layouts found under {UI_DIR}")

    output.parent.mkdir(parents=True, exist_ok=True)
    # Entries are stored uncompressed: DEFLATE output can differ between zlib
    # builds, and the layouts are a few kilobytes of text.
    with zipfile.ZipFile(output, "w") as archive:
        for layout in layouts:
            info = zipfile.ZipInfo(f"ui/{layout.name}", FIXED_TIMESTAMP)
            info.compress_type = zipfile.ZIP_STORED
            info.create_system = 3
            info.external_attr = 0o100644 << 16
            archive.writestr(info, layout.read_bytes())

    digest = hashlib.sha256(output.read_bytes()).hexdigest()
    return digest


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument(
        "--output", type=Path, required=True, help="output PK3 path"
    )
    args = parser.parse_args()
    output = args.output.resolve()
    digest = build(output)
    print(f"built {output}")
    print(f"sha256 {digest}")


if __name__ == "__main__":
    main()
