# Build and run main.cpp on Windows. Source lives in src/; build from repo root with -Isrc.
# Usage: .\run.ps1   (or: pwsh -File run.ps1)
# See docs/project-layout.md and docs/SETUP.md.

$ErrorActionPreference = "Stop"
Set-Location $PSScriptRoot

$src = "src/main.cpp"
$out = "main.exe"

# Try common MSYS2 MinGW locations (ucrt64 first, then mingw64) so g++ can find its DLLs
foreach ($p in @("C:\msys64\ucrt64\bin", "C:\msys64\mingw64\bin")) {
  if (Test-Path "$p\g++.exe") { $env:PATH = "$p;$env:PATH"; break }
}

$compiler = $null
if (Get-Command g++ -ErrorAction SilentlyContinue) { $compiler = "g++" }
elseif (Get-Command clang++ -ErrorAction SilentlyContinue) { $compiler = "clang++" }

if (-not $compiler) {
  Write-Host "No C++ compiler found. See docs/windows-gcc-setup.md: install MSYS2, run pacman -S mingw-w64-ucrt-x86_64-gcc, add bin to PATH." -ForegroundColor Yellow
  exit 1
}

Write-Host "===== Build ($compiler) ====="
& $compiler -std=c++17 -Wall -g -Isrc -o $out $src
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }

Write-Host "===== Run ====="
& ".\$out"
