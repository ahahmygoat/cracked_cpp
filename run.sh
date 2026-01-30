#!/usr/bin/env bash
# Build and run main.cpp on macOS / Linux. Source lives in src/; build from repo root with -Isrc.
# Usage: ./run.sh   (or: bash run.sh)
# See docs/project-layout.md and docs/SETUP.md.

set -e
cd "$(dirname "$0")"

CXX="${CXX:-g++}"
SRC="src/main.cpp"
OUT="main"

if command -v "$CXX" &>/dev/null; then
  echo "===== Build ($CXX) ====="
  "$CXX" -std=c++17 -Wall -g -Isrc -o "$OUT" "$SRC"
  echo "===== Run ====="
  ./"$OUT"
else
  echo "error: No C++ compiler found. Install g++ or clang++ and ensure it is on PATH." >&2
  exit 1
fi
