@echo off
REM ============================================================================
REM  The Betrayed Will / وصية الغدر
REM
REM  Report what is really inside a downloaded model folder, and above all
REM  whether the mesh is RIGGED. An unrigged mesh is a statue.
REM
REM    .\tools\INSPECT_MODEL.cmd "C:\Users\dell\Downloads\Tripo3d_Blender_Bridge"
REM ============================================================================

setlocal
powershell -NoProfile -ExecutionPolicy Bypass -Command ^
  "Get-ChildItem '%~dp0*.ps1' | Unblock-File -ErrorAction SilentlyContinue; exit 0"

powershell -NoProfile -ExecutionPolicy Bypass -File "%~dp0INSPECT_MODEL.ps1" %*

echo.
pause
exit /b %ERRORLEVEL%
