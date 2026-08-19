#!/usr/bin/env python3
"""
The Betrayed Will / وصية الغدر — offline pre-compile audit (UE 5.8 only).

This host has no Unreal toolchain, so this script does NOT compile anything.
It catches the classes of mistake that would otherwise only surface after a
20-minute UBT build on the production laptop:

  E1  member function declared in a header but never defined  -> LNK2019
  E2  UCLASS / USTRUCT / UINTERFACE without GENERATED_BODY    -> UHT error
  E3  *.generated.h is not the last include                   -> UHT error
  E4  raw non-ASCII bytes inside TEXT("...")                  -> MSVC CP1252 mojibake
  E5  file is not valid UTF-8
  E6  more than one .uproject, or EngineAssociation != 5.8
  E7  module used in an #include but missing from TBW.Build.cs
  E8  #include of a project header that no module include path can reach -> C1083
  E9  a known UE API called with the wrong argument form -> C2665 and friends

  W1  private helper defined but never called                 -> dead feature
  W2  known deprecated API still in use                       -> tracked debt

Exit code 0 = no errors (warnings allowed), 1 = at least one error.
"""

from __future__ import annotations

import json
import re
import sys
from pathlib import Path, PurePosixPath

ROOT = Path(__file__).resolve().parents[1]
SRC = ROOT / "Source"

ERRORS: list[str] = []
WARNINGS: list[str] = []
NOTES: list[str] = []


def err(code: str, msg: str) -> None:
    ERRORS.append(f"[{code}] {msg}")


def warn(code: str, msg: str) -> None:
    WARNINGS.append(f"[{code}] {msg}")


# --------------------------------------------------------------------------
# helpers
# --------------------------------------------------------------------------

def strip_comments_and_strings(text: str) -> str:
    """Blank out // ... , /* ... */ and "..." so regexes do not match inside them."""
    out = []
    i, n = 0, len(text)
    while i < n:
        c = text[i]
        if c == "/" and i + 1 < n and text[i + 1] == "/":
            j = text.find("\n", i)
            j = n if j < 0 else j
            out.append(" " * (j - i))
            i = j
        elif c == "/" and i + 1 < n and text[i + 1] == "*":
            j = text.find("*/", i + 2)
            j = n if j < 0 else j + 2
            out.append(re.sub(r"[^\n]", " ", text[i:j]))
            i = j
        elif c == '"':
            j = i + 1
            while j < n:
                if text[j] == "\\":
                    j += 2
                    continue
                if text[j] == '"':
                    j += 1
                    break
                j += 1
            out.append(" " * (j - i))
            i = j
        else:
            out.append(c)
            i += 1
    return "".join(out)


def source_files(suffixes: tuple[str, ...]) -> list[Path]:
    return sorted(p for p in SRC.rglob("*") if p.suffix in suffixes)


# --------------------------------------------------------------------------
# E5 / E4 — encoding
# --------------------------------------------------------------------------

def check_encoding() -> None:
    for path in source_files((".h", ".cpp", ".cs")):
        raw = path.read_bytes()
        try:
            text = raw.decode("utf-8")
        except UnicodeDecodeError as exc:
            err("E5", f"{path.relative_to(ROOT)} is not valid UTF-8 ({exc})")
            continue

        for lineno, line in enumerate(text.splitlines(), 1):
            for match in re.finditer(r'TEXT\(\s*"((?:[^"\\]|\\.)*)"', line):
                literal = match.group(1)
                if any(ord(ch) > 127 for ch in literal):
                    err(
                        "E4",
                        f"{path.relative_to(ROOT)}:{lineno} raw non-ASCII inside TEXT(\"...\") — "
                        "MSVC may decode it as CP1252. Use \\uXXXX escapes.",
                    )


# --------------------------------------------------------------------------
# E2 / E3 — UHT structural rules
# --------------------------------------------------------------------------

def check_uht_rules() -> None:
    for path in source_files((".h",)):
        text = path.read_text(encoding="utf-8")
        code = strip_comments_and_strings(text)
        raw_no_comments = re.sub(r"//[^\n]*", "", text)
        rel = path.relative_to(ROOT)

        macros = list(re.finditer(r"^\s*(UCLASS|USTRUCT|UINTERFACE|UENUM)\s*\(", code, re.M))
        for m in macros:
            if m.group(1) == "UENUM":
                continue
            tail = code[m.end(): m.end() + 900]
            if "GENERATED_BODY()" not in tail and "GENERATED_UCLASS_BODY()" not in tail:
                line = code[: m.start()].count("\n") + 1
                err("E2", f"{rel}:{line} {m.group(1)} without GENERATED_BODY()")

        includes = re.findall(r'^\s*#include\s+"([^"]+)"', raw_no_comments, re.M)
        gen = [i for i in includes if i.endswith(".generated.h")]
        if macros and not gen:
            err("E2", f"{rel} declares UHT macros but never includes its .generated.h")
        if gen and includes[-1] != gen[-1]:
            err("E3", f"{rel} .generated.h must be the LAST include (found '{includes[-1]}' after it)")


