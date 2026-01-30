# Wrapper: run the real build script in scripts/
# Usage from repo root: .\build-MerkelMain.ps1
& (Join-Path $PSScriptRoot "scripts\build-MerkelMain.ps1")
exit $LASTEXITCODE
