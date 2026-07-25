#!/usr/bin/env python3
"""Fail Release builds when Debug-only sandbox markers appear in artefacts (V1.3)."""

from __future__ import annotations

import argparse
import sys
from pathlib import Path

# Allow `python3 Scripts/release/check_release_hygiene.py` from the repo root.
_SCRIPT_DIR = Path(__file__).resolve().parent
if str(_SCRIPT_DIR) not in sys.path:
    sys.path.insert(0, str(_SCRIPT_DIR))

from prepare_release import (  # noqa: E402
    PLATFORM_ASSETS,
    ArtefactPaths,
    discover_artefact_paths,
)

# ASCII markers that must never appear in Release plugin binaries (U-10 / D-063).
FORBIDDEN_STRINGS: tuple[bytes, ...] = (
    b"TestComponent",
)


def iter_artefact_files(artefacts: ArtefactPaths) -> list[Path]:
    """Flatten VST3 / AU / Standalone files under discovered Release artefact roots."""
    roots: list[Path] = [artefacts.vst3, artefacts.standalone]
    if artefacts.au is not None:
        roots.append(artefacts.au)

    files: list[Path] = []
    for root in roots:
        if root.is_file():
            files.append(root)
        elif root.is_dir():
            files.extend(sorted(p for p in root.rglob("*") if p.is_file()))
    return files


def find_forbidden_in_bytes(data: bytes, needles: tuple[bytes, ...] = FORBIDDEN_STRINGS) -> list[str]:
    """Return human-readable needle labels found in a binary blob."""
    hits: list[str] = []
    for needle in needles:
        if needle in data:
            hits.append(needle.decode("ascii", errors="replace"))
    return hits


def scan_file_for_forbidden(
    path: Path,
    *,
    needles: tuple[bytes, ...] = FORBIDDEN_STRINGS,
) -> list[str]:
    """Scan one file; return forbidden markers present (empty if clean)."""
    try:
        data = path.read_bytes()
    except OSError as exc:
        raise SystemExit(f"Cannot read artefact file: {path} ({exc})") from exc
    return find_forbidden_in_bytes(data, needles)


def check_release_artefacts(
    platform: str,
    *,
    artefacts_root: Path | None = None,
    needles: tuple[bytes, ...] = FORBIDDEN_STRINGS,
) -> list[str]:
    """
    Discover Release artefacts and report hygiene violations.

    Returns a list of violation messages (empty = pass).
    """
    artefacts = discover_artefact_paths(platform, artefacts_root=artefacts_root)
    files = iter_artefact_files(artefacts)
    if not files:
        raise SystemExit(
            "Release hygiene check FAILED — no artefact files found to scan "
            f"(platform={platform}, root={artefacts_root or 'default'})"
        )

    violations: list[str] = []

    for path in files:
        hits = scan_file_for_forbidden(path, needles=needles)
        if hits:
            rel = path
            violations.append(f"{rel}: contains forbidden marker(s): {', '.join(hits)}")

    return violations


def main(argv: list[str] | None = None) -> int:
    parser = argparse.ArgumentParser(
        description=(
            "Scan Release plugin artefacts for Debug-only sandbox markers "
            "(e.g. TestComponent). Fails with exit code 1 if any are found."
        ),
    )
    parser.add_argument("platform", choices=PLATFORM_ASSETS)
    parser.add_argument(
        "--artefacts-root",
        type=Path,
        help="Override Matrix-Control_artefacts/Release root",
    )
    args = parser.parse_args(argv)

    violations = check_release_artefacts(
        args.platform,
        artefacts_root=args.artefacts_root,
    )
    if violations:
        print("Release hygiene check FAILED — Debug sandbox evidence in Release artefacts:")
        for line in violations:
            print(f"  * {line}")
        return 1

    print(f"Release hygiene OK — no forbidden markers in {args.platform} Release artefacts.")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
