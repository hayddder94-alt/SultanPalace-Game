# The Betrayed Will / وصية الغدر — where is Unreal Engine 5.8?
#
# Run this when the build script cannot find the engine.
# It only looks and reports. The single thing it can change is a permanent
# UE58_ROOT environment variable for your user account, and only with -Remember.
#
# Usage:
#   .\tools\find_ue58.cmd              full search (registry, launcher, disk sweep)
#   .\tools\find_ue58.cmd -Quick       skip the disk sweep
#   .\tools\find_ue58.cmd -Remember    save the 5.8 path as UE58_ROOT

param([switch]$Remember, [switch]$Quick)

$ErrorActionPreference = "Stop"
. (Join-Path $PSScriptRoot "ue58_common.ps1")

Write-Host ""
Write-Host "=============================================================="
Write-Host " UNREAL ENGINE DISCOVERY"
Write-Host "=============================================================="
if (-not $Quick) {
    Write-Host " Searching registry, Epic Launcher manifest and all drives."
    Write-Host " The disk sweep can take up to a minute. Use -Quick to skip it."
}
Write-Host ""

# One scan, reused for both the report and the decision.
$all = @(Find-UnrealEngines -DeepScan:(-not $Quick))

if ($all.Count -eq 0) {
    Write-Host " No Unreal Engine installation was detected on this PC." -ForegroundColor Red
} else {
    Write-Host " Unreal installations detected:"
    foreach ($e in $all) {
        $ver  = if ($e.Version) { $e.Version } else { "unknown" }
        $mark = if ($e.Version -and $e.Version.StartsWith("5.8")) { "   <-- 5.8, this is the one" } else { "" }
        Write-Host ("   {0,-9} {1}{2}" -f $ver, $e.Path, $mark)
        Write-Host ("             source: {0}" -f $e.Source) -ForegroundColor DarkGray
    }
}
Write-Host ""

$five8 = $all | Where-Object { $_.Version -and $_.Version.StartsWith("5.8") } | Select-Object -First 1

if (-not $five8) {
    Write-Host " RESULT: no Unreal Engine 5.8 found." -ForegroundColor Red
    Write-Host ""
    Write-Host " If you know where it is, the correct folder is the one that contains"
    Write-Host " Engine\Build\BatchFiles\Build.bat . Point the build at it directly:"
    Write-Host ""
    Write-Host '   .\tools\build_phase2.cmd -EngineRoot "E:\your\path\UE_5.8"'
    Write-Host ""
    Write-Host " If 5.8 is not installed: Epic Games Launcher > Unreal Engine > Library >"
    Write-Host " '+' > pick 5.8. Roughly 60 GB free space is needed."
    Write-Host ""
    Write-Host " This project is locked to 5.8. Do not build it with 5.4/5.5/5.6/5.7."
    exit 1
}

Write-Host " RESULT: UE $($five8.Version)" -ForegroundColor Green
Write-Host " Path   : $($five8.Path)"
Write-Host ""

if ($Remember) {
    [Environment]::SetEnvironmentVariable("UE58_ROOT", $five8.Path, "User")
    $env:UE58_ROOT = $five8.Path
    Write-Host " Saved UE58_ROOT for your user account." -ForegroundColor Green
    Write-Host " New PowerShell windows will pick it up automatically."
    Write-Host ""
}

Write-Host " Build now with:"
if ($Remember) {
    Write-Host "   .\tools\build_phase2.cmd"
} else {
    Write-Host "   .\tools\build_phase2.cmd -EngineRoot ""$($five8.Path)"""
    Write-Host "   (or run this script again with -Remember so you never pass it again)"
}
exit 0
