# The Betrayed Will — Windows Development package
# Run on a Windows machine with Unreal Engine 5.8.x installed.
# Usage:
#   .\tools\package_win_dev.ps1
#   .\tools\package_win_dev.ps1 -EngineRoot "C:\Program Files\Epic Games\UE_5.8"

param(
    [string]$EngineRoot = $env:UE58_ROOT,
    [ValidateSet("Development", "DebugGame", "Shipping")]
    [string]$Config = "Development"
)

$ErrorActionPreference = "Stop"
$ProjectRoot = Resolve-Path (Join-Path $PSScriptRoot "..")
$UProject = Join-Path $ProjectRoot "TheBetrayedWill.uproject"
$Archive = Join-Path $ProjectRoot "Saved\StagedBuilds\Win64"
$Version = (Get-Content (Join-Path $ProjectRoot "VERSION") -Raw).Trim()

if (-not $EngineRoot) {
    foreach ($Guess in @(
        "C:\Program Files\Epic Games\UE_5.8",
        "D:\Epic Games\UE_5.8",
        $env:UE_ROOT
    )) {
        if ($Guess -and (Test-Path $Guess)) { $EngineRoot = $Guess; break }
    }
}

if (-not $EngineRoot -or -not (Test-Path $EngineRoot)) {
    Write-Error "Set UE58_ROOT or pass -EngineRoot to your Unreal Engine 5.8 install."
}

$RunUAT = Join-Path $EngineRoot "Engine\Build\BatchFiles\RunUAT.bat"
if (-not (Test-Path $RunUAT)) {
    Write-Error "RunUAT.bat not found under $EngineRoot"
}

Write-Host "TBW $Version"
Write-Host "Engine : $EngineRoot"
Write-Host "Config : $Config"
Write-Host "Project: $UProject"

& $RunUAT BuildCookRun `
    -project="$UProject" `
    -noP4 `
    -platform=Win64 `
    -clientconfig=$Config `
    -serverconfig=$Config `
    -build `
    -cook `
    -pak `
    -stage `
    -prereqs `
    -archive `
    -archivedirectory="$Archive"

if ($LASTEXITCODE -ne 0) {
    Write-Error "BuildCookRun failed with $LASTEXITCODE"
}

Write-Host "Staged: $Archive"
Write-Host "Smoke: launch TheBetrayedWill.exe, walk, press E on a test object, then: tbw.Flags.List"
