# Build and run MerkelMain (MerkelMain + OrderBookEntry + OrderBook + CSVReader).
# Output: build/MerkelMain.exe (avoids "Permission denied" when exe in root is locked).
# Usage: .\run.ps1 or .\scripts\build-MerkelMain.ps1 from repo root.
# If g++ not found: install MSYS2, run pacman -S mingw-w64-ucrt-x86_64-gcc, add bin to PATH.
# See docs/windows-gcc-setup.md.

$ErrorActionPreference = "Stop"
$repoRoot = (Split-Path $PSScriptRoot -Parent)
Set-Location $repoRoot

# Output to build/ so data/ is found when run from repo root; avoids locking root exe.
if (-not (Test-Path "build")) { New-Item -ItemType Directory -Path "build" | Out-Null }
$out = "build/MerkelMain.exe"

$mingwPaths = @("C:\msys64\ucrt64\bin", "C:\msys64\mingw64\bin")
foreach ($p in $mingwPaths) {
    if (Test-Path "$p\g++.exe") { $env:PATH = "$p;$env:PATH"; break }
}

$src = @("src/MerkelMain.cpp", "src/OrderBookEntry.cpp", "src/OrderBook.cpp", "src/CSVReader.cpp")

if (-not (Get-Command g++ -ErrorAction SilentlyContinue)) {
    Write-Host "ERROR: g++ not found. Install MSYS2 and add its bin folder to PATH." -ForegroundColor Red
    Write-Host "  See docs/windows-gcc-setup.md for step-by-step." -ForegroundColor Yellow
    Write-Host "  Run from repo root: .\run.ps1  or  .\scripts\build-MerkelMain.ps1" -ForegroundColor Yellow
    exit 1
}

Write-Host "===== Build ($($src -join ', ')) =====" -ForegroundColor Cyan
& g++ -std=c++17 -Wall -g -Isrc -o $out $src
if ($LASTEXITCODE -ne 0) { Write-Host "Build failed." -ForegroundColor Red; exit $LASTEXITCODE }

Write-Host "===== Run (cwd = repo root so data/ is found) =====" -ForegroundColor Cyan
& ".\$out"
exit $LASTEXITCODE
