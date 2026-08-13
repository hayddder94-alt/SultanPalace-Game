#!/usr/bin/env bash
# Windows Development package must be run on Windows with UE 5.7.
# This shell wrapper only prints the requirement so Linux CI does not pretend.
set -euo pipefail
echo "The Betrayed Will Windows Development build cannot be produced on this host."
echo "Install Unreal Engine 5.7 on Windows and run:"
echo "  powershell -File tools/package_win_dev.ps1"
exit 2
