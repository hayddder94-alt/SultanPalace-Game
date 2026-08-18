@echo off
REM The Betrayed Will / وصية الغدر — Phase 2 build (double-click me)
REM
REM Windows blocks unsigned .ps1 files by default:
REM   "... is not digitally signed. You cannot run this script on the current system."
REM This wrapper launches the PowerShell script with that policy bypassed for THIS
REM process only. Nothing on your machine is changed permanently.
REM
REM Usage: double-click this file, or from PowerShell:
REM   .\tools\build_phase2.cmd
REM   .\tools\build_phase2.cmd -EngineRoot "D:\Epic Games\UE_5.8"

setlocal
echo.
echo === The Betrayed Will - Phase 2 build ===
echo.

powershell -NoProfile -ExecutionPolicy Bypass -Command ^
  "Get-ChildItem '%~dp0*.ps1' | Unblock-File -ErrorAction SilentlyContinue; exit 0"

powershell -NoProfile -ExecutionPolicy Bypass -File "%~dp0phase2_build_and_check.ps1" %*

set EXITCODE=%ERRORLEVEL%
echo.
echo (exit code %EXITCODE%)
echo.
pause
exit /b %EXITCODE%
