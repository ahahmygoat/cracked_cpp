# Project layout: folders and where things live

This doc describes **how the repo is organized** so you can find source, scripts, data, and docs. It reflects the **current C++ codebase**: `src/` for source, `scripts/` for build scripts, `data/` for CSV, `docs/` for documentation.

**Takeaway:** All C++ source lives in **`src/`**. Build from the **repo root** with **`-Isrc`** so `#include "OrderBookEntry.h"` and similar resolve. **`.\run.ps1`** (Windows) builds and runs **MerkelMain** (output **build/MerkelMain.exe**). Use **`.\scripts\run-main.ps1`** for main.exe, **`.\scripts\build-OrderBookEntry.ps1`** for the OrderBookEntry demo.

---

## 1. Folder structure (repo root)

| Folder / file   | Purpose |
|-----------------|---------|
| **src/**        | All C++ source (`.cpp`, `.h`). Single place for application code. |
| **scripts/**    | Build scripts (PowerShell). Each script builds one target from repo root with `-Isrc`. |
| **data/**       | Input data (e.g. `order_book_example.csv`). Paths in code are relative to **current working directory** (run from repo root). |
| **docs/**       | Documentation (markdown). Learning path, design, setup, C++ concepts. See [INDEX.md](INDEX.md). |
| **run.ps1**     | Build and run MerkelMain → **build/MerkelMain.exe** (Windows). Uses `-Isrc`. Run from repo root so data/ is found. |
| **run.sh**      | Build and run `src/main.cpp` → `main` (macOS/Linux). Uses `-Isrc`. |
| **README.md**   | Project overview, how to run, learning path, design philosophy. |
| **SETUP.md**    | Quick setup; points to docs/SETUP.md for full details. |

---

## 2. What’s in `src/`

| File | Role |
|------|------|
| **main.cpp** | Simplest entry point: single `main()`, one pass through the menu (no loop). No OrderBookEntry/CSVReader. |
| **refactorMain.cpp** | Same menu with a **loop**; logic split into functions (printMenu, getUserOption, validateUserOption, handleUserOption) and enum class MenuOption. Includes cin.fail() handling. |
| **MerkelMain.cpp**, **MerkelMain.h** | Class-based app: `init()` loads order book via **OrderBook::load(path)**, sets **currentTimestamp_** to earliest; `run()` is the menu loop. Private **orderBook_** (OrderBook) and **currentTimestamp_**. Option 2 = stats for **current time window**; option 6 = advance to next time. Defines its own `main()`. |
| **OrderBook.cpp**, **OrderBook.h** | Order book: entries by (product, timestamp). **load()**, **getOrders**, **matchOrders**, **getBestBid**, **getBestAsk**, **getAllEntries**, **getAllEntriesAtTime**, **getEarliestTime**, **getLatestTime**, **getNextTime**, **getPreviousTime**. |
| **OrderBookEntry.cpp**, **OrderBookEntry.h** | Order book entry type (price, amount, timestamp, product, orderType), Format helpers, global `orders`, printOrderBook*, compute* (average/low/high/spread), time helpers (getEarliestTime, getLatestTime, getNextTime, getPreviousTime over a vector). Defines its own `main()` for the OrderBookEntry demo when built with ORDERBOOK_STANDALONE. |
| **CSVReader.cpp**, **CSVReader.h** | CSV loading: `readCSV(path)` returns vector, `readCSV(path, out)` fills a vector and returns count. Tokenizes by comma (see [tokenizer.md](tokenizer.md)), try/catch for stod (see [exception-handling.md](exception-handling.md)). |

**Include style:** All `#include "OrderBookEntry.h"`, `#include "CSVReader.h"`, etc. assume headers are found via **`-Isrc`** when compiling from repo root. Do not use `#include "src/OrderBookEntry.h"`.

---

## 3. Build scripts (`scripts/`)

Scripts run from **repo root** (they set `$repoRoot = (Split-Path $PSScriptRoot -Parent)` and `Set-Location $repoRoot`). They add MSYS2 MinGW to PATH if present, then invoke `g++` with **`-Isrc`** and the right `.cpp` files.

| Script | Builds | Output | Command (from repo root) |
|--------|--------|--------|---------------------------|
| **scripts/build-main.ps1** | `src/main.cpp` | `main.exe` | `.\scripts\build-main.ps1` |
| **scripts/build-OrderBookEntry.ps1** | `src/OrderBookEntry.cpp` + `src/CSVReader.cpp` | `OrderBookEntry.exe` | `.\scripts\build-OrderBookEntry.ps1` |
| **scripts/build-MerkelMain.ps1** | `src/MerkelMain.cpp` + `src/OrderBookEntry.cpp` + `src/OrderBook.cpp` + `src/CSVReader.cpp` | **build/MerkelMain.exe** | `.\run.ps1` or `.\scripts\build-MerkelMain.ps1` |

**MerkelMain** outputs to **build/MerkelMain.exe** so the exe in the repo root is not locked; if you see "Permission denied" when linking, close any running MerkelMain.exe and rebuild.

**Manual build examples (from repo root):**

```powershell
# main (simple menu)
g++ -std=c++17 -Wall -g -Isrc -o main.exe src/main.cpp

# OrderBookEntry demo
g++ -std=c++17 -Wall -g -Isrc -o OrderBookEntry.exe src/OrderBookEntry.cpp src/CSVReader.cpp

# MerkelMain (full exchange loop with OrderBook, time stepping)
g++ -std=c++17 -Wall -g -Isrc -o build/MerkelMain.exe src/MerkelMain.cpp src/OrderBookEntry.cpp src/OrderBook.cpp src/CSVReader.cpp
```

---

## 4. Data and working directory

- **data/order_book_example.csv** — Used by OrderBookEntry demo and MerkelMain. Path in code is `"data/order_book_example.csv"` (relative to **current working directory**).
- **Run from repo root** so that `data/` is found. The scripts (`run.ps1`, `run.sh`, `scripts/build-*.ps1`) set the working directory to the repo root before building and running.

---

## 5. Docs and code (context)

The docs describe the **current** layout and code:

- **INDEX.md** — Doc map and learning path; points to project layout and SETUP.
- **SETUP.md** — How to run (run.ps1, run.sh, scripts), compilers, PATH. Refers to `src/` and `scripts/`.
- **headers-and-cpp.md** — .h/.cpp split; examples use OrderBookEntry, MerkelMain, CSVReader (all in `src/`).
- **organizing-code.md** — Limiting exposure, private orders_, init embedding load; MerkelMain uses CSVReader::readCSV.
- **tokenizer.md** — CSV tokenizing; implementation in **CSVReader.cpp** (`tokenize`, `readCSVInto`).
- **exception-handling.md** — try/catch, file open, stod; implementation in **CSVReader.cpp** (`readCSVInto`).
- **merkel-main.md** — MerkelMain app; build with OrderBookEntry.cpp + CSVReader.cpp (from `src/`).

When a doc says “OrderBookEntry.cpp” or “CSVReader.cpp”, it means **`src/OrderBookEntry.cpp`** and **`src/CSVReader.cpp`** in this layout.

---

## 6. Quick reference

| I want to… | Do this |
|------------|---------|
| Run the simple menu (main.cpp) | `.\run.ps1` (Windows) or `./run.sh` (macOS/Linux) |
| Build and run main from scripts | `.\scripts\build-main.ps1` |
| Build and run OrderBookEntry demo | `.\scripts\build-OrderBookEntry.ps1` |
| Build and run MerkelMain | `.\scripts\build-MerkelMain.ps1` |
| Find all C++ source | Look in **src/** |
| Find build scripts | Look in **scripts/** |
| Find docs | Look in **docs/**; start with [INDEX.md](INDEX.md). |

---

---

## Related docs

- [SETUP.md](SETUP.md) — Full setup: compilers, PATH, IDE.
- [DESIGN.md](DESIGN.md) — Design and OOP context.
- [INDEX.md](INDEX.md) — Doc map and learning path.
