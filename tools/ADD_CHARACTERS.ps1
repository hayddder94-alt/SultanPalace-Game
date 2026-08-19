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
$candidates = @(
    "Engine\Templates\TP_ThirdPersonBP\Content\Characters",
    "Engine\Templates\TP_ThirdPerson\Content\Characters",
    "Engine\Templates\TP_FirstPersonBP\Content\Characters",
    "Templates\TP_ThirdPersonBP\Content\Characters"
) | ForEach-Object { Join-Path $EngineRoot $_ } | Where-Object { Test-Path $_ }

if (-not $candidates -or $candidates.Count -eq 0) {
    Write-Host " No template character content found under the engine." -ForegroundColor Yellow
    Write-Host " Searching more widely (this can take a moment) ..."
    $found = Get-ChildItem -Path (Join-Path $EngineRoot "Engine\Templates") -Directory -Recurse `
                -Filter "Mannequins" -ErrorAction SilentlyContinue | Select-Object -First 1
    if ($found) { $candidates = @($found.Parent.FullName) }
}

if (-not $candidates -or $candidates.Count -eq 0) {
    Write-Host ""
    Write-Host " Could not find the mannequin content in this engine install." -ForegroundColor Red
    Write-Host " Fall back to the editor: Content Browser > Add > Add Feature or"
    Write-Host " Content Pack > Blueprint Feature > Third Person > Add."
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
Write-Host " Skeletal meshes  : $($meshes.Count)"
foreach ($m in $meshes) { Write-Host "     $($m.BaseName)" }
Write-Host " Anim blueprints  : $($anims.Count)"
foreach ($a in $anims) { Write-Host "     $($a.BaseName)" }
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

try {
    Set-Clipboard -Value (($out | Where-Object { $_ -match "body:|PASS |FAIL |INFO |SELFTEST RESULT|VERDICT" }) -join "`r`n")
    Write-Host " Result copied to your clipboard - press Ctrl+V in the chat."
} catch { }
Write-Host ""
exit 0
