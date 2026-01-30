# C++ Exchange Simulator — Learning Project

A small C++ console app that simulates an exchange menu (help, stats, offer, bid, wallet, continue). The goal is **to learn C++ and good practices** by building something real, with **documentation and code that teach**.

---

## Who this is for

- **Beginners** — Concepts are explained so you can follow even if you're new to C++.
- **Anyone who wants "why"** — We document **design choices** and **tradeoffs**, not just "what" the code does.
- **Principal-engineer style** — Comments and docs explain intent, alternatives, and when to change things later.

---

## How to read this codebase (noob-friendly)

1. **Start with the docs** — [docs/cpp-basics.md](docs/cpp-basics.md) explains functions, variables, and control flow. No prior C++ assumed.
2. **Project layout** — All C++ source is in **src/**; build scripts are in **scripts/**. See [docs/project-layout.md](docs/project-layout.md).
3. **Then open the code** — In **src/refactorMain.cpp** or **src/main.cpp**, read the **block comment at the top**. It explains DESIGN, WHY we use certain includes, and links to other entry points (MerkelMain, OrderBookEntry, CSVReader).
4. **As you read each section** — Look for comments that say **DESIGN**, **WHY**, or **Tradeoff**. Those explain the "principal engineer" choices so a noob can get it.
5. **When you're lost** — Check the **Learning path** list below; the docs and code are cross-linked so you can jump to the right concept.

---

## How to run

- **Windows:** `.\run.ps1` (builds and runs **src/main.cpp** → main.exe)
- **macOS / Linux:** `./run.sh` or `bash run.sh` (builds and runs **src/main.cpp** → main)

Other targets (from repo root):

- **OrderBookEntry demo:** `.\scripts\build-OrderBookEntry.ps1` (Windows)
- **MerkelMain (exchange loop with CSV):** `.\scripts\build-MerkelMain.ps1` (Windows)

Or build manually from repo root: `g++ -std=c++17 -Isrc -o main.exe src/main.cpp` then `.\main.exe`.  
Full setup (compilers, PATH, IDE): **[SETUP.md](SETUP.md)** → [docs/SETUP.md](docs/SETUP.md).  
**Project layout (src/, scripts/):** [docs/project-layout.md](docs/project-layout.md).  
**Windows: g++ not working?** → [docs/windows-gcc-setup.md](docs/windows-gcc-setup.md) (download compiler, set PATH, run `g++`).

---

## Learning path (docs in order)

1. **[SETUP.md](SETUP.md)** — Index of all docs; run the app.
2. **[docs/INDEX.md](docs/INDEX.md)** — Doc map and paths: noob, principal, PM. Start here to pick your path.
3. **[docs/cpp-basics.md](docs/cpp-basics.md)** — Functions, variables, control flow (if/while/switch). Start here if you're new.
4. **[docs/cpp-classes.md](docs/cpp-classes.md)** — What a class is, class vs object, defining a type and creating instances.
5. **[docs/ClassesandData.md](docs/ClassesandData.md)** — Types, struct/class, bits/sizeof, data structures.
6. **[docs/ORDERBOOK.md](docs/ORDERBOOK.md)** — What an order book is, how the dataset is structured (CSV).
7. **[docs/DESIGN.md](docs/DESIGN.md)** — OOP, design, system thinking, when to refactor, PM lens (principal-engineer level).
8. **[docs/SETUP.md](docs/SETUP.md)** — Full setup: compilers, PATH, scripts, Git, troubleshooting.
9. **[docs/git-github-cheatsheet.md](docs/git-github-cheatsheet.md)** — Git and GitHub commands.

---

## Design philosophy (how we document and code)

- **Teach, don't just show** — Comments and docs explain _why_ something is there and what would happen if you changed it.
- **Noob-friendly** — Jargon is explained; we use analogies (e.g. class = blueprint, object = the actual thing).
- **Design choices are explicit** — When we pick one approach (e.g. `enum class` over `const int`, or public data for simplicity), we say so and mention the tradeoff.
- **Simple first, refine later** — We start with the simplest thing that works (e.g. public members, one file). Docs point out when to tighten design (e.g. private data + getters/setters, header files).
- **Cross-platform** — Code and setup target Windows, macOS, and Linux; we use the standard library and avoid platform-specific tricks unless noted.

---

## Main design choices (summary)

| Choice                                             | Why                                                                 | Tradeoff / later step                                                 |
| -------------------------------------------------- | ------------------------------------------------------------------- | --------------------------------------------------------------------- |
| **main.cpp: one main(), inline logic**             | Easiest to read: one file, one flow.                                | Refactor into functions (refactorMain.cpp) when the file gets long.   |
| **refactorMain: functions per action**             | One job per function; main() just orchestrates.                     | Keeps main readable and makes testing easier.                         |
| **enum class for menu options**                    | Type-safe; no magic numbers; clear intent.                          | Slightly more verbose than `const int`; worth it.                     |
| **Pass by reference for validateUserOption(int&)** | So we can update the caller's variable when they re-enter.          | By value would not update main's userOption.                          |
| **Order struct with public data**                  | Simplest way to group price, amount, timestamp, product, orderType. | Later: private data + constructors + getters if you need invariants.  |
| **cin.fail() handling**                            | Non-numeric input would leave garbage and break the loop.           | We clear and ignore so the next read works; user gets a fresh prompt. |
| **run.ps1 sets PATH for the session**              | So g++ finds its DLLs on Windows without requiring global PATH.     | For daily use, add MinGW to user PATH once (see docs/SETUP.md).       |

More detail lives in the code comments and in the docs linked above.

---

## Project layout (high level)

- **src/** — All C++ source. Build from repo root with **-Isrc**.
  - **main.cpp** — Simple menu: one `main()`, inline menu + validation + actions. Best to read first.
  - **refactorMain.cpp** — Same menu refactored: functions, `enum class`, `Order` struct.
  - **MerkelMain.cpp/.h** — Class-based app: init() loads CSV via CSVReader, run() is the menu loop.
  - **OrderBookEntry.cpp/.h** — Order book entry type, Format helpers, printOrderBook*, compute* (stats).
  - **CSVReader.cpp/.h** — CSV loading (readCSV); tokenize and parse with try/catch.
- **scripts/** — Build scripts (PowerShell): build-main.ps1, build-OrderBookEntry.ps1, build-MerkelMain.ps1. Run from repo root.
- **docs/** — All written docs (INDEX, SETUP, project-layout, cpp-basics, DESIGN, etc.).
- **data/** — Example order book CSV (`order_book_example.csv`). Paths in code are relative to current working directory (run from repo root).

Full layout and build commands: [docs/project-layout.md](docs/project-layout.md).

---

## Contributing to the docs

When you add or change code:

- **Comment the "why"** — What problem this solves, why this approach, what to do instead if requirements change.
- **Comment for a noob** — Assume the reader hasn't seen the idiom before; one line of context helps.
- **Call out design choices** — "We use X so that Y; alternative is Z but we don't do that because …."

This keeps the repo useful as a teaching codebase.
