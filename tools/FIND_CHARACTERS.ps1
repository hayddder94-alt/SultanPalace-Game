# ============================================================================
#  The Betrayed Will / وصية الغدر — what character content does this engine have?
#
#  ADD_CHARACTERS found nothing under Engine\Templates. That usually means the
#  Epic Games Launcher installed the engine WITHOUT the "Templates and Feature
#  Packs" component, which is an optional tick box. This reports exactly what is
#  present so we stop guessing.
#
#  Usage:  .\tools\FIND_CHARACTERS.cmd
# ============================================================================

param([string]$EngineRoot = $env:UE58_ROOT)

$ErrorActionPreference = "Continue"
. (Join-Path $PSScriptRoot "ue58_common.ps1")

Clear-Host
Write-Host ""
Write-Host "============================================================" -ForegroundColor Cyan
Write-Host "   WHAT CHARACTER CONTENT IS INSTALLED?" -ForegroundColor Cyan
Write-Host "============================================================" -ForegroundColor Cyan
Write-Host ""

$EngineRoot = Resolve-UE58Root -Preferred $EngineRoot
if (-not $EngineRoot) {
    Write-Host "Unreal Engine 5.8 not found." -ForegroundColor Red
    exit 1
}
Write-Host " Engine: $EngineRoot"
Write-Host ""

$report = New-Object System.Collections.Generic.List[string]
function Note($text, $colour = "Gray") { Write-Host $text -ForegroundColor $colour; $report.Add($text) }

Note " engine: $EngineRoot"

# ---------------------------------------------------------------- 1 templates
$templates = Join-Path $EngineRoot "Engine\Templates"
if (Test-Path $templates) {
    $dirs = @(Get-ChildItem $templates -Directory -ErrorAction SilentlyContinue)
    Note " Engine\Templates          : EXISTS, $($dirs.Count) template(s)" "Green"
    foreach ($d in ($dirs | Select-Object -First 20)) { Note "     $($d.Name)" }
} else {
    Note " Engine\Templates          : MISSING" "Red"
    Note "     -> the 'Templates and Feature Packs' component was not installed" "Yellow"
}

# ------------------------------------------------------------ 2 feature packs
$packs = Join-Path $EngineRoot "Engine\FeaturePacks"
if (Test-Path $packs) {
    $upacks = @(Get-ChildItem $packs -Filter "*.upack" -ErrorAction SilentlyContinue)
    Note " Engine\FeaturePacks       : EXISTS, $($upacks.Count) pack(s)" "Green"
    foreach ($u in $upacks) {
        Note ("     {0}  ({1:N0} MB)" -f $u.Name, ($u.Length / 1MB))
    }
    if ($upacks.Count -gt 0) {
        Note "     -> the editor's Add Feature Pack button can install these" "Green"
    }
} else {
    Note " Engine\FeaturePacks       : MISSING" "Red"
}

# --------------------------------------------------- 3 any skeletal mesh at all
Write-Host ""
Write-Host " Searching the engine for skeletal meshes (may take a minute) ..." -ForegroundColor DarkGray
$skm = @(Get-ChildItem -Path (Join-Path $EngineRoot "Engine") -Include "SKM_*.uasset", "SK_*.uasset" `
            -Recurse -File -ErrorAction SilentlyContinue | Select-Object -First 40)
Note ""
Note " skeletal meshes found     : $($skm.Count)"
foreach ($m in $skm) {
    Note "     $($m.FullName.Replace($EngineRoot, '<engine>'))"
}

# --------------------------------------------------------- 4 project side
$projChars = Join-Path (Resolve-Path (Join-Path $PSScriptRoot "..")) "Content\Characters"
Note ""
if (Test-Path $projChars) {
    $n = @(Get-ChildItem $projChars -Recurse -File -ErrorAction SilentlyContinue).Count
    Note " project Content\Characters: EXISTS, $n file(s)" "Green"
} else {
    Note " project Content\Characters: not present yet"
}

# --------------------------------------------------------------- verdict
Write-Host ""
Write-Host "============================================================" -ForegroundColor Cyan
if (Test-Path $templates) {
    Write-Host "   Templates are installed - ADD_CHARACTERS should work." -ForegroundColor Green
    Write-Host "   If it still fails, send this report." -ForegroundColor Green
} else {
    Write-Host "   The engine was installed without templates." -ForegroundColor Yellow
    Write-Host ""
    Write-Host "   Fix it once, in Epic Games Launcher:" -ForegroundColor White
    Write-Host "     Library > next to Unreal Engine 5.8 click the down arrow > Options"
    Write-Host "     tick 'Templates and Feature Packs'  >  Apply"
    Write-Host ""
    Write-Host "   That downloads a few hundred MB and needs no project changes."
    Write-Host "   Then run:  .\tools\ADD_CHARACTERS.cmd"
}
Write-Host "============================================================" -ForegroundColor Cyan
Write-Host ""

try {
    Set-Clipboard -Value ($report -join "`r`n")
    Write-Host " Report copied to your clipboard - press Ctrl+V in the chat." -ForegroundColor Green
} catch { }
Write-Host ""
exit 0
