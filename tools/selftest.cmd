@echo off
REM The Betrayed Will / وصية الغدر — headless system verification (double-click me)
REM Runs a real Unreal process with no window and no GPU, executes tbw.SelfTest,
REM and prints a verdict. Build first with tools\build_phase2.cmd.

setlocal
echo.
echo === The Betrayed Will - headless self test ===
echo.

powershell -NoProfile -ExecutionPolicy Bypass -Command ^
  "Get-ChildItem '%~dp0*.ps1' | Unblock-File -ErrorAction SilentlyContinue; exit 0"

powershell -NoProfile -ExecutionPolicy Bypass -File "%~dp0selftest.ps1" %*

set EXITCODE=%ERRORLEVEL%
echo.
echo (exit code %EXITCODE%)
echo.
pause
exit /b %EXITCODE%
