@echo off
REM ============================================================================
REM  The Betrayed Will / وصية الغدر
REM
REM  WHICH GPU IS DRAWING THE GAME?
REM
REM  One command. It reads the hardware, reads Unreal's own logs, and says
REM  whether the engine landed on the Quadro or on the Intel chip.
REM
REM    .\tools\GPU.cmd
REM ============================================================================

setlocal
powershell -NoProfile -ExecutionPolicy Bypass -Command ^
  "Get-ChildItem '%~dp0*.ps1' | Unblock-File -ErrorAction SilentlyContinue; exit 0"

powershell -NoProfile -ExecutionPolicy Bypass -File "%~dp0GPU.ps1" %*

echo.
pause
exit /b %ERRORLEVEL%
