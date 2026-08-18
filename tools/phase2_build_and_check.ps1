# The Betrayed Will / وصية الغدر — Phase 2 editor build + log triage
#
# Run this on the Windows machine that has Unreal Engine 5.8.x.
#   1. locates UE 5.8 (registry / launcher manifest / disk)
#   2. regenerates project files (best effort - the build does not need them)
#   3. builds TheBetrayedWillEditor Win64 Development
#   4. saves the full log and prints ONLY what matters
#
# Usage (PowerShell, from the repo root):
#   .\tools\build_phase2.cmd
#   .\tools\build_phase2.cmd -EngineRoot "E:\UE_5.8"
#   .\tools\build_phase2.cmd -Clean          # force a full rebuild
#   .\tools\build_phase2.cmd -SkipGenerate   # build only
#
# Copy the SUMMARY block at the end back into the chat.

param(
    [string]$EngineRoot = $env:UE58_ROOT,
    [switch]$Clean,
    [switch]$SkipGenerate,
    [switch]$AllowWrongEngineVersion
)

$ErrorActionPreference = "Stop"
. (Join-Path $PSScriptRoot "ue58_common.ps1")

$ProjectRoot = (Resolve-Path (Join-Path $PSScriptRoot "..")).Path
$UProject    = Join-Path $ProjectRoot "TheBetrayedWill.uproject"
$LogDir      = Join-Path $ProjectRoot "Saved\Logs"
$Stamp       = Get-Date -Format "yyyyMMdd-HHmmss"
$BuildLog    = Join-Path $LogDir "Phase2_Build_$Stamp.log"

New-Item -ItemType Directory -Force -Path $LogDir | Out-Null

<#
    Runs a native tool, tees its output to the log, returns its exit code.
    Native tools write progress to stderr. With '2>&1' and $ErrorActionPreference='Stop'
    PowerShell turns that into a TERMINATING NativeCommandError and kills the script
    before the build even starts - which is exactly what happened on the first run.
    So the preference is relaxed for the duration of the call only.
#>
function Invoke-Logged {
    param(
        [Parameter(Mandatory)] [string]   $Exe,
        [string[]] $Arguments = @(),
        [Parameter(Mandatory)] [string]   $LogPath,
        [switch]   $Append
    )
    $previous = $ErrorActionPreference
    $ErrorActionPreference = "Continue"
    try {
        # Out-Host is essential: Tee-Object forwards every line down the pipeline,
        # and anything left in the success stream becomes part of this function's
        # return value - which would turn the exit code into a huge array.
        if ($Append) {
            & $Exe @Arguments 2>&1 | Tee-Object -FilePath $LogPath -Append | Out-Host
        } else {
            & $Exe @Arguments 2>&1 | Tee-Object -FilePath $LogPath | Out-Host
        }
        return $LASTEXITCODE
    } finally {
        $ErrorActionPreference = $previous
    }
}

# ---------------------------------------------------------------- engine root
$EngineRoot = Resolve-UE58Root -Preferred $EngineRoot

if (-not $EngineRoot) {
    Write-Host ""
    Write-Host "Unreal Engine 5.8 was not found on this PC." -ForegroundColor Red
    Write-Host ""
    Show-UnrealEngines
    Write-Host ""
    Write-Host "Options:"
    Write-Host "  1. Pass the folder that contains Engine\Build\BatchFiles\Build.bat :"
    Write-Host '       .\tools\build_phase2.cmd -EngineRoot "E:\UE_5.8"'
    Write-Host "  2. Run .\tools\find_ue58.cmd for a full report."
    Write-Host "  3. If 5.8 is not installed: Epic Games Launcher > Library > '+' > 5.8."
    Write-Host ""
    Write-Host "This project is locked to UE 5.8. Do not build it with another version."
    exit 1
}

$BuildBat = Join-Path $EngineRoot "Engine\Build\BatchFiles\Build.bat"
$UBTDll   = Join-Path $EngineRoot "Engine\Binaries\DotNET\UnrealBuildTool\UnrealBuildTool.dll"
if (-not (Test-Path $BuildBat)) { Write-Error "Build.bat not found under $EngineRoot" }

# Unreal ships its own .NET runtime. Using it avoids the system-wide
# "You must install or update .NET to run this application." failure.
$BundledDotnet = Get-BundledDotnet $EngineRoot

$EngineVersion    = Get-UnrealEngineVersion $EngineRoot
if (-not $EngineVersion) { $EngineVersion = "unknown" }
$IsInstalledBuild = Test-InstalledEngineBuild $EngineRoot
$EngineKind       = if ($IsInstalledBuild) { "installed build (InstalledBuild.txt present)" } else { "source build" }
$EngineIds        = @(Get-EngineIdentifiers $EngineRoot)
$Association      = (Get-Content $UProject -Raw | ConvertFrom-Json).EngineAssociation

