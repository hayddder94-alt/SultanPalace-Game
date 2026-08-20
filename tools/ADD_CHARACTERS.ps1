# ============================================================================
#  The Betrayed Will / وصية الغدر — give the game real character bodies
#
#  The Third Person feature pack is not a download. Its content already sits on
#  disk inside the engine, under Engine\Templates. "Add Feature or Content Pack"
#  in the editor simply copies it into the project - so this does the same thing
#  without the menus.
#
#  No account, no download, no licence question: it is engine content, covered by
#  the same Unreal Engine EULA the project already runs under.
#
#  Usage:
#    .\tools\ADD_CHARACTERS.cmd
#    .\tools\ADD_CHARACTERS.cmd -Force      overwrite an existing copy
# ============================================================================

param(
    [string]$EngineRoot = $env:UE58_ROOT,
    [string]$Source = "",
    [switch]$Force,
    [switch]$SkipVerify
)

$ErrorActionPreference = "Continue"
. (Join-Path $PSScriptRoot "ue58_common.ps1")

$ProjectRoot   = (Resolve-Path (Join-Path $PSScriptRoot "..")).Path
$ProjectContent = Join-Path $ProjectRoot "Content"

Clear-Host
Write-Host ""
Write-Host "============================================================" -ForegroundColor Cyan
Write-Host "   ADDING CHARACTER BODIES" -ForegroundColor Cyan
Write-Host "============================================================" -ForegroundColor Cyan
Write-Host ""

$EngineRoot = Resolve-UE58Root -Preferred $EngineRoot
if (-not $EngineRoot) {
    Write-Host "Unreal Engine 5.8 not found. Run .\tools\find_ue58.cmd first." -ForegroundColor Red
    exit 1
}
Write-Host " Engine  : $EngineRoot"
Write-Host " Project : $ProjectContent"
Write-Host ""

# ---------------------------------------------------------------- find source
# Ordered by preference: the Blueprint third person template ships the full
# Manny/Quinn set with animations and an AnimBP, which is what the pawn looks for.
if ($Source) {
    if (-not (Test-Path $Source)) {
        Write-Host " -Source path does not exist: $Source" -ForegroundColor Red
        exit 1
    }
    $candidates = @($Source)
} else {
$candidates = @(
    "Engine\Templates\TP_ThirdPersonBP\Content\Characters",
    "Engine\Templates\TP_ThirdPerson\Content\Characters",
    "Engine\Templates\TP_FirstPersonBP\Content\Characters",
    "Templates\TP_ThirdPersonBP\Content\Characters"
) | ForEach-Object { Join-Path $EngineRoot $_ } | Where-Object { Test-Path $_ }

if (-not $candidates -or $candidates.Count -eq 0) {
    Write-Host " Not in the usual places. Searching the whole engine folder ..." -ForegroundColor Yellow
    # Some installs put templates elsewhere, or omit them entirely. Look for the
    # folder that actually holds a mannequin, wherever it is.
    $found = Get-ChildItem -Path $EngineRoot -Directory -Recurse -Filter "Mannequins" `
                -ErrorAction SilentlyContinue | Select-Object -First 1
    if ($found) {
        $candidates = @($found.Parent.FullName)
    } else {
        # Last resort: any folder containing a skeletal mesh asset.
        $mesh = Get-ChildItem -Path $EngineRoot -Include "SKM_*.uasset" -Recurse -File `
                    -ErrorAction SilentlyContinue | Select-Object -First 1
        if ($mesh) { $candidates = @($mesh.Directory.Parent.FullName) }
    }
}
}

if (-not $candidates -or $candidates.Count -eq 0) {
    Write-Host ""
    Write-Host " No character content exists in this engine install." -ForegroundColor Red
    Write-Host ""
    Write-Host " Almost certainly the engine was installed without the optional" -ForegroundColor Yellow
    Write-Host " 'Templates and Feature Packs' component. Fix it once:" -ForegroundColor Yellow
    Write-Host ""
    Write-Host "   Epic Games Launcher > Library > down-arrow next to UE 5.8 >"
    Write-Host "   Options > tick 'Templates and Feature Packs' > Apply"
    Write-Host ""
    Write-Host " Then run this again. To see exactly what is installed now, run:"
    Write-Host "   .\tools\FIND_CHARACTERS.cmd"
    Write-Host ""
    Write-Host " Nothing is blocked by this: the palace, the cast and the dialogue"
    Write-Host " all work without it. Only the body stays a placeholder."
    exit 1
}

