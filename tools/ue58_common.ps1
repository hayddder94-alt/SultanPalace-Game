# The Betrayed Will / وصية الغدر — shared Unreal Engine discovery
# Dot-sourced by the other tools. Not meant to be run directly.

function Get-UnrealEngineVersion {
    param([string]$EngineRoot)

    $VersionH = Join-Path $EngineRoot "Engine\Source\Runtime\Launch\Resources\Version.h"
    if (-not (Test-Path $VersionH)) { return $null }
    $vh    = Get-Content $VersionH -Raw
    $major = [regex]::Match($vh, "ENGINE_MAJOR_VERSION\s+(\d+)").Groups[1].Value
    $minor = [regex]::Match($vh, "ENGINE_MINOR_VERSION\s+(\d+)").Groups[1].Value
    $patch = [regex]::Match($vh, "ENGINE_PATCH_VERSION\s+(\d+)").Groups[1].Value
    if (-not $major) { return $null }
    return "$major.$minor.$patch"
}

function Test-UnrealEngineRoot {
    param([string]$Path)
    if (-not $Path) { return $false }
    if (-not (Test-Path $Path)) { return $false }
    return (Test-Path (Join-Path $Path "Engine\Build\BatchFiles\Build.bat"))
}

<#
    Returns every Unreal install this machine can prove it has, newest info first.
    Sources, in order of reliability:
      1. UE58_ROOT / UE_ROOT environment variables
      2. Epic Games Launcher manifest  (C:\ProgramData\Epic\UnrealEngineLauncher\LauncherInstalled.dat)
      3. Registry HKLM\SOFTWARE\EpicGames\Unreal Engine\<ver>\InstalledDirectory
      4. Registry HKCU\SOFTWARE\Epic Games\Unreal Engine\Builds   (source builds)
      5. A bounded probe of the usual folders on every fixed drive
#>
function Find-UnrealEngines {
    param([switch]$DeepScan)

    $candidates = New-Object System.Collections.Generic.List[object]

    function Add-Candidate([string]$Path, [string]$Source) {
        if (-not (Test-UnrealEngineRoot $Path)) { return }
        $full = (Resolve-Path $Path -ErrorAction SilentlyContinue).Path
        if (-not $full) { return }
        foreach ($c in $candidates) { if ($c.Path -ieq $full) { return } }
        $candidates.Add([pscustomobject]@{
            Path    = $full
            Version = (Get-UnrealEngineVersion $full)
            Source  = $Source
        })
    }

    # 1 — environment
    Add-Candidate $env:UE58_ROOT "UE58_ROOT env var"
    Add-Candidate $env:UE_ROOT   "UE_ROOT env var"

    # 2 — Epic launcher manifest
    $dat = "C:\ProgramData\Epic\UnrealEngineLauncher\LauncherInstalled.dat"
    if (Test-Path $dat) {
        try {
            $json = Get-Content $dat -Raw | ConvertFrom-Json
            foreach ($item in $json.InstallationList) {
                if ($item.AppName -like "UE_*") {
                    Add-Candidate $item.InstallLocation "Epic Launcher ($($item.AppName))"
                }
            }
        } catch { }
    }

    # 3 — registry, launcher installs
    $hklm = "HKLM:\SOFTWARE\EpicGames\Unreal Engine"
    if (Test-Path $hklm) {
        foreach ($key in (Get-ChildItem $hklm -ErrorAction SilentlyContinue)) {
            $dir = (Get-ItemProperty $key.PSPath -ErrorAction SilentlyContinue).InstalledDirectory
            Add-Candidate $dir "registry HKLM ($($key.PSChildName))"
        }
    }

    # 4 — registry, source builds
    $hkcu = "HKCU:\SOFTWARE\Epic Games\Unreal Engine\Builds"
    if (Test-Path $hkcu) {
        $props = Get-ItemProperty $hkcu -ErrorAction SilentlyContinue
        foreach ($p in $props.PSObject.Properties) {
            if ($p.Name -notlike "PS*") { Add-Candidate $p.Value "registry HKCU source build" }
        }
    }

    # 5 — bounded filesystem probe
    $drives = (Get-PSDrive -PSProvider FileSystem -ErrorAction SilentlyContinue |
               Where-Object { $_.Root -match "^[A-Z]:\\$" }).Root
    foreach ($d in $drives) {
        foreach ($pattern in @(
            "Program Files\Epic Games\UE_*",
            "Epic Games\UE_*",
            "Epic\UE_*",
            "Games\Epic Games\UE_*",
            "UE_*",
            "Unreal\UE_*",
            "UnrealEngine*"
        )) {
            foreach ($hit in (Get-Item (Join-Path $d $pattern) -ErrorAction SilentlyContinue)) {
                Add-Candidate $hit.FullName "found on disk"
            }
        }
    }

    # 6 — pruned sweep of every fixed drive. Catches installs in folders nobody
    #     could guess, e.g. E:\Games\Engines\MyUE58.
    #     Top level is filtered first so we never descend into Windows, then each
    #     surviving branch is walked 2 levels deep. -Recurse is explicit because
    #     Windows PowerShell 5.1 ignores -Depth without it.
    if ($candidates.Count -eq 0 -or $DeepScan) {
        # Single quotes: '$Recycle.Bin' must NOT be treated as a PowerShell variable.
        $SkipTop = @('Windows', '$Recycle.Bin', 'System Volume Information',
                     'Recovery', 'PerfLogs', 'OneDriveTemp', 'Program Files (x86)')
        foreach ($d in $drives) {
            $top = @()
            try {
                $top = Get-ChildItem -Path $d -Directory -Force -ErrorAction SilentlyContinue |
                       Where-Object { $SkipTop -notcontains $_.Name }
            } catch { }

            foreach ($branch in $top) {
                if (Test-UnrealEngineRoot $branch.FullName) {
                    Add-Candidate $branch.FullName "disk sweep ($d)"
                    continue
                }
                $subs = @()
                try {
                    $subs = Get-ChildItem -Path $branch.FullName -Directory -Recurse -Depth 2 -Force -ErrorAction SilentlyContinue
                } catch { }
                foreach ($sub in $subs) {
                    if (Test-Path (Join-Path $sub.FullName "Engine\Build\BatchFiles\Build.bat")) {
                        Add-Candidate $sub.FullName "disk sweep ($d)"
                    }
                }
            }
        }
    }

    return $candidates
}

