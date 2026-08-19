@echo off
REM ============================================================================
REM  The Betrayed Will / وصية الغدر
REM
REM  DOUBLE-CLICK THIS FILE. That is the whole instruction.
REM
REM  It pulls the latest code, builds, runs the automated test, and opens one
REM  report in Notepad for you to copy into the chat.
REM ============================================================================

setlocal
powershell -NoProfile -ExecutionPolicy Bypass -Command ^
  "Get-ChildItem '%~dp0*.ps1' | Unblock-File -ErrorAction SilentlyContinue; exit 0"

powershell -NoProfile -ExecutionPolicy Bypass -File "%~dp0GO.ps1" %*

echo.
pause
exit /b %ERRORLEVEL%
