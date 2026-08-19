@echo off
REM  The Betrayed Will — report what character content this engine install has.
REM  Run this when ADD_CHARACTERS says it found nothing.

setlocal
powershell -NoProfile -ExecutionPolicy Bypass -Command ^
  "Get-ChildItem '%~dp0*.ps1' | Unblock-File -ErrorAction SilentlyContinue; exit 0"

powershell -NoProfile -ExecutionPolicy Bypass -File "%~dp0FIND_CHARACTERS.ps1" %*

echo.
pause
exit /b %ERRORLEVEL%