<#
    Picks the 5.8 install. Returns $null if this machine has no 5.8.
#>
function Resolve-UE58Root {
    param([string]$Preferred, [switch]$DeepScan)

    if ($Preferred) {
        if (-not (Test-UnrealEngineRoot $Preferred)) {
            Write-Warning "'$Preferred' does not look like an Unreal install (no Engine\Build\BatchFiles\Build.bat)."
            return $null
        }
        return (Resolve-Path $Preferred).Path
    }

    $all = @(Find-UnrealEngines -DeepScan:$DeepScan)
    $five8 = $all | Where-Object { $_.Version -and $_.Version.StartsWith("5.8") } | Select-Object -First 1
    if ($five8) { return $five8.Path }

    # Quick pass found engines, but no 5.8 among them. It may be on a data drive
    # in a folder nobody would guess. Sweep before giving up.
    if (-not $DeepScan) {
        Write-Host " No 5.8 in the quick scan - sweeping all drives (up to a minute) ..." -ForegroundColor DarkGray
        $all = @(Find-UnrealEngines -DeepScan)
        $five8 = $all | Where-Object { $_.Version -and $_.Version.StartsWith("5.8") } | Select-Object -First 1
        if ($five8) { return $five8.Path }
    }

    # Fall back to a path that merely looks like 5.8 even if Version.h is unreadable.
    $named = $all | Where-Object { $_.Path -match "UE_?5\.8" } | Select-Object -First 1
    if ($named) { return $named.Path }

    return $null
}

function Show-UnrealEngines {
    param([switch]$DeepScan)
    $all = @(Find-UnrealEngines -DeepScan:$DeepScan)
    if ($all.Count -eq 0) {
        Write-Host " No Unreal Engine installation was detected on this PC." -ForegroundColor Red
        return
    }
    Write-Host " Unreal installations detected:"
    foreach ($e in $all) {
        $mark = if ($e.Version -and $e.Version.StartsWith("5.8")) { "  <-- 5.8, use this" } else { "" }
        $ver  = if ($e.Version) { $e.Version } else { "unknown" }
        Write-Host ("   {0,-9} {1}{2}" -f $ver, $e.Path, $mark)
        Write-Host ("             source: {0}" -f $e.Source) -ForegroundColor DarkGray
    }
}
