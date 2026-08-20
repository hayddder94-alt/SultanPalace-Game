# ============================================================================
#  The Betrayed Will / وصية الغدر — build the palace level, headless
#
#  Runs tools/ue_python/build_east_wing.py inside a real editor process with no
#  window and no GPU, then verifies the result with the self test.
#
#  This exists because the alternative was a GUI walkthrough: open the editor,
#  find Tools, find Execute Python Script, pick a file, wait, save. Every manual
#  step is a step that gets skipped or done differently twice.
#
#  Usage:
#    .\tools\BUILD_LEVEL.cmd
#    .\tools\BUILD_LEVEL.cmd -SkipVerify
# ============================================================================

param(
    [string]$EngineRoot = $env:UE58_ROOT,
    [switch]$SkipVerify,
    [int]$TimeoutSeconds = 900
)

$ErrorActionPreference = "Continue"
. (Join-Path $PSScriptRoot "ue58_common.ps1")

$ProjectRoot = (Resolve-Path (Join-Path $PSScriptRoot "..")).Path
$UProject    = Join-Path $ProjectRoot "TheBetrayedWill.uproject"
$Script      = Join-Path $ProjectRoot "tools\ue_python\build_east_wing.py"
$LogDir      = Join-Path $ProjectRoot "Saved\Logs"
$Stamp       = Get-Date -Format "yyyyMMdd-HHmmss"
$LogFile     = Join-Path $LogDir "BuildLevel_$Stamp.log"
$MapPackage  = "/Game/TBW/Maps/L_VS_Palace_EastWing"

New-Item -ItemType Directory -Force -Path $LogDir | Out-Null

$EngineRoot = Resolve-UE58Root -Preferred $EngineRoot
if (-not $EngineRoot) {
    Write-Host "Unreal Engine 5.8 not found. Run .\tools\find_ue58.cmd first." -ForegroundColor Red
    exit 1
}

$EditorCmd = Join-Path $EngineRoot "Engine\Binaries\Win64\UnrealEditor-Cmd.exe"
if (-not (Test-Path $EditorCmd)) {
    $EditorCmd = Join-Path $EngineRoot "Engine\Binaries\Win64\UnrealEditor.exe"
}

Clear-Host
Write-Host ""
Write-Host "============================================================" -ForegroundColor Cyan
Write-Host "   BUILDING THE PALACE - headless" -ForegroundColor Cyan
Write-Host "============================================================" -ForegroundColor Cyan
Write-Host " Engine : $EngineRoot"
Write-Host " Script : $Script"
Write-Host " Map    : $MapPackage"
Write-Host " Log    : $LogFile"
Write-Host ""
Write-Host " No window will appear. This takes a few minutes on first run."
Write-Host ""

if (-not (Test-Path $Script)) {
    Write-Host "Level script missing: $Script" -ForegroundColor Red
    exit 1
}

# The editor must run in EDITOR mode, not -game: creating and saving a .umap
# needs LevelEditorSubsystem and EditorActorSubsystem, which only exist there.
$argList = @(
    "`"$UProject`"",
    "-run=pythonscript",
    "-script=`"$Script`"",
    "-unattended",
    "-nullrhi",
    "-nosound",
    "-nosplash",
    # The Git source control plugin tried to check the map out and popped a
    # dialog mid-save. Headless runs must never wait on a human.
    "-SCCProvider=None",
    "-stdout",
    "-FullStdOutLogOutput",
    "-AbsLog=`"$LogFile`""
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
$tbw = @($log | Select-String -Pattern "\[TBW\]")
$saved = @($log | Select-String -Pattern "level saved")
$errors = @($log | Select-String -Pattern "LogPython: Error|LogOutputDevice: Error|Fatal error") | Select-Object -First 15

Write-Host "------------------------------------------------------------"
foreach ($line in $tbw) { Write-Host "   $($line.Line.Trim())" }
Write-Host "------------------------------------------------------------"
Write-Host " Duration : $([math]::Round($sw.Elapsed.TotalMinutes,1)) min"
Write-Host ""

if ($errors.Count -gt 0) {
    Write-Host "---- PYTHON / ENGINE ERRORS --------------------------------" -ForegroundColor Red
    $errors | ForEach-Object { Write-Host $_.Line.Trim() }
    Write-Host ""
}

if ($saved.Count -eq 0) {
    Write-Host " The level was NOT saved. Send the lines above." -ForegroundColor Red
    Write-Host " Full log: $LogFile"
    exit 1
}

Write-Host " Level built and saved: $MapPackage" -ForegroundColor Green
Write-Host ""

# Deliberately NOT editing Config/DefaultEngine.ini here.
# A local-only edit to a file the repository also owns turns the next git pull
# into a merge conflict, and the whole point of GO.cmd is that the user never
# has to resolve one. The map is opened explicitly instead: by -Map for the self
# test, and by hand in the editor.

if ($SkipVerify) { exit 0 }

Write-Host " Verifying the level with the self test ..." -ForegroundColor White
Write-Host ""

$verifyArgs = @("-NoProfile", "-ExecutionPolicy", "Bypass", "-File",
                (Join-Path $PSScriptRoot "selftest.ps1"), "-Map", $MapPackage)
if ($EngineRoot) { $verifyArgs += @("-EngineRoot", $EngineRoot) }

$out = & powershell @verifyArgs 2>&1
$code = $LASTEXITCODE
$out | Where-Object { $_ -match "PASS |FAIL |INFO |VERDICT|SELFTEST RESULT" } |
    ForEach-Object { Write-Host "   $_" }

$summary = ($out | Where-Object { $_ -match "PASS |FAIL |INFO |VERDICT|SELFTEST RESULT|interactable" }) -join "`r`n"
Copy-TBWReport -Title "Palace build + verify" `
    -Text "$($tbw -join "`r`n")`r`n`r`n$summary" | Out-Null

Write-Host ""
exit $code