# --------------------------------------------------------------------------
# E1 / W1 — declared vs defined member functions
# --------------------------------------------------------------------------

DECL_RE = re.compile(
    r"""^[ \t]*
        (?P<virtual>virtual\s+)?
        (?P<static>static\s+)?
        (?:explicit\s+|inline\s+|FORCEINLINE\s+|constexpr\s+)*
        (?P<ret>[A-Za-z_][\w:<>,\s\*&\.]*?[\s\*&]+)?
        (?P<name>[A-Za-z_]\w*)
        \s*\((?P<args>[^;{}]*)\)
        \s*(?P<const>const\s*)?
        (?P<tail>(?:override\s*|final\s*)*)
        ;
    """,
    re.X,
)

SKIP_NAMES = {
    "if", "for", "while", "switch", "return", "sizeof", "static_assert",
    "GENERATED_BODY", "GENERATED_UCLASS_BODY", "UPROPERTY", "UFUNCTION",
    "UCLASS", "USTRUCT", "UINTERFACE", "UENUM", "TEXT", "NSLOCTEXT",
    "DECLARE_LOG_CATEGORY_EXTERN",
}

# UHT writes the thunk for these; C++ only supplies Name_Implementation.
UHT_EVENT_META = ("BlueprintNativeEvent", "BlueprintImplementableEvent")


class Decl:
    __slots__ = ("cls", "name", "line", "path", "is_virtual", "ufunction", "uht_event", "access")

    def __init__(self, cls, name, line, path, is_virtual, ufunction, uht_event, access):
        self.cls = cls
        self.name = name
        self.line = line
        self.path = path
        self.is_virtual = is_virtual
        self.ufunction = ufunction
        self.uht_event = uht_event
        self.access = access


def collect_class_decls() -> list[Decl]:
    decls: list[Decl] = []
    for path in source_files((".h",)):
        code = strip_comments_and_strings(path.read_text(encoding="utf-8"))
        lines = code.splitlines()
        current = None
        depth = 0
        depth_at_class = None
        access = "private"
        pending_ufunction = None
        for lineno, line in enumerate(lines, 1):
            stripped = line.strip()

            cls = re.match(r"(?:class|struct)\s+(?:\w+_API\s+)?([A-Za-z_]\w*)\s*(?::|$|\{)", stripped)
            if cls and not stripped.endswith(";"):
                current = cls.group(1)
                depth_at_class = depth
                access = "private" if stripped.startswith("class") else "public"

            if re.match(r"(public|protected|private)\s*:", stripped):
                access = stripped.split(":")[0].strip()

            if stripped.startswith("UFUNCTION"):
                pending_ufunction = stripped
            elif current and stripped and not stripped.startswith(("UPROPERTY", "//", "*", "/*")):
                m = DECL_RE.match(line)
                if m:
                    name = m.group("name")
                    ret = m.group("ret")
                    is_ctor = (name == current and ret is None)
                    if name not in SKIP_NAMES and not name.startswith("~") and (ret or is_ctor):
                        uht_event = bool(pending_ufunction and any(k in pending_ufunction for k in UHT_EVENT_META))
                        decls.append(Decl(
                            cls=current,
                            name=name,
                            line=lineno,
                            path=path,
                            is_virtual=bool(m.group("virtual")) or "override" in (m.group("tail") or ""),
                            ufunction=bool(pending_ufunction),
                            uht_event=uht_event,
                            access=access,
                        ))
                if not stripped.startswith(")"):
                    pending_ufunction = None

            depth += line.count("{") - line.count("}")
            if current and depth_at_class is not None and depth <= depth_at_class and "}" in line:
                current = None
                depth_at_class = None
    return decls


