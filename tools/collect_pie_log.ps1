# The Betrayed Will / وصية الغدر — pull the Phase 2 evidence out of the editor log
#
# Run this AFTER a PIE session. It reads Saved\Logs\TheBetrayedWill.log and prints
# only what the Phase 2 checklist cares about, in a form you can paste into chat.
#
# Usage:
#   .\tools\collect_pie_log.cmd
#   .\tools\collect_pie_log.cmd -Full        also list every warning/error line

param(
    [switch]$Full,
    [string]$LogFile
)

$ErrorActionPreference = "Stop"
$ProjectRoot = (Resolve-Path (Join-Path $PSScriptRoot "..")).Path
$LogDir      = Join-Path $ProjectRoot "Saved\Logs"

if (-not $LogFile) {
    $LogFile = Join-Path $LogDir "TheBetrayedWill.log"
    if (-not (Test-Path $LogFile)) {
        $newest = Get-ChildItem -Path $LogDir -Filter "*.log" -ErrorAction SilentlyContinue |
                  Sort-Object LastWriteTime -Descending | Select-Object -First 1
        if ($newest) { $LogFile = $newest.FullName }
    }
}

if (-not (Test-Path $LogFile)) {
    Write-Host "No editor log found under $LogDir." -ForegroundColor Red
    Write-Host "Launch the editor with .\tools\run_editor.cmd, play once, then run this again."
    exit 1
}

$log = @(Get-Content $LogFile -ErrorAction SilentlyContinue)

Write-Host ""
Write-Host "=============================================================="
Write-Host " PHASE 2 PIE EVIDENCE  (copy everything below into the chat)"
Write-Host "=============================================================="
Write-Host "Log file : $LogFile"
Write-Host "Lines    : $($log.Count)"
Write-Host ""

function Show-Section {
    param([string]$Title, [string]$Pattern, [int]$Max = 30)
    $hits = @($log | Select-String -Pattern $Pattern | Select-Object -Last $Max)
    Write-Host "---- $Title ($($hits.Count)) ----"
    if ($hits.Count -eq 0) { Write-Host "  (none)" }
    foreach ($h in $hits) { Write-Host "  $($h.Line.Trim())" }
    Write-Host ""
}

# Which GPU actually rendered. On an Optimus laptop the editor can silently
# land on the Intel iGPU instead of the Quadro, which turns an empty map into
# 12 FPS. This is the first thing to read before optimising anything.
Show-Section -Title "GPU / RHI adapter"    -Pattern "Adapter Name|Adapter Driver|Driver Version|DeviceId|GPU Adapter|Using .*RHI|RHI Init"

# Which map was really open. The debug HUD used to hard-code its map name.
Show-Section -Title "Map actually loaded"  -Pattern "LoadMap|Bringing World|Game class is"

Show-Section -Title "Shader compilation"   -Pattern "ShaderCompilers?: .*(Compiling|Submitting|finished)|Shaders left"

Show-Section -Title "TBW runtime log"      -Pattern "LogTBW"
Show-Section -Title "Startup / version"    -Pattern "The Betrayed Will .* starting|Engine lock|L_VS_Palace|L_Dev_Sandbox"
Show-Section -Title "Interaction"          -Pattern "Interact:|Test door"
Show-Section -Title "Font / text shaping"  -Pattern "SlateFont|DroidSans|Arabic|HarfBuzz|missing glyph"
Show-Section -Title "Mobility complaints"  -Pattern "has to be 'Movable'"
Show-Section -Title "Errors"               -Pattern "Error:|LogOutputDevice: Error"

if ($Full) {
    Show-Section -Title "All warnings" -Pattern "Warning:" -Max 60
}

Write-Host "=============================================================="
Write-Host "Also paste your filled rows from docs\PHASE2_PIE_CHECKLIST.md,"
Write-Host "including the FPS numbers from the debug HUD and 'stat unit'."
Write-Host "=============================================================="
exit 0
