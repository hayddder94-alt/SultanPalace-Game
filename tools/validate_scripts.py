#!/usr/bin/env python3
"""
Lint the Windows tooling. There is no PowerShell on this host, so this does not
parse PowerShell - it catches the specific, repeatable ways our .ps1 files have
actually broken, each of which cost a round trip with the user to discover.

  S1  a block-comment terminator inside the body of a block comment

      PowerShell ends the comment at the FIRST '#>'. A note that mentioned
      '<# #>' in prose therefore closed its own comment three lines early, and
      PowerShell executed the remaining English as commands:
          the : The term 'the' is not recognized ...
      Do not write the terminator inside a comment. Say "block comment".

  S2  an unterminated block comment (depth never returns to zero)

  S3  PowerShell 7 only cmdlets in scripts the user runs under Windows
      PowerShell 5.1, which is what `powershell` resolves to on this machine.

Exit 0 = clean, 1 = at least one error.
"""

from __future__ import annotations

import re
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
TOOLS = ROOT / "tools"

# Cmdlets that only exist in PowerShell 7+. The .cmd wrappers all invoke
# `powershell`, which is Windows PowerShell 5.1, never `pwsh`.
PS7_ONLY = ["Join-String", "ConvertFrom-Markdown", "Test-Json", "Get-Uptime"]

errors: list[str] = []


def strip_strings(line: str) -> str:
    """Blank out "..." and '...' so tokens inside literals are not counted."""
    out = []
    i, n = 0, len(line)
    quote = None
    while i < n:
        c = line[i]
        if quote:
            if c == quote:
                quote = None
            out.append(" ")
        elif c in "\"'":
            quote = c
            out.append(" ")
        else:
            out.append(c)
        i += 1
    return "".join(out)


def check(path: Path) -> None:
    rel = path.relative_to(ROOT)
    depth = 0
    for lineno, raw in enumerate(path.read_text(encoding="utf-8").splitlines(), 1):
        # Inside a comment, string literals do not exist - the text is text.
        line = raw if depth > 0 else strip_strings(raw)
        stripped = line.strip()

        if depth > 0 and "#>" in line and not stripped.startswith("#>"):
            errors.append(
                f"[S1] {rel}:{lineno} block-comment terminator inside the comment body "
                f"- PowerShell will end the comment here and run the rest as code:\n"
                f"        {raw.strip()}")

        depth += line.count("<#")
        depth -= line.count("#>")
        if depth < 0:
            depth = 0

        for cmdlet in PS7_ONLY:
            if depth == 0 and re.search(rf"\b{re.escape(cmdlet)}\b", line):
                errors.append(
                    f"[S3] {rel}:{lineno} {cmdlet} is PowerShell 7 only; this machine "
                    f"runs Windows PowerShell 5.1")

    if depth > 0:
        errors.append(f"[S2] {rel}: block comment never closed ({depth} open)")


def main() -> int:
    files = sorted(TOOLS.rglob("*.ps1"))
    for f in files:
        check(f)

    print("=" * 66)
    print("WINDOWS SCRIPT LINT")
    print("=" * 66)
    print(f"  scripts checked  {len(files)}")
    print()
    for e in errors:
        print(f"  X  {e}")
    print()
    print("RESULT: FAIL" if errors else "RESULT: PASS")
    return 1 if errors else 0


if __name__ == "__main__":
    sys.exit(main())
