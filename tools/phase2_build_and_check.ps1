# The Betrayed Will / وصية الغدر — Phase 2 editor build + log triage
#
# Run this on the Windows machine that has Unreal Engine 5.8.x.
# It does three things and nothing else:
#   1. regenerates Visual Studio project files
#   2. builds TheBetrayedWillEditor Win64 Development
#   3. saves the full log and prints ONLY the errors/warnings that matter
#
# Usage (PowerShell, from the repo root):
#   .\tools\phase2_build_and_check.ps1
#   .\tools\phase2_build_and_check.ps1 -EngineRoot "D:\Epic Games\UE_5.8"
#   .\tools\phase2_build_and_check.ps1 -Clean          # force a full rebuild
#
# Copy the SUMMARY block at the end back into the chat.

param(
    [string]$EngineRoot = $env:UE58_ROOT,
    [switch]$Clean,
    [switch]$SkipGenerate
)

$ErrorActionPreference = "Stop"
$ProjectRoot = (Resolve-Path (Join-Path $PSScriptRoot "..")).Path
$UProject    = Join-Path $ProjectRoot "TheBetrayedWill.uproject"
$LogDir      = Join-Path $ProjectRoot "Saved\Logs"
$Stamp       = Get-Date -Format "yyyyMMdd-HHmmss"
$BuildLog    = Join-Path $LogDir "Phase2_Build_$Stamp.log"

New-Item -ItemType Directory -Force -Path $LogDir | Out-Null

# ---------------------------------------------------------------- engine root
if (-not $EngineRoot) {
    foreach ($Guess in @(
        "C:\Program Files\Epic Games\UE_5.8",
        "D:\Epic Games\UE_5.8",
        "D:\UE_5.8",
        $env:UE_ROOT
    )) {
        if ($Guess -and (Test-Path $Guess)) { $EngineRoot = $Guess; break }
    }
}
if (-not $EngineRoot -or -not (Test-Path $EngineRoot)) {
    Write-Error "Unreal Engine 5.8 not found. Pass -EngineRoot ""C:\path\to\UE_5.8"" or set UE58_ROOT."
}

$BuildBat   = Join-Path $EngineRoot "Engine\Build\BatchFiles\Build.bat"
$VersionH   = Join-Path $EngineRoot "Engine\Source\Runtime\Launch\Resources\Version.h"
$UBTExe     = Join-Path $EngineRoot "Engine\Binaries\DotNET\UnrealBuildTool\UnrealBuildTool.exe"

if (-not (Test-Path $BuildBat)) { Write-Error "Build.bat not found under $EngineRoot" }

# ------------------------------------------------------------- engine version
$EngineVersion = "unknown"
if (Test-Path $VersionH) {
    $vh    = Get-Content $VersionH -Raw
    $major = [regex]::Match($vh, "ENGINE_MAJOR_VERSION\s+(\d+)").Groups[1].Value
    $minor = [regex]::Match($vh, "ENGINE_MINOR_VERSION\s+(\d+)").Groups[1].Value
    $patch = [regex]::Match($vh, "ENGINE_PATCH_VERSION\s+(\d+)").Groups[1].Value
    $EngineVersion = "$major.$minor.$patch"
}

Write-Host ""
Write-Host "=============================================================="
Write-Host " THE BETRAYED WILL - PHASE 2 BUILD CHECK"
Write-Host "=============================================================="
Write-Host " Engine root    : $EngineRoot"
Write-Host " Engine version : $EngineVersion   (project is locked to 5.8)"
Write-Host " Project        : $UProject"
Write-Host " Log            : $BuildLog"
Write-Host ""

if ($EngineVersion -ne "unknown" -and -not $EngineVersion.StartsWith("5.8")) {
    Write-Warning "This engine is $EngineVersion but the project is locked to 5.8. Stop and point -EngineRoot at UE 5.8."
}

# --------------------------------------------------------------------- clean
if ($Clean) {
    Write-Host "[clean] removing Binaries / Intermediate ..."
    foreach ($d in @("Binaries", "Intermediate", "DerivedDataCache")) {
        $p = Join-Path $ProjectRoot $d
        if (Test-Path $p) { Remove-Item $p -Recurse -Force -ErrorAction SilentlyContinue }
    }
}

# ---------------------------------------------------------- project files gen
if (-not $SkipGenerate) {
    Write-Host "[1/2] generating project files ..."
    if (Test-Path $UBTExe) {
        & $UBTExe -projectfiles -project="$UProject" -game -rocket -progress 2>&1 |
            Tee-Object -FilePath $BuildLog
    } else {
        Write-Warning "UnrealBuildTool.exe not found; skipping project file generation."
    }
}

# --------------------------------------------------------------------- build
Write-Host "[2/2] building TheBetrayedWillEditor Win64 Development ..."
$sw = [System.Diagnostics.Stopwatch]::StartNew()
& $BuildBat TheBetrayedWillEditor Win64 Development -Project="$UProject" -WaitMutex -FromMsBuild 2>&1 |
    Tee-Object -FilePath $BuildLog -Append
$BuildExit = $LASTEXITCODE
$sw.Stop()

# ------------------------------------------------------------------- triage
$log      = Get-Content $BuildLog -ErrorAction SilentlyContinue
$errors   = $log | Select-String -Pattern "error [A-Z]+\d+|: error|LNK\d{4}|fatal error" | Select-Object -First 40
$warnings = $log | Select-String -Pattern "warning [A-Z]+\d+|: warning" |
            Where-Object { $_ -notmatch "Microsoft.MakeFile.Targets" } | Select-Object -First 25
$uht      = $log | Select-String -Pattern "UnrealHeaderTool|UHT" | Select-Object -First 10

Write-Host ""
Write-Host "=============================================================="
Write-Host " SUMMARY  (copy everything below into the chat)"
Write-Host "=============================================================="
Write-Host "Engine version : $EngineVersion"
Write-Host "Target         : TheBetrayedWillEditor Win64 Development"
Write-Host "Exit code      : $BuildExit  ->  $(if ($BuildExit -eq 0) { 'BUILD SUCCEEDED' } else { 'BUILD FAILED' })"
Write-Host "Duration       : $([math]::Round($sw.Elapsed.TotalMinutes,1)) min"
Write-Host "Errors found   : $($errors.Count)"
Write-Host "Warnings found : $($warnings.Count)"
Write-Host "Full log       : $BuildLog"
Write-Host ""

if ($errors.Count -gt 0) {
    Write-Host "---- ERRORS -------------------------------------------------"
    $errors | ForEach-Object { Write-Host $_.Line.Trim() }
    Write-Host ""
}
if ($warnings.Count -gt 0) {
    Write-Host "---- WARNINGS (first 25) ------------------------------------"
    $warnings | ForEach-Object { Write-Host $_.Line.Trim() }
    Write-Host ""
}

if ($BuildExit -eq 0) {
    Write-Host "Next: open TheBetrayedWill.uproject in UE 5.8, press Play, and walk"
    Write-Host "through docs\PHASE2_PIE_CHECKLIST.md. Do not start Phase 3 before it passes."
}

exit $BuildExit
