# The Betrayed Will / وصية الغدر — launch the editor for the Phase 2 PIE pass
#
# Usage:
#   .\tools\run_editor.cmd
#   .\tools\run_editor.cmd -EngineRoot "E:\UE_5.8"
#   .\tools\run_editor.cmd -Wait        keep this window and wait for the editor to close
#
# After you finish the PIE pass, run .\tools\collect_pie_log.cmd to pull the
# relevant lines out of the editor log.

param(
    [string]$EngineRoot = $env:UE58_ROOT,
    [switch]$Wait
)

$ErrorActionPreference = "Stop"
. (Join-Path $PSScriptRoot "ue58_common.ps1")

$ProjectRoot = (Resolve-Path (Join-Path $PSScriptRoot "..")).Path
$UProject    = Join-Path $ProjectRoot "TheBetrayedWill.uproject"

$EngineRoot = Resolve-UE58Root -Preferred $EngineRoot
if (-not $EngineRoot) {
    Write-Host "Unreal Engine 5.8 not found. Run .\tools\find_ue58.cmd first." -ForegroundColor Red
    exit 1
}

$Editor = Join-Path $EngineRoot "Engine\Binaries\Win64\UnrealEditor.exe"
if (-not (Test-Path $Editor)) {
    Write-Host "UnrealEditor.exe not found at $Editor" -ForegroundColor Red
    exit 1
}

Write-Host ""
Write-Host "=============================================================="
Write-Host " LAUNCHING THE EDITOR"
Write-Host "=============================================================="
Write-Host " Editor  : $Editor"
Write-Host " Project : $UProject"
Write-Host " Log     : $ProjectRoot\Saved\Logs\TheBetrayedWill.log"
Write-Host ""
Write-Host " In the editor: press Play (PIE), then work through"
Write-Host "   docs\PHASE2_PIE_CHECKLIST.md"
Write-Host ""
Write-Host " Useful console commands during PIE (press the tilde key):"
Write-Host "   tbw.Version          tbw.Debug.Hud        tbw.Flags.List"
Write-Host "   tbw.Flags.Get TestDoorOpen                tbw.Debug.Reset"
Write-Host "   tbw.Identity.Set Raynor                   tbw.Identity.Set Evan"
Write-Host "   stat unit            stat fps"
Write-Host ""

$proc = Start-Process -FilePath $Editor -ArgumentList "`"$UProject`"", "-log" -PassThru

if ($Wait) {
    Write-Host " Waiting for the editor to close ..."
    $proc.WaitForExit()
    Write-Host " Editor closed. Now run: .\tools\collect_pie_log.cmd"
} else {
    Write-Host " Editor started (pid $($proc.Id)). This window can be closed."
    Write-Host " When you are done testing, run: .\tools\collect_pie_log.cmd"
}
exit 0
