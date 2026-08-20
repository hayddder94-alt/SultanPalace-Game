@echo off
REM ============================================================================
REM  The Betrayed Will / وصية الغدر
REM
REM  ONE COMMAND: pull, remove the broken plugin copy, compile, rebuild the
REM  level, and launch the game standalone.
REM
REM    .\tools\PLAY.cmd              everything, then play
REM    .\tools\PLAY.cmd -Editor      open the editor instead of the game
REM    .\tools\PLAY.cmd -SkipBuild   go straight to playing
REM ============================================================================

setlocal
powershell -NoProfile -ExecutionPolicy Bypass -Command ^
  "Get-ChildItem '%~dp0*.ps1' | Unblock-File -ErrorAction SilentlyContinue; exit 0"

powershell -NoProfile -ExecutionPolicy Bypass -File "%~dp0PLAY.ps1" %*

echo.
pause
exit /b %ERRORLEVEL%