Write-Host ""
Write-Host "=============================================================="
Write-Host " THE BETRAYED WILL - PHASE 2 BUILD CHECK"
Write-Host "=============================================================="
Write-Host " Engine root    : $EngineRoot"
Write-Host " Engine version : $EngineVersion   (project is locked to 5.8)"
Write-Host " Engine kind    : $EngineKind"
Write-Host " Registered as  : $(if ($EngineIds.Count) { $EngineIds -join ', ' } else { '(not registered)' })"
Write-Host " Bundled .NET   : $(if ($BundledDotnet) { $BundledDotnet } else { '(none found - will rely on Build.bat)' })"
Write-Host " Project        : $UProject"
Write-Host " EngineAssoc.   : $Association"
Write-Host " Log            : $BuildLog"
Write-Host ""

if ($EngineIds.Count -gt 0 -and ($EngineIds -notcontains $Association)) {
    Write-Host " NOTE: engine registered as '$($EngineIds -join "', '")' but the project asks for '$Association'." -ForegroundColor Yellow
    Write-Host "       Command-line builds are unaffected. For Explorer double-click support run:" -ForegroundColor Yellow
    Write-Host "         .\tools\find_ue58.cmd -RegisterAs58" -ForegroundColor Yellow
    Write-Host ""
}

if ($EngineVersion -ne "unknown" -and -not $EngineVersion.StartsWith("5.8")) {
    Write-Host " ENGINE MISMATCH: this install is $EngineVersion, the project is locked to 5.8." -ForegroundColor Red
    Write-Host " Building would produce a wall of unrelated errors. Stopping." -ForegroundColor Red
    Write-Host ""
    Write-Host " Run .\tools\find_ue58.cmd to list every engine on this PC,"
    Write-Host " or override deliberately with -AllowWrongEngineVersion (not recommended)."
    if (-not $AllowWrongEngineVersion) { exit 2 }
    Write-Warning "Continuing on $EngineVersion because -AllowWrongEngineVersion was passed."
}

# --------------------------------------------------------------------- clean
if ($Clean) {
    Write-Host "[clean] removing Binaries / Intermediate / DerivedDataCache ..."
    foreach ($d in @("Binaries", "Intermediate", "DerivedDataCache")) {
        $p = Join-Path $ProjectRoot $d
        if (Test-Path $p) { Remove-Item $p -Recurse -Force -ErrorAction SilentlyContinue }
    }
}

# ---------------------------------------------------------- project files gen
if (-not $SkipGenerate) {
    Write-Host "[1/2] generating project files (optional step) ..."

    $GenArgs = @("-projectfiles", "-project=$UProject", "-game", "-progress")
    # -rocket is only meaningful for installed builds.
    if ($IsInstalledBuild) { $GenArgs += "-rocket" }

    $GenExit = $null
    if ($BundledDotnet -and (Test-Path $UBTDll)) {
        # Engine's own .NET runtime + UnrealBuildTool.dll: no system .NET required.
        $GenExit = Invoke-Logged -Exe $BundledDotnet -Arguments (@($UBTDll) + $GenArgs) -LogPath $BuildLog
    } else {
        # Build.bat sets up the engine's .NET itself.
        $GenExit = Invoke-Logged -Exe $BuildBat -Arguments $GenArgs -LogPath $BuildLog
    }

    if ($GenExit -ne 0) {
        Write-Warning "Project file generation returned $GenExit. Continuing - the compile below does not need it."
    }
} else {
    Write-Host "[1/2] project file generation skipped."
}

# --------------------------------------------------------------------- build
Write-Host "[2/2] building TheBetrayedWillEditor Win64 Development ..."
Write-Host "      First build on this machine can take 20-40 minutes. Leave it running."
$sw = [System.Diagnostics.Stopwatch]::StartNew()

$BuildArgs = @("TheBetrayedWillEditor", "Win64", "Development", "-Project=$UProject", "-WaitMutex", "-FromMsBuild")
$BuildExit = Invoke-Logged -Exe $BuildBat -Arguments $BuildArgs -LogPath $BuildLog -Append
$sw.Stop()

# ------------------------------------------------------------------- triage
$log      = @(Get-Content $BuildLog -ErrorAction SilentlyContinue)
$errors   = @($log | Select-String -Pattern "error [A-Z]+\d+|: error|LNK\d{4}|fatal error|You must install or update \.NET" |
                     Select-Object -First 40)
$warnings = @($log | Select-String -Pattern "warning [A-Z]+\d+|: warning" |
                     Where-Object { $_ -notmatch "Microsoft.MakeFile.Targets" } |
                     Select-Object -First 25)

Write-Host ""
Write-Host "=============================================================="
Write-Host " SUMMARY  (copy everything below into the chat)"
Write-Host "=============================================================="
Write-Host "Engine         : $EngineVersion  at  $EngineRoot"
Write-Host "Engine kind    : $EngineKind"
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
    Write-Host "Next: open TheBetrayedWill.uproject in UE 5.8, press Play, and work through"
    Write-Host "docs\PHASE2_PIE_CHECKLIST.md. Phase 3 does not start before that passes."
}

exit $BuildExit
