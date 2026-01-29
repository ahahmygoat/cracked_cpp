# Build and run main.cpp on Windows.
# Usage: .\run.ps1   (or: pwsh -File run.ps1)

$ErrorActionPreference = "Stop"
Set-Location $PSScriptRoot

$src = "main.cpp"
$out = "main.exe"

# Prefer MinGW on PATH so g++ can find its DLLs
$env:PATH = "C:\msys64\mingw64\bin;$env:PATH"

$compiler = $null
if (Get-Command g++ -ErrorAction SilentlyContinue) { $compiler = "g++" }
elseif (Get-Command clang++ -ErrorAction SilentlyContinue) { $compiler = "clang++" }

if (-not $compiler) {
  Write-Error "No C++ compiler found. Install MinGW (MSYS2) or Clang and add its bin folder to PATH."
  exit 1
}

Write-Host "===== Build ($compiler) ====="
& $compiler -std=c++17 -g -o $out $src
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }

Write-Host "===== Run ====="
& ".\$out"
