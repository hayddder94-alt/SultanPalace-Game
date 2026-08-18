@echo off
REM The Betrayed Will / وصية الغدر — Windows setup (double-click me)
REM
REM Clones the project from GitHub into C:\Dev\SultanPalace-Game (or updates it if
REM it is already there), checks out the session branch, and reports where the
REM authoritative project file lives. Does NOT build. Does NOT touch your old copies.
REM
REM Usage: double-click, or from PowerShell:
REM   .\tools\setup_windows.cmd
REM   .\tools\setup_windows.cmd -Root "D:\Dev" -Build

setlocal
echo.
echo === The Betrayed Will - Windows setup ===
echo.

powershell -NoProfile -ExecutionPolicy Bypass -Command ^
  "Get-ChildItem '%~dp0*.ps1' | Unblock-File -ErrorAction SilentlyContinue; exit 0"

powershell -NoProfile -ExecutionPolicy Bypass -File "%~dp0windows_bootstrap.ps1" %*

set EXITCODE=%ERRORLEVEL%
echo.
echo (exit code %EXITCODE%)
echo.
pause
exit /b %EXITCODE%
