# The Betrayed Will / وصية الغدر — Windows bootstrap
#
# Use this when the machine has NO clone yet, or when you are not sure where the
# project folder is. It is safe to run repeatedly.
#
#   1. checks git is installed
#   2. clones the repo (or updates it if the folder already exists)
#   3. checks out the session branch arena/019ffc4c-sultanpalace-game
#   4. optionally runs the Phase 2 editor build
#
# Usage from ANY folder in PowerShell:
#   powershell -ExecutionPolicy Bypass -File .\windows_bootstrap.ps1
#   powershell -ExecutionPolicy Bypass -File .\windows_bootstrap.ps1 -Root "D:\Dev" -Build
#
param(
    [string]$Root = "C:\Dev",
    [string]$Branch = "arena/019ffc4c-sultanpalace-game",
    [string]$EngineRoot = $env:UE58_ROOT,
    [switch]$Build
)

$ErrorActionPreference = "Stop"
$RepoUrl = "https://github.com/hayddder94-alt/SultanPalace-Game.git"
$RepoDir = Join-Path $Root "SultanPalace-Game"

Write-Host ""
Write-Host "=============================================================="
Write-Host " THE BETRAYED WILL - WINDOWS BOOTSTRAP"
Write-Host "=============================================================="

# ------------------------------------------------------------------ git check
$git = Get-Command git -ErrorAction SilentlyContinue
if (-not $git) {
    Write-Host ""
    Write-Host "git is not installed or not on PATH." -ForegroundColor Red
    Write-Host "Install it from https://git-scm.com/download/win then reopen PowerShell."
    exit 1
}
Write-Host " git            : $((git --version))"

# ------------------------------------------------------------- clone / update
New-Item -ItemType Directory -Force -Path $Root | Out-Null

if (Test-Path (Join-Path $RepoDir ".git")) {
    Write-Host " repo           : found at $RepoDir - updating"
    Push-Location $RepoDir
    git remote set-url origin $RepoUrl
    git fetch origin $Branch
    git checkout $Branch
    git pull --ff-only origin $Branch
    Pop-Location
} elseif (Test-Path $RepoDir) {
    Write-Host ""
    Write-Host "$RepoDir exists but is not a git clone." -ForegroundColor Red
    Write-Host "Rename or delete it, then run this script again."
    exit 1
} else {
    Write-Host " repo           : cloning into $RepoDir"
    git clone --branch $Branch $RepoUrl $RepoDir
}

Push-Location $RepoDir
$Version = (Get-Content (Join-Path $RepoDir "VERSION") -Raw).Trim()
Write-Host " branch         : $((git rev-parse --abbrev-ref HEAD))"
Write-Host " commit         : $((git log -1 --pretty=format:'%h %s'))"
Write-Host " version        : $Version"
Write-Host " project file   : $(Join-Path $RepoDir 'TheBetrayedWill.uproject')"

# -------------------------------------------------- warn about other copies
# Only the usual suspects - a full drive scan would take minutes on a laptop SSD.
$SearchRoots = @(
    (Join-Path $env:USERPROFILE "Documents\Unreal Projects"),
    (Join-Path $env:USERPROFILE "Documents"),
    (Join-Path $env:USERPROFILE "Desktop"),
    (Join-Path $env:USERPROFILE "Downloads"),
    "C:\Dev", "D:\Dev", "D:\"
) | Where-Object { $_ -and (Test-Path $_) } | Select-Object -Unique

$Others = @()
foreach ($SearchRoot in $SearchRoots) {
    $found = Get-ChildItem -Path $SearchRoot -Filter "TheBetrayedWill.uproject" -Recurse -Depth 4 `
                           -ErrorAction SilentlyContinue -Force | Select-Object -First 5
    foreach ($f in $found) {
        if ($f.DirectoryName -ne $RepoDir) { $Others += $f.FullName }
    }
}
$Others = $Others | Select-Object -Unique
if ($Others.Count -gt 0) {
    Write-Host ""
    Write-Host " NOTE: other copies of the project exist on this PC:" -ForegroundColor Yellow
    $Others | ForEach-Object { Write-Host "   $_" -ForegroundColor Yellow }
    Write-Host " The authoritative one is the clone above. Do not build the old copies." -ForegroundColor Yellow
}

Pop-Location

Write-Host ""
Write-Host "--------------------------------------------------------------"
Write-Host " Ready. Project folder:"
Write-Host "   $RepoDir"
Write-Host "--------------------------------------------------------------"

if ($Build) {
    $script = Join-Path $RepoDir "tools\phase2_build_and_check.ps1"
    if ($EngineRoot) {
        & powershell -ExecutionPolicy Bypass -File $script -EngineRoot $EngineRoot
    } else {
        & powershell -ExecutionPolicy Bypass -File $script
    }
} else {
    Write-Host ""
    Write-Host " Next:"
    Write-Host "   cd $RepoDir"
    Write-Host "   powershell -ExecutionPolicy Bypass -File .\tools\phase2_build_and_check.ps1"
}