def check_definitions() -> None:
    decls = collect_class_decls()
    cpp_code = strip_comments_and_strings("\n".join(p.read_text(encoding="utf-8") for p in source_files((".cpp",))))
    header_code = strip_comments_and_strings("\n".join(p.read_text(encoding="utf-8") for p in source_files((".h",))))
    all_code = cpp_code + "\n" + header_code

    for d in decls:
        if d.uht_event:
            continue  # UHT generates the thunk; only *_Implementation must exist
        pattern = re.compile(rf"\b{re.escape(d.cls)}\s*::\s*{re.escape(d.name)}\s*\(")
        if pattern.search(all_code):
            continue
        err(
            "E1",
            f"{d.path.relative_to(ROOT)}:{d.line} {d.cls}::{d.name}() declared but never defined "
            "-> unresolved external symbol at link time",
        )

    # BlueprintNativeEvent declared on an interface must have an _Implementation on every implementer.
    for d in decls:
        if not d.uht_event:
            continue
        impl = re.compile(rf"::{re.escape(d.name)}_Implementation\s*\(")
        if not impl.search(all_code):
            warn("W3", f"{d.cls}::{d.name}() is a BlueprintNativeEvent with no C++ _Implementation anywhere")

    # W1 — something declared+defined that nothing ever references.
    for d in decls:
        if d.is_virtual or d.ufunction or d.name == d.cls:
            continue
        refs = len(re.findall(rf"\b{re.escape(d.name)}\b", all_code))
        decl_and_def = 2  # the header declaration + the qualified definition
        if refs - decl_and_def <= 0:
            warn(
                "W1",
                f"{d.cls}::{d.name}() is defined but nothing calls it "
                f"({d.path.relative_to(ROOT)}:{d.line}) — dead code, or a step that was written and never wired up",
            )



# --------------------------------------------------------------------------
# E8 — project includes must resolve the way UBT resolves them
# --------------------------------------------------------------------------

def check_include_resolution() -> None:
    """
    UBT exposes a module's Public/ and Private/ folders as include roots (plus the
    directory of the including file). The module root itself is NOT an include root
    once Public/Private exist. An #include that only resolves relative to the module
    root compiles nowhere and fails as:
        fatal error C1083: Cannot open include file
    """
    module = SRC / "TBW"
    public = module / "Public"
    private = module / "Private"

    project_headers: dict[str, list[Path]] = {}
    for h in module.rglob("*.h"):
        project_headers.setdefault(h.name.lower(), []).append(h)

    for path in source_files((".h", ".cpp")):
        text = re.sub(r"//[^\n]*", "", path.read_text(encoding="utf-8"))
        for lineno, line in enumerate(text.splitlines(), 1):
            m = re.match(r'\s*#include\s+"([^"]+)"', line)
            if not m:
                continue
            inc = m.group(1)
            if inc.endswith(".generated.h"):
                continue

            roots = [public, private, path.parent]
            if any((root / inc).is_file() for root in roots):
                continue

            # Not reachable. Is it one of ours (a real bug) or an engine header?
            owners = project_headers.get(PurePosixPath(inc).name.lower(), [])
            if owners:
                where = ", ".join(str(o.relative_to(ROOT)) for o in owners)
                err(
                    "E8",
                    f"{path.relative_to(ROOT)}:{lineno} #include \"{inc}\" cannot be resolved from "
                    f"Public/, Private/ or the file's own folder. The header exists at {where} "
                    "-> fatal error C1083 at compile time.",
                )



# --------------------------------------------------------------------------
# E9 — UE API call shapes that the compiler rejects
# --------------------------------------------------------------------------

# Each entry: (name, regex, why). These are mistakes that read as correct C++ but
# do not match the engine's signature, so only a compiler catches them - unless
# they are written down here after the first time they cost a build.
API_SHAPES = [
    (
        "TActorIterator takes a const UWorld* pointer, not a dereferenced world",
        re.compile(r"TActorIterator\s*<[^>]+>\s*\w+\s*\(\s*\*"),
        "error C2665: no overloaded function could convert all the argument types",
    ),
    (
        "GetSubsystem is called on a pointer, not on a dereferenced object",
        re.compile(r"\(\s*\*\s*\w+\s*\)\s*->\s*GetSubsystem"),
        "dereferencing then arrow is a type error",
    ),
    (
        "SpawnActor needs a class or template argument",
        re.compile(r"SpawnActor\s*\(\s*\)"),
        "SpawnActor with no arguments does not compile",
    ),
]


def check_api_shapes() -> None:
    for path in source_files((".cpp", ".h")):
        code = strip_comments_and_strings(path.read_text(encoding="utf-8"))
        for lineno, line in enumerate(code.splitlines(), 1):
            for name, pattern, why in API_SHAPES:
                if pattern.search(line):
                    err("E9", f"{path.relative_to(ROOT)}:{lineno} {name} -> {why}")


