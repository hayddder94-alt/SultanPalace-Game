# ============================================================================
#  The Betrayed Will / وصية الغدر — ONE COMMAND
#
#  Runs the whole verification chain and writes a single report to paste back:
#     1. pull the latest code
#     2. build the editor target
#     3. run the automated self test inside a real Unreal process
#
#  Stops at the first real problem, because a failure later in the chain is
#  meaningless if an earlier step already broke.
# ============================================================================

param(
    [string]$EngineRoot = $env:UE58_ROOT,
    [switch]$SkipPull,
    [switch]$SkipBuild
)

$ErrorActionPreference = "Continue"
$ProjectRoot = (Resolve-Path (Join-Path $PSScriptRoot "..")).Path
$LogDir = Join-Path $ProjectRoot "Saved\Logs"
$Report = Join-Path $LogDir "REPORT.txt"
New-Item -ItemType Directory -Force -Path $LogDir | Out-Null

$lines = New-Object System.Collections.Generic.List[string]
function Say($text, $colour = "Gray") {
    Write-Host $text -ForegroundColor $colour
    $lines.Add($text)
}
function Record($text) { $lines.Add($text) }

Clear-Host
Write-Host ""
Write-Host "============================================================" -ForegroundColor Cyan
Write-Host "   THE BETRAYED WILL - one command" -ForegroundColor Cyan
Write-Host "============================================================" -ForegroundColor Cyan
Write-Host ""

Record "============================================================"
Record " THE BETRAYED WILL - FULL REPORT"
Record " $(Get-Date -Format 'yyyy-MM-dd HH:mm')"
Record " machine: $env:COMPUTERNAME"
Record "============================================================"

# ------------------------------------------------------------- step 1: pull
$step = 1
if (-not $SkipPull) {
    Write-Host "   Step 1 of 3 : getting the latest code" -ForegroundColor White
    Record ""
    Record "---------- STEP 1: git pull ----------"

    Push-Location $ProjectRoot
    $pull = & git pull origin arena/019ffc4c-sultanpalace-game 2>&1
    $pullCode = $LASTEXITCODE
    $pull | ForEach-Object { Record "  $_" }
    $head = & git log -1 --pretty=format:"%h %s" 2>&1
    Record "  now at: $head"
    Pop-Location

    if ($pullCode -ne 0) {
        Say "   git pull FAILED - check the internet connection." "Red"
        Record "RESULT: git pull failed"
        $lines | Set-Content -Path $Report -Encoding UTF8
        Write-Host ""
        Write-Host "   Report: $Report"
        Start-Process notepad $Report
        exit 1
    }
    Write-Host "   code is up to date: $head" -ForegroundColor Green
} else {
    Record "---------- STEP 1: skipped ----------"
}

# ------------------------------------------------------------ step 2: build
Write-Host ""
Write-Host "   Step 2 of 3 : building - the first run can take 20-40 minutes" -ForegroundColor White
Write-Host ""

Record ""
Record "---------- STEP 2: build ----------"

$buildCode = 0
if (-not $SkipBuild) {
    $buildArgs = @("-NoProfile", "-ExecutionPolicy", "Bypass", "-File",
                   (Join-Path $PSScriptRoot "phase2_build_and_check.ps1"))
    if ($EngineRoot) { $buildArgs += @("-EngineRoot", $EngineRoot) }

    $buildOut = & powershell @buildArgs 2>&1
    $buildCode = $LASTEXITCODE
    $buildOut | ForEach-Object { Record "  $_" }

    # Show only what matters on screen; the report keeps everything.
    $buildOut | Where-Object { $_ -match "Engine |Exit code|Errors found|Warnings found|error |BUILD" } |
        ForEach-Object { Write-Host "   $_" }
} else {
    Record "  skipped"
}

if ($buildCode -ne 0) {
    Write-Host ""
    Write-Host "   BUILD FAILED - nothing else matters until this is fixed." -ForegroundColor Red
    Record "RESULT: build failed with exit code $buildCode"
    $lines | Set-Content -Path $Report -Encoding UTF8
    Write-Host ""
    Write-Host "   Report: $Report"
    Start-Process notepad $Report
    exit 1
}
Write-Host "   build: SUCCEEDED" -ForegroundColor Green

# --------------------------------------------------------- step 3: selftest
Write-Host ""
Write-Host "   Step 3 of 3 : automated self test - no window will appear" -ForegroundColor White
Write-Host ""

Record ""
Record "---------- STEP 3: self test ----------"

$testArgs = @("-NoProfile", "-ExecutionPolicy", "Bypass", "-File",
              (Join-Path $PSScriptRoot "selftest.ps1"))
if ($EngineRoot) { $testArgs += @("-EngineRoot", $EngineRoot) }

$testOut = & powershell @testArgs 2>&1
$testCode = $LASTEXITCODE
$testOut | ForEach-Object { Record "  $_" }
$testOut | Where-Object { $_ -match "PASS |FAIL |INFO |VERDICT|SELFTEST RESULT" } |
    ForEach-Object { Write-Host "   $_" }

Record ""
Record "RESULT: build ok, self test exit code $testCode"
Record "---------- END OF REPORT ----------"

$lines | Set-Content -Path $Report -Encoding UTF8

Write-Host ""
Write-Host "============================================================" -ForegroundColor Cyan
Write-Host "   DONE" -ForegroundColor Cyan
Write-Host "============================================================" -ForegroundColor Cyan
Write-Host ""
Write-Host "   Report written to:"
Write-Host "     $Report"
Write-Host ""
Write-Host "   It is opening in Notepad. Select all with Ctrl+A, copy with"
Write-Host "   Ctrl+C, and paste it into the chat."
Write-Host ""

Start-Process notepad $Report
exit $testCode
