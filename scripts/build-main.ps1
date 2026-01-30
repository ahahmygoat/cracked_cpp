# Build and run main.cpp (simple menu, single main).
# Usage: .\scripts\build-main.ps1   or from scripts: .\build-main.ps1
# If g++ not found: install MSYS2, run pacman -S mingw-w64-ucrt-x86_64-gcc, add bin to PATH.
# See docs/windows-gcc-setup.md.

$ErrorActionPreference = "Stop"
$repoRoot = (Split-Path $PSScriptRoot -Parent)
Set-Location $repoRoot

$mingwPaths = @("C:\msys64\ucrt64\bin", "C:\msys64\mingw64\bin")
foreach ($p in $mingwPaths) {
    if (Test-Path "$p\g++.exe") { $env:PATH = "$p;$env:PATH"; break }
}

$src = "src/main.cpp"
$out = "main.exe"

if (-not (Get-Command g++ -ErrorAction SilentlyContinue)) {
    Write-Host "ERROR: g++ not found. See docs/windows-gcc-setup.md." -ForegroundColor Red
    exit 1
}

Write-Host "===== Build ($src) =====" -ForegroundColor Cyan
& g++ -std=c++17 -Wall -g -Isrc -o $out $src
if ($LASTEXITCODE -ne 0) { Write-Host "Build failed." -ForegroundColor Red; exit $LASTEXITCODE }

Write-Host "===== Run =====" -ForegroundColor Cyan
& ".\$out"
exit $LASTEXITCODE
