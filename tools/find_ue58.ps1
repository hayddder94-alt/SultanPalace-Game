# The Betrayed Will / وصية الغدر — where is Unreal Engine 5.8?
#
# Run this when the build script says "Unreal Engine 5.8 not found".
# It only looks and reports. It changes nothing except, optionally, a permanent
# UE58_ROOT environment variable for your user account when you pass -Remember.
#
# Usage:
#   .\tools\find_ue58.cmd
#   .\tools\find_ue58.cmd -Remember

param([switch]$Remember)

$ErrorActionPreference = "Stop"
. (Join-Path $PSScriptRoot "ue58_common.ps1")

Write-Host ""
Write-Host "=============================================================="
Write-Host " UNREAL ENGINE DISCOVERY"
Write-Host "=============================================================="
Write-Host ""

Show-UnrealEngines
Write-Host ""

$root = Resolve-UE58Root
if (-not $root) {
    Write-Host " RESULT: no Unreal Engine 5.8 found." -ForegroundColor Red
    Write-Host ""
    Write-Host " If UE 5.8 IS installed, find its folder yourself - it is the one that"
    Write-Host " contains Engine\Build\BatchFiles\Build.bat - then build with:"
    Write-Host ""
    Write-Host '   .\tools\build_phase2.cmd -EngineRoot "D:\your\path\UE_5.8"'
    Write-Host ""
    Write-Host " If it is NOT installed: Epic Games Launcher > Unreal Engine > Library >"
    Write-Host " '+' > choose 5.8, and tick 'Editor symbols for debugging' only if you"
    Write-Host " have the disk space. A 5.8 install needs roughly 60 GB free."
    Write-Host ""
    Write-Host " This project is locked to 5.8. Do not build it with 5.4/5.5/5.6/5.7."
    exit 1
}

$ver = Get-UnrealEngineVersion $root
Write-Host " RESULT: using $root" -ForegroundColor Green
Write-Host " Version: $ver"
Write-Host ""

if ($Remember) {
    [Environment]::SetEnvironmentVariable("UE58_ROOT", $root, "User")
    Write-Host " Saved UE58_ROOT for your user account. Reopen PowerShell for it to apply."
    Write-Host ""
}

Write-Host " Build now with:"
Write-Host "   .\tools\build_phase2.cmd"
exit 0
