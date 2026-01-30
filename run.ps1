# Default run: build and run MerkelMain (exchange app). Usage: .\run.ps1
# Other: .\scripts\run-main.ps1 (main.exe), .\scripts\build-OrderBookEntry.ps1 (OrderBookEntry demo).
& (Join-Path $PSScriptRoot "scripts\build-MerkelMain.ps1")
exit $LASTEXITCODE
