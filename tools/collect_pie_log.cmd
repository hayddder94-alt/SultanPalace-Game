@echo off
REM The Betrayed Will / وصية الغدر — wrapper around collect_pie_log.ps1 (double-click me)
REM Windows blocks unsigned .ps1 files; this bypasses that for this process only.

setlocal
echo.
powershell -NoProfile -ExecutionPolicy Bypass -Command ^
  "Get-ChildItem '%~dp0*.ps1' | Unblock-File -ErrorAction SilentlyContinue; exit 0"

powershell -NoProfile -ExecutionPolicy Bypass -File "%~dp0collect_pie_log.ps1" %*

set EXITCODE=%ERRORLEVEL%
echo.
echo (exit code %EXITCODE%)
echo.
pause
exit /b %EXITCODE%
