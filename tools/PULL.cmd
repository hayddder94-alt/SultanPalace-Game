@echo off
REM ============================================================================
REM  The Betrayed Will / وصية الغدر
REM
REM  Pull the latest code even when a plain `git pull` refuses because the
REM  editor rewrote a tracked file. Diagnoses first, then does the safe thing.
REM
REM    .\tools\PULL.cmd            stash local edits, pull, put them back
REM    .\tools\PULL.cmd -Discard   throw local edits away, then pull
REM    .\tools\PULL.cmd -DryRun    diagnose only, change nothing
REM ============================================================================

setlocal
powershell -NoProfile -ExecutionPolicy Bypass -Command ^
  "Get-ChildItem '%~dp0*.ps1' | Unblock-File -ErrorAction SilentlyContinue; exit 0"

powershell -NoProfile -ExecutionPolicy Bypass -File "%~dp0PULL.ps1" %*

echo.
pause
exit /b %ERRORLEVEL%
