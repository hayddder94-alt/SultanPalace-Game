@echo off
REM The Betrayed Will / وصية الغدر — locate Unreal Engine 5.8 (double-click me)
REM Looks in the Epic Launcher manifest, the registry, and the usual folders on
REM every drive. Reports only. Pass -Remember to store UE58_ROOT for your user.

setlocal
echo.
echo === The Betrayed Will - find Unreal Engine 5.8 ===
echo.

powershell -NoProfile -ExecutionPolicy Bypass -Command ^
  "Get-ChildItem '%~dp0*.ps1' | Unblock-File -ErrorAction SilentlyContinue; exit 0"

powershell -NoProfile -ExecutionPolicy Bypass -File "%~dp0find_ue58.ps1" %*

set EXITCODE=%ERRORLEVEL%
echo.
echo (exit code %EXITCODE%)
echo.
pause
exit /b %EXITCODE%
