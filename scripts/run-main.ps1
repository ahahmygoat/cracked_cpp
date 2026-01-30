# Build and run main.exe (main.cpp). Run from repo root: .\scripts\run-main.ps1
# See docs/project-layout.md and docs/SETUP.md.

$ErrorActionPreference = "Stop"
$repoRoot = (Split-Path $PSScriptRoot -Parent)
Set-Location $repoRoot

$src = "src/main.cpp"
$out = "main.exe"

foreach ($p in @("C:\msys64\ucrt64\bin", "C:\msys64\mingw64\bin")) {
    if (Test-Path "$p\g++.exe") { $env:PATH = "$p;$env:PATH"; break }
}

$compiler = $null
if (Get-Command g++ -ErrorAction SilentlyContinue) { $compiler = "g++" }
elseif (Get-Command clang++ -ErrorAction SilentlyContinue) { $compiler = "clang++" }

if (-not $compiler) {
    Write-Host "No C++ compiler found. See docs/windows-gcc-setup.md." -ForegroundColor Yellow
    exit 1
}

Write-Host "===== Build ($compiler) =====" -ForegroundColor Cyan
& $compiler -std=c++17 -Wall -g -Isrc -o $out $src
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }

Write-Host "===== Run =====" -ForegroundColor Cyan
& ".\$out"
