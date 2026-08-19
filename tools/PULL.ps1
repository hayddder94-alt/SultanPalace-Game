# ============================================================================
#  The Betrayed Will / وصية الغدر — pull, safely, when a plain pull refuses
#
#  A plain `git pull` fails the moment the editor has rewritten a tracked file,
#  which it does routinely: opening the project rewrites Config\*.ini, and a
#  level build touches the map. The failure message is git's, but the fix is
#  always one of three, and choosing the wrong one loses work.
#
#  This script decides which, does it, and says what it did.
#
#  Usage:
#    .\tools\PULL.cmd            pull, stashing local edits and putting them back
#    .\tools\PULL.cmd -Discard   pull, throwing local edits away (no stash)
#    .\tools\PULL.cmd -DryRun    diagnose only, change nothing
# ============================================================================

param(
    [switch]$Discard,
    [switch]$DryRun
)

$ErrorActionPreference = "Continue"
$ProjectRoot = (Resolve-Path (Join-Path $PSScriptRoot "..")).Path
$Branch      = "arena/019ffc4c-sultanpalace-game"

Push-Location $ProjectRoot

Write-Host ""
Write-Host "============================================================" -ForegroundColor Cyan
Write-Host "   PULL - $Branch" -ForegroundColor Cyan
Write-Host "============================================================" -ForegroundColor Cyan
Write-Host ""

# ---------------------------------------------------------------- diagnose
& git rev-parse --is-inside-work-tree > $null 2>&1
if ($LASTEXITCODE -ne 0) {
    Write-Host " This folder is not a git checkout." -ForegroundColor Red
    Write-Host " Clone the repository again into C:\Dev\SultanPalace-Game."
    Pop-Location; exit 1
}

& git ls-remote --exit-code origin HEAD > $null 2>&1
if ($LASTEXITCODE -ne 0) {
    Write-Host " GitHub is unreachable from this machine." -ForegroundColor Red
    Write-Host " Nothing local is wrong. Check the connection and try again."
    Pop-Location; exit 1
}

$head   = & git log -1 --pretty=format:"%h %s"
$dirty  = @(& git status --porcelain | Where-Object { $_ -notmatch '^\?\?' })
$untrk  = @(& git status --porcelain | Where-Object { $_ -match '^\?\?' })

Write-Host " local head    : $head"
Write-Host " modified      : $($dirty.Count) tracked file(s)"
foreach ($d in $dirty) { Write-Host "     $d" -ForegroundColor Yellow }
Write-Host " untracked     : $($untrk.Count) file(s)  (these never block a pull)"
Write-Host ""

& git fetch origin $Branch 2>&1 | Out-Null
$behind = & git rev-list --count HEAD..FETCH_HEAD
$ahead  = & git rev-list --count FETCH_HEAD..HEAD
Write-Host " behind remote : $behind commit(s)"
Write-Host " ahead of it   : $ahead commit(s)"
Write-Host ""

if ($DryRun) {
    Write-Host " -DryRun: nothing was changed." -ForegroundColor Cyan
    Pop-Location; exit 0
}

if ($ahead -gt 0) {
    Write-Host " You have $ahead local commit(s) that are not on the branch." -ForegroundColor Red
    Write-Host " This script will not decide what happens to them. Paste this"
    Write-Host " screen into the chat and ask."
    Pop-Location; exit 1
}

if ($behind -eq 0 -and $dirty.Count -eq 0) {
    Write-Host " Already up to date. Nothing to do." -ForegroundColor Green
    Pop-Location; exit 0
}

# ----------------------------------------------------------------- act
$stashed = $false
if ($dirty.Count -gt 0) {
    if ($Discard) {
        Write-Host " Discarding local edits to tracked files ..." -ForegroundColor Yellow
        & git checkout -- .
    } else {
        # Default is the reversible option. Editor-rewritten .ini files are
        # rarely precious, but "rarely" is not "never", and a stash costs
        # nothing while an overwrite cannot be undone.
        Write-Host " Stashing local edits so they can come back ..." -ForegroundColor Yellow
        & git stash push -m "PULL.cmd auto-stash $(Get-Date -Format 'yyyy-MM-dd HH:mm')"
        if ($LASTEXITCODE -eq 0) { $stashed = $true }
    }
    Write-Host ""
}

Write-Host " Pulling ..." -ForegroundColor White
& git pull --ff-only origin $Branch
$pullCode = $LASTEXITCODE
Write-Host ""

if ($pullCode -ne 0) {
    Write-Host " The pull still failed. Nothing has been lost." -ForegroundColor Red
    if ($stashed) {
        Write-Host " Your edits are in the stash: run 'git stash pop' to get them back."
    }
    Pop-Location; exit 1
}

if ($stashed) {
    Write-Host " Putting your edits back ..." -ForegroundColor White
    & git stash pop
    if ($LASTEXITCODE -ne 0) {
        Write-Host ""
        Write-Host " Your edits conflict with what was just pulled." -ForegroundColor Red
        Write-Host " They are still safe in the stash. To throw them away:"
        Write-Host "     git checkout -- . ; git stash drop"
        Pop-Location; exit 1
    }
}

Write-Host ""
Write-Host " Now at: $(& git log -1 --pretty=format:'%h %s')" -ForegroundColor Green
Write-Host ""
Pop-Location
exit 0
