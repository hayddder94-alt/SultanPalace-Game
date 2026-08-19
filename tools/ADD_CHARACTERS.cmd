@echo off
REM ============================================================================
REM  The Betrayed Will / وصية الغدر
REM
REM  DOUBLE-CLICK THIS to give the game real character bodies.
REM
REM  Copies the engine's own mannequin content into the project - the same thing
REM  "Add Feature or Content Pack > Third Person" does, without the menus.
REM  No download, no account, no licence question.
REM ============================================================================

setlocal
powershell -NoProfile -ExecutionPolicy Bypass -Command ^
  "Get-ChildItem '%~dp0*.ps1' | Unblock-File -ErrorAction SilentlyContinue; exit 0"

powershell -NoProfile -ExecutionPolicy Bypass -File "%~dp0ADD_CHARACTERS.ps1" %*

echo.
pause
exit /b %ERRORLEVEL%
