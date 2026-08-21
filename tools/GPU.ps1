# ============================================================================
#  The Betrayed Will / وصية الغدر — WHICH GPU IS DRAWING THE GAME?
#
#  I have asked for this ten times and never made it easy, which is my fault.
#  One command. No interpretation needed. It answers three separate questions:
#
#     1. what graphics hardware is in this laptop
#     2. which adapter Unreal actually chose, from its own logs
#     3. whether that is the fast one
#
#  Point 2 is the one that matters. An Optimus laptop has two GPUs and Windows
#  decides which one an application gets. If Unreal landed on the Intel chip,
#  every performance number so far has been measured on the wrong hardware.
#
#  Usage:
#    .\tools\GPU.cmd
# ============================================================================

$ErrorActionPreference = "Continue"
. (Join-Path $PSScriptRoot "ue58_common.ps1")

$ProjectRoot = (Resolve-Path (Join-Path $PSScriptRoot "..")).Path
$LogDir      = Join-Path $ProjectRoot "Saved\Logs"

$report = New-Object System.Collections.Generic.List[string]
function Say($t, $c = "Gray") { Write-Host $t -ForegroundColor $c; $report.Add($t) }

Clear-Host
Write-Host ""
Write-Host "============================================================" -ForegroundColor Cyan
Write-Host "   WHICH GPU IS DRAWING THE GAME?" -ForegroundColor Cyan
Write-Host "============================================================" -ForegroundColor Cyan
Write-Host ""

# ------------------------------------------------- 1. what is in the machine
Say " 1. GRAPHICS HARDWARE IN THIS MACHINE"
Say ""
$cards = @(Get-CimInstance Win32_VideoController -ErrorAction SilentlyContinue)
if ($cards.Count -eq 0) {
    Say "    could not read Win32_VideoController." "Yellow"
} else {
    foreach ($c in $cards) {
        # AdapterRAM is a 32-bit field and lies above 4 GB, so it is reported
        # as-is rather than dressed up as a fact.
        $vram = if ($c.AdapterRAM -gt 0) {
            "{0:N0} MB reported" -f ($c.AdapterRAM / 1MB)
        } else { "unknown" }
        Say ("    {0}" -f $c.Name)
        Say ("       driver {0}   video memory: {1}" -f $c.DriverVersion, $vram)
    }
}
Say ""

# ------------------------------------------- 2. what Unreal actually picked
Say " 2. WHAT UNREAL CHOSE  (from its own logs)"
Say ""

$logs = @()
if (Test-Path $LogDir) {
    $logs = @(Get-ChildItem $LogDir -Filter "*.log" -File -ErrorAction SilentlyContinue |
              Sort-Object LastWriteTime -Descending)
}

if ($logs.Count -eq 0) {
    Say "    No logs under $LogDir." "Yellow"
    Say "    Run .\tools\PLAY.cmd once, close the game, then run this again."
} else {
    # Every line Unreal writes that names the adapter it selected. The exact
    # wording moves between engine versions and between the D3D11 and D3D12
    # RHIs, so this matches on the several forms rather than on one.
    $pattern = "Adapter Name|Adapter Driver|Chosen D3D12 Adapter|Selected D3D12 Adapter|" +
               "GPU Adapter|RHI Adapter|Adapter\s*:|DeviceId|VendorId|Driver Version|" +
               "Using D3D12|Using D3D11|Initializing RHI|GPU:"

    $found = $false
    foreach ($log in ($logs | Select-Object -First 8)) {
        $hits = @(Select-String -Path $log.FullName -Pattern $pattern -ErrorAction SilentlyContinue |
                  Select-Object -First 14)
        if ($hits.Count -eq 0) { continue }
        $found = $true
        Say ("    --- {0}   ({1:yyyy-MM-dd HH:mm}) ---" -f $log.Name, $log.LastWriteTime)
        foreach ($h in $hits) {
            $line = $h.Line.Trim()
            # Strip Unreal's timestamp and frame counter so the useful half fits.
            $line = $line -replace '^\[[\d\.\-:]+\]\[\s*\d+\]', ''
            Say ("      {0}" -f $line.Trim())
        }
        Say ""
        break
    }
    if (-not $found) {
        Say "    None of the $($logs.Count) log(s) name an adapter." "Yellow"
        Say "    That happens when every run so far used -nullrhi, which draws"
        Say "    nothing at all. Run .\tools\PLAY.cmd - it launches for real."
        Say ""
    }
}

# ------------------------------------------------------------ 3. the verdict
Say " 3. VERDICT"
Say ""

$blob = ""
foreach ($log in ($logs | Select-Object -First 8)) {
    $blob += (Get-Content $log.FullName -TotalCount 4000 -ErrorAction SilentlyContinue) -join "`n"
}

$hasNvidia  = $blob -match "(?i)NVIDIA|Quadro|GeForce|RTX|GTX"
$hasIntel   = $blob -match "(?i)Intel\(R\) HD|Intel\(R\) UHD|Intel\(R\) Iris|HD Graphics 530"
$nullRhi    = $blob -match "(?i)nullrhi"

if ($hasNvidia -and -not $hasIntel) {
    Say "    Unreal is rendering on the NVIDIA card. This is correct." "Green"
    Say "    Frame rates measured here are real, and slow means slow."
} elseif ($hasIntel -and -not $hasNvidia) {
    Say "    Unreal is rendering on the INTEL INTEGRATED chip." "Red"
    Say ""
    Say "    That is the problem, and it is not a small one - the integrated" "Red"
    Say "    chip is several generations of performance behind the Quadro." "Red"
    Say ""
    Say "    Fix it once, in NVIDIA Control Panel:"
    Say "      Manage 3D Settings > Program Settings > Add"
    Say "      E:\UE_5.8\Engine\Binaries\Win64\UnrealEditor.exe"
    Say "      Preferred processor: High-performance NVIDIA processor > Apply"
    Say ""
    Say "    Windows 10 also has its own switch that can override that one:"
    Say "      Settings > System > Display > Graphics settings"
    Say "      Add UnrealEditor.exe > Options > High performance"
} elseif ($hasIntel -and $hasNvidia) {
    Say "    Both names appear in the logs. Read the 'Adapter Name' line in" "Yellow"
    Say "    section 2 above - that one is the adapter actually chosen; the" "Yellow"
    Say "    other is usually just the enumeration listing every device." "Yellow"
} elseif ($nullRhi) {
    Say "    Every run so far was -nullrhi, which renders nothing." "Yellow"
    Say "    The self test uses that deliberately. Run .\tools\PLAY.cmd to"
    Say "    get a real render, then run this again."
} else {
    Say "    Not enough evidence yet. Run .\tools\PLAY.cmd, close the game," "Yellow"
    Say "    then run this again."
}

Say ""
Say "============================================================"
Write-Host ""

Copy-TBWReport -Title "GPU check" -Text ($report -join "`r`n") | Out-Null
exit 0