# --------------------------------------------------------------------------
# E6 — project / engine lock
# --------------------------------------------------------------------------

def check_project_lock() -> None:
    projects = sorted(ROOT.glob("*.uproject")) + sorted(
        p for p in ROOT.rglob("*.uproject") if p.parent != ROOT
    )
    if len(projects) != 1:
        err("E6", f"expected exactly one .uproject in the repository, found {len(projects)}: {projects}")
        return
    data = json.loads(projects[0].read_text(encoding="utf-8"))
    if data.get("EngineAssociation") != "5.8":
        err("E6", f"EngineAssociation is {data.get('EngineAssociation')!r}, must be '5.8'")
    NOTES.append(f"project: {projects[0].name}  EngineAssociation={data.get('EngineAssociation')}")

    for target in ("TheBetrayedWill.Target.cs", "TheBetrayedWillEditor.Target.cs"):
        text = (SRC / target).read_text(encoding="utf-8")
        if "EngineIncludeOrderVersion.Unreal5_8" not in text:
            err("E6", f"{target} does not pin EngineIncludeOrderVersion.Unreal5_8")


# --------------------------------------------------------------------------
# E7 — Build.cs dependencies
# --------------------------------------------------------------------------

INCLUDE_TO_MODULE = {
    "EnhancedInput": "EnhancedInput",
    "EnhancedInputComponent.h": "EnhancedInput",
    "EnhancedInputSubsystems.h": "EnhancedInput",
    "InputAction.h": "EnhancedInput",
    "InputMappingContext.h": "EnhancedInput",
    "Widgets/": "Slate",
    "Styling/": "SlateCore",
    "Fonts/": "SlateCore",
    "Blueprint/UserWidget.h": "UMG",
    "GameplayTagContainer.h": "GameplayTags",
    "NavigationSystem.h": "NavigationSystem",
    "LevelSequence": "LevelSequence",
    "CineCameraActor.h": "CinematicCamera",
}


def check_build_deps() -> None:
    build = (SRC / "TBW" / "TBW.Build.cs").read_text(encoding="utf-8")
    declared = set(re.findall(r'"([A-Za-z][\w]*)"', build))
    for path in source_files((".h", ".cpp")):
        for inc in re.findall(r'^\s*#include\s+"([^"]+)"', path.read_text(encoding="utf-8"), re.M):
            for needle, module in INCLUDE_TO_MODULE.items():
                if inc.startswith(needle) or inc == needle:
                    if module not in declared:
                        err("E7", f"{path.relative_to(ROOT)} includes '{inc}' but TBW.Build.cs lacks module '{module}'")


# --------------------------------------------------------------------------
# W2 — tracked deprecations
# --------------------------------------------------------------------------

DEPRECATED = {
    r"FSlateFontInfo\s*\(\s*[A-Za-z_]": "FSlateFontInfo(FString) ctor is deprecated in UE5 — accepted technical debt, Arabic HUD works",
}


def check_deprecations() -> None:
    for path in source_files((".cpp", ".h")):
        text = path.read_text(encoding="utf-8")
        for lineno, line in enumerate(text.splitlines(), 1):
            for pattern, note in DEPRECATED.items():
                if re.search(pattern, line):
                    warn("W2", f"{path.relative_to(ROOT)}:{lineno} {note}")


# --------------------------------------------------------------------------

def main() -> int:
    check_project_lock()
    check_encoding()
    check_uht_rules()
    check_definitions()
    check_build_deps()
    check_include_resolution()
    check_api_shapes()
    check_deprecations()

    print("=" * 72)
    print("THE BETRAYED WILL — PHASE 2 OFFLINE PRE-COMPILE AUDIT (UE 5.8)")
    print("=" * 72)
    for note in NOTES:
        print(f"  .  {note}")
    print(f"  .  scanned {len(source_files(('.h', '.cpp')))} C++ files")
    print()

    if ERRORS:
        print(f"ERRORS ({len(ERRORS)}) — these WILL break the build:")
        for e in ERRORS:
            print(f"  X  {e}")
        print()
    if WARNINGS:
        print(f"WARNINGS ({len(WARNINGS)}):")
        for w in WARNINGS:
            print(f"  !  {w}")
        print()

    if ERRORS:
        print("RESULT: FAIL")
        return 1
    print("RESULT: PASS (structural only — a real UE 5.8 build on Windows is still required)")
    return 0


if __name__ == "__main__":
    sys.exit(main())