$source = $candidates[0]
$dest = Join-Path $ProjectContent "Characters"
Write-Host " Source  : $source" -ForegroundColor Green
Write-Host " Target  : $dest"
Write-Host ""

if ((Test-Path $dest) -and -not $Force) {
    Write-Host " Characters folder already exists - nothing copied." -ForegroundColor Yellow
    Write-Host " Pass -Force to overwrite it."
} else {
    Write-Host " Copying ..." -NoNewline
    New-Item -ItemType Directory -Force -Path $dest | Out-Null
    Copy-Item -Path (Join-Path $source "*") -Destination $dest -Recurse -Force
    Write-Host " done." -ForegroundColor Green
}

# ---------------------------------------------------------------- report
$meshes = @(Get-ChildItem -Path $dest -Filter "SKM_*.uasset" -Recurse -ErrorAction SilentlyContinue)
$anims  = @(Get-ChildItem -Path $dest -Filter "ABP_*.uasset" -Recurse -ErrorAction SilentlyContinue)
$total  = @(Get-ChildItem -Path $dest -Recurse -File -ErrorAction SilentlyContinue)
$sizeMb = [math]::Round((($total | Measure-Object Length -Sum).Sum / 1MB), 1)

Write-Host ""
Write-Host " Files copied     : $($total.Count)  ($sizeMb MB)"
<#
    Print the /Game/... PACKAGE PATH, not just the file name.

    The first run of this reported "Anim blueprints : 2  ABP_Manny,
    ABP_MannyExtended" and that told nobody whether the loader in
    TBWPlayerCharacter.cpp could actually reach them - the C++ candidate list is
    written in package paths, and a name on its own cannot be compared against
    one. A skeletal mesh with no anim instance T-poses and slides, so the
    difference matters more than it looks.
#>
function Show-Assets {
    param([string]$Label, $Items, [string]$ContentRoot)
    Write-Host " $Label : $($Items.Count)"
    foreach ($i in $Items) {
        $rel = $i.FullName.Substring($ContentRoot.Length).TrimStart('\')
        $pkg = "/Game/" + ($rel -replace '\\', '/') -replace '\.uasset$', ''
        Write-Host "     ${pkg}.$($i.BaseName)"
    }
}

Show-Assets -Label "Skeletal meshes " -Items $meshes -ContentRoot $ProjectContent
Show-Assets -Label "Anim blueprints " -Items $anims  -ContentRoot $ProjectContent
Write-Host ""

if ($meshes.Count -eq 0) {
    Write-Host " No SKM_* mesh found. The pawn will keep the placeholder cube." -ForegroundColor Yellow
    Write-Host " Send this output and I will point the loader at whatever is there."
    exit 1
}

Write-Host " These are engine template assets, licensed under the same Unreal" -ForegroundColor DarkGray
Write-Host " Engine EULA as the project. They stay OUT of git - .gitignore already" -ForegroundColor DarkGray
Write-Host " excludes third-party art, and each machine copies its own." -ForegroundColor DarkGray
Write-Host ""

if ($SkipVerify) { exit 0 }

Write-Host " Verifying that the pawn picks them up ..." -ForegroundColor White
Write-Host ""

$out = & powershell -NoProfile -ExecutionPolicy Bypass -File (Join-Path $PSScriptRoot "selftest.ps1") `
        -EngineRoot $EngineRoot 2>&1
$out | Where-Object { $_ -match "body:|SELFTEST RESULT|VERDICT" } | ForEach-Object { Write-Host "   $_" }

$body = ($out | Where-Object { $_ -match "body:" }) -join ""
Write-Host ""
if ($body -match "skeletal") {
    Write-Host " The player is now a real character, not a cube." -ForegroundColor Green
} else {
    Write-Host " Still reporting a placeholder cube. Send the output above." -ForegroundColor Yellow
}

Copy-TBWReport -Title "Character import" `
    -Text (($out | Where-Object { $_ -match "body:|PASS |FAIL |INFO |SELFTEST RESULT|VERDICT" }) -join "`r`n") | Out-Null
Write-Host ""
exit 0
