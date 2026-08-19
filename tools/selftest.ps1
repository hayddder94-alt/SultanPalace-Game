# The Betrayed Will / وصية الغدر — headless system verification
#
# Builds nothing. Launches the editor with no rendering, runs tbw.SelfTest inside
# a real Unreal process, quits, and prints the verdict.
#
# This exists because verifying by hand meant a thirty-step checklist. It replaces
# the mechanical half of that. It cannot judge how anything FEELS - that is still
# a human sitting in front of a running game.
#
# Usage:
#   .\tools\selftest.cmd
#   .\tools\selftest.cmd -Map /Game/TBW/Maps/L_VS_Palace_EastWing
#   .\tools\selftest.cmd -EngineRoot "E:\UE_5.8"

param(
    [string]$EngineRoot = $env:UE58_ROOT,
    [string]$Map = "",
    [int]$TimeoutSeconds = 420
)

$ErrorActionPreference = "Stop"
. (Join-Path $PSScriptRoot "ue58_common.ps1")

$ProjectRoot = (Resolve-Path (Join-Path $PSScriptRoot "..")).Path
$UProject    = Join-Path $ProjectRoot "TheBetrayedWill.uproject"
$LogDir      = Join-Path $ProjectRoot "Saved\Logs"
$Stamp       = Get-Date -Format "yyyyMMdd-HHmmss"
$LogFile     = Join-Path $LogDir "SelfTest_$Stamp.log"

New-Item -ItemType Directory -Force -Path $LogDir | Out-Null

$EngineRoot = Resolve-UE58Root -Preferred $EngineRoot
if (-not $EngineRoot) {
    Write-Host "Unreal Engine 5.8 not found. Run .\tools\find_ue58.cmd first." -ForegroundColor Red
    exit 1
}

# UnrealEditor-Cmd is the console variant: no window, no splash, exits cleanly.
$EditorCmd = Join-Path $EngineRoot "Engine\Binaries\Win64\UnrealEditor-Cmd.exe"
if (-not (Test-Path $EditorCmd)) {
    $EditorCmd = Join-Path $EngineRoot "Engine\Binaries\Win64\UnrealEditor.exe"
}
if (-not (Test-Path $EditorCmd)) {
    Write-Host "No editor executable under $EngineRoot" -ForegroundColor Red
    exit 1
}

Write-Host ""
Write-Host "=============================================================="
Write-Host " THE BETRAYED WILL - HEADLESS SELF TEST"
Write-Host "=============================================================="
Write-Host " Engine  : $EngineRoot"
Write-Host " Project : $UProject"
Write-Host " Map     : $(if ($Map) { $Map } else { '(project default)' })"
Write-Host " Log     : $LogFile"
Write-Host ""
Write-Host " Starting a real Unreal process with -nullrhi. No window will appear."
Write-Host " First run compiles shaders and can take several minutes."
Write-Host ""

$argList = @("`"$UProject`"")
if ($Map) { $argList += $Map }
$argList += @(
    "-game",
    "-unattended",
    "-nullrhi",              # no GPU work at all
    "-nosound",
    "-nosplash",
    "-stdout",
    "-FullStdOutLogOutput",
    "-AbsLog=`"$LogFile`"",
    "-ExecCmds=`"tbw.Version, tbw.SelfTest, quit`""
)

$sw = [System.Diagnostics.Stopwatch]::StartNew()
$proc = Start-Process -FilePath $EditorCmd -ArgumentList $argList -PassThru -NoNewWindow
if (-not $proc.WaitForExit($TimeoutSeconds * 1000)) {
    Write-Host "Timed out after $TimeoutSeconds s - killing the process." -ForegroundColor Red
    try { $proc.Kill() } catch { }
}
$sw.Stop()

Start-Sleep -Milliseconds 400
$log = @(Get-Content $LogFile -ErrorAction SilentlyContinue)

$report = @($log | Select-String -Pattern "SELFTEST|PASS  |FAIL  |INFO  ")
$result = @($log | Select-String -Pattern "TBW SELFTEST RESULT:") | Select-Object -Last 1
$errors = @($log | Select-String -Pattern "LogOutputDevice: Error|Fatal error|Assertion failed") | Select-Object -First 12

Write-Host "=============================================================="
Write-Host " SUMMARY  (copy everything below into the chat)"
Write-Host "=============================================================="
Write-Host "Duration   : $([math]::Round($sw.Elapsed.TotalMinutes,1)) min"
Write-Host "Exit code  : $($proc.ExitCode)"
Write-Host "Log        : $LogFile"
Write-Host ""

if ($report.Count -gt 0) {
    $report | ForEach-Object { Write-Host $_.Line.Trim() }
    Write-Host ""
}

if ($errors.Count -gt 0) {
    Write-Host "---- ENGINE ERRORS -------------------------------------------"
    $errors | ForEach-Object { Write-Host $_.Line.Trim() }
    Write-Host ""
}

if (-not $result) {
    Write-Host "VERDICT: the self test never ran. The build may be stale - run" -ForegroundColor Red
    Write-Host "         .\tools\build_phase2.cmd first, then this again." -ForegroundColor Red
    exit 2
}

if ($result -match "(\d+) passed, (\d+) failed") {
    $failed = [int]$Matches[2]
    if ($failed -eq 0) {
        Write-Host "VERDICT: all systems green." -ForegroundColor Green
        exit 0
    }
    Write-Host "VERDICT: $failed check(s) failed - paste the FAIL lines above." -ForegroundColor Red
    exit 1
}

Write-Host "VERDICT: unreadable result line." -ForegroundColor Yellow
exit 3
