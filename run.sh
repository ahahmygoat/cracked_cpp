#!/usr/bin/env bash
# Build and run main.cpp on macOS / Linux.
# Usage: ./run.sh   (or: bash run.sh)

set -e
cd "$(dirname "$0")"

CXX="${CXX:-g++}"
SRC="main.cpp"
OUT="main"

if command -v "$CXX" &>/dev/null; then
  echo "===== Build ($CXX) ====="
  "$CXX" -std=c++17 -Wall -g -o "$OUT" "$SRC"
  echo "===== Run ====="
  ./"$OUT"
else
  echo "error: No C++ compiler found. Install g++ or clang++ and ensure it is on PATH." >&2
  exit 1
fi
