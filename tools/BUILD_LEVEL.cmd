@echo off
REM ============================================================================
REM  The Betrayed Will / وصية الغدر
REM
REM  DOUBLE-CLICK THIS to build the palace level.
REM
REM  Runs the level generator inside a real editor process with no window,
REM  saves /Game/TBW/Maps/L_VS_Palace_EastWing, points the project at it, and
REM  verifies it with the self test. No editor menus, no manual steps.
REM ============================================================================

setlocal
powershell -NoProfile -ExecutionPolicy Bypass -Command ^
  "Get-ChildItem '%~dp0*.ps1' | Unblock-File -ErrorAction SilentlyContinue; exit 0"

powershell -NoProfile -ExecutionPolicy Bypass -File "%~dp0BUILD_LEVEL.ps1" %*

echo.
pause
exit /b %ERRORLEVEL%
