# ============================================================================
#  The Betrayed Will / وصية الغدر — ONE COMMAND TO PLAY
#
#  Everything between "I pulled" and "I am walking in the palace", in order,
#  stopping at the first thing that is actually wrong:
#
#     1. pull
#     2. delete the broken plugin copy if it is still there
#     3. compile (refuses if the editor is open, and says so)
#     4. rebuild the level from data
#     5. launch
#
#  Launches STANDALONE by default, not the editor. The editor renders its whole
#  UI on top of the game and shares the GPU with the asset browser; a frame rate
#  measured there is not the game's frame rate. -Editor if you need the viewport.
#
#  Usage:
#    .\tools\PLAY.cmd                 pull, build, rebuild level, play standalone
#    .\tools\PLAY.cmd -Editor         open the editor instead
#    .\tools\PLAY.cmd -SkipBuild      go straight to playing
#    .\tools\PLAY.cmd -SkipLevel      do not rebuild the level
# ============================================================================

param(
    [string]$EngineRoot = $env:UE58_ROOT,
    [switch]$Editor,
    [switch]$SkipBuild,
    [switch]$SkipLevel,
    [switch]$SkipPull
)

$ErrorActionPreference = "Continue"
. (Join-Path $PSScriptRoot "ue58_common.ps1")

$ProjectRoot = (Resolve-Path (Join-Path $PSScriptRoot "..")).Path
$UProject    = Join-Path $ProjectRoot "TheBetrayedWill.uproject"
$MapPackage  = "/Game/TBW/Maps/L_VS_Palace_EastWing"

function Step($n, $text) {
    Write-Host ""
    Write-Host "  [$n] $text" -ForegroundColor White
}
function Die($text) {
    Write-Host ""
    Write-Host "============================================================" -ForegroundColor Red
    Write-Host "   STOPPED: $text" -ForegroundColor Red
    Write-Host "============================================================" -ForegroundColor Red
    Write-Host ""
    exit 1
}

Clear-Host
Write-Host ""
Write-Host "============================================================" -ForegroundColor Cyan
Write-Host "   THE BETRAYED WILL - PLAY" -ForegroundColor Cyan
Write-Host "============================================================" -ForegroundColor Cyan

# ------------------------------------------------------------------ 1. pull
if (-not $SkipPull) {
    Step 1 "getting the latest code"
    & powershell -NoProfile -ExecutionPolicy Bypass -File (Join-Path $PSScriptRoot "PULL.ps1") | Out-Host
    if ($LASTEXITCODE -ne 0) { Die "the pull failed - read the lines above" }
} else {
    Step 1 "pull skipped"
}

# ------------------------------------------------- 2. the broken plugin copy
Step 2 "checking for the broken plugin copy"
$copy = Join-Path $ProjectRoot "Content\Characters"
if (Test-Path $copy) {
    # Assets copied out of a plugin keep absolute references, so every animation
    # comes back with no skeleton and the character stands in its bind pose
    # while the report says success. The plugin is mounted by the uproject now,
    # so the copy is not just redundant, it shadows the working one.
    Write-Host "      found $copy - removing it (the plugin is mounted instead)" -ForegroundColor Yellow
    Remove-Item $copy -Recurse -Force -ErrorAction SilentlyContinue
    if (Test-Path $copy) { Die "could not delete $copy - is the editor holding it open?" }
    Write-Host "      removed." -ForegroundColor Green
} else {
    Write-Host "      clean." -ForegroundColor Green
}

# ----------------------------------------------------------------- 3. build
if (-not $SkipBuild) {
    Step 3 "compiling - the editor must be closed"
    $buildArgs = @("-NoProfile", "-ExecutionPolicy", "Bypass", "-File",
                   (Join-Path $PSScriptRoot "phase2_build_and_check.ps1"))
    if ($EngineRoot) { $buildArgs += @("-EngineRoot", $EngineRoot) }
    $out = & powershell @buildArgs 2>&1
    $code = $LASTEXITCODE
    $out | Where-Object { $_ -match "Exit code|Errors found|error [A-Z]+\d+|: error|^cause |^fix   |EDITOR IS RUNNING|^   pid " } |
        ForEach-Object { Write-Host "      $_" }
    if ($code -eq 90) { Die "close the Unreal editor, then run this again" }
    if ($code -ne 0)  { Die "the code did not compile - the errors are above" }
    Write-Host "      compiled." -ForegroundColor Green
} else {
    Step 3 "build skipped"
}

# ----------------------------------------------------------------- 4. level
if (-not $SkipLevel) {
    Step 4 "rebuilding the palace from data"
    $lvlArgs = @("-NoProfile", "-ExecutionPolicy", "Bypass", "-File",
                 (Join-Path $PSScriptRoot "BUILD_LEVEL.ps1"), "-SkipVerify")
    if ($EngineRoot) { $lvlArgs += @("-EngineRoot", $EngineRoot) }
    $out = & powershell @lvlArgs 2>&1
    $out | Where-Object { $_ -match "\[TBW\]" } | ForEach-Object { Write-Host "      $_" }
    if ($LASTEXITCODE -ne 0) { Die "the level did not build - the lines above say why" }
} else {
    Step 4 "level rebuild skipped"
}

# ---------------------------------------------------------------- 5. launch
$EngineRoot = Resolve-UE58Root -Preferred $EngineRoot
if (-not $EngineRoot) { Die "Unreal Engine 5.8 not found - run .\tools\find_ue58.cmd" }
$EditorExe = Join-Path $EngineRoot "Engine\Binaries\Win64\UnrealEditor.exe"
if (-not (Test-Path $EditorExe)) { Die "UnrealEditor.exe not found under $EngineRoot" }

Step 5 $(if ($Editor) { "opening the editor" } else { "launching the game" })

if ($Editor) {
    Start-Process -FilePath $EditorExe -ArgumentList @("`"$UProject`"", $MapPackage)
    Write-Host ""
    Write-Host "   The editor is opening on $MapPackage." -ForegroundColor Green
    Write-Host '   Press Play. Console is ` or \ once you have clicked in the viewport.' 
} else {
    # Standalone. No editor UI competing for the GPU, so 'stat unit' here is a
    # number worth writing down - unlike one read inside the editor.
    $gameArgs = @("`"$UProject`"", $MapPackage, "-game", "-windowed", "-ResX=1600", "-ResY=900")
    Start-Process -FilePath $EditorExe -ArgumentList $gameArgs
    Write-Host ""
    Write-Host "   The game is starting in its own window." -ForegroundColor Green
}

Write-Host ""
Write-Host "============================================================" -ForegroundColor Cyan
Write-Host "   WHAT TO LOOK AT" -ForegroundColor Cyan
Write-Host "============================================================" -ForegroundColor Cyan
Write-Host ""
Write-Host "   1. Walk with W. Do the legs and arms move, or does the body"
Write-Host "      slide upright? That is the one question that matters."
Write-Host ""
Write-Host "   2. The palace should be mudbrick walls and a lapis hall, not grey."
Write-Host ""
Write-Host "   3. Seventeen people, each a different colour. Raynor deep red,"
Write-Host "      Darius near black, Leila temple gold."
Write-Host ""
Write-Host '   4. Console:  `  or  \   then:' 
Write-Host "        stat unit                      frame time"
Write-Host "        tbw.Dialogue.Play VS02_TheWillReading"
Write-Host ""
Write-Host "   Close the window when done, then:  .\tools\collect_pie_log.cmd"
Write-Host ""
exit 0
