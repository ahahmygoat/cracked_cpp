# Docs index — Learn C++, OOP, and design (noob → principal level)

This folder is your **single entry point** for all written docs. We teach **syntax**, **OOP**, **design**, and **system thinking** so you can read like a noob and reason like a principal engineer or PM.

**How to use these docs:** Start with **Who this is for** (below) to pick your path. Use **Docs by category** to jump to a theme. Follow **Learning path** for a recommended order. Use **Doc map** for a one-line description of every doc. All paths point back here; each doc ends with **Related docs** so you can branch out.

---

## Who this is for

| You are… | Start here |
|----------|------------|
| **Noob** — new to C++ or programming | [cpp-basics.md](cpp-basics.md) → [cpp-classes.md](cpp-classes.md) → [ClassesandData.md](ClassesandData.md). Read "What is…" and "Why…"; ignore jargon until you hit it. |
| **Want the "why"** — design choices, tradeoffs | [DESIGN.md](DESIGN.md) first, then skim code comments and doc callouts marked **Principal** / **Tradeoff**. |
| **Principal / staff** — evaluating structure and rationale | [DESIGN.md](DESIGN.md), [README.md](../README.md) (design philosophy), and **Main design choices** in README. |
| **PM / product** — what we build first, when to refactor | [DESIGN.md](DESIGN.md) (sections "PM lens" and "When to refactor"), [ORDERBOOK.md](ORDERBOOK.md) (domain). |

---

## Project layout (current codebase)

- **src/** — All C++ source (main.cpp, refactorMain.cpp, MerkelMain, OrderBookEntry, CSVReader). Build from repo root with **-Isrc**.
- **scripts/** — Build scripts: `build-main.ps1`, `build-OrderBookEntry.ps1`, `build-MerkelMain.ps1`. Run from repo root (e.g. `.\scripts\build-MerkelMain.ps1`).
- **data/** — CSV data (e.g. order_book_example.csv). Paths in code are relative to current working directory (run from repo root).
- **docs/** — This folder. Start with [INDEX.md](INDEX.md); full layout: [project-layout.md](project-layout.md).

---

## Docs by category

Use this to jump to a theme instead of reading in order.

| Category | Docs |
|----------|------|
| **Getting started** | [SETUP.md](SETUP.md) (run, build, compilers), [project-layout.md](project-layout.md) (src/, scripts/, how to build each target), [windows-gcc-setup.md](windows-gcc-setup.md) (Windows: g++ not found). |
| **C++ syntax & concepts** | [cpp-basics.md](cpp-basics.md) (functions, variables, control flow), [cpp-classes.md](cpp-classes.md) (class vs object), [ClassesandData.md](ClassesandData.md) (types, struct/class, vectors, error handling), [simple-classes-and-vectors.md](simple-classes-and-vectors.md) (Vec3D, vector of objects), [vector-iteration.md](vector-iteration.md) (index, iterator, range-for; value vs ref vs const ref), [constructors-and-initialization.md](constructors-and-initialization.md) (default, parameterized, copy, move, = default, = delete), [tokenizer.md](tokenizer.md) (split by delimiter; CSVReader), [exception-handling.md](exception-handling.md) (try/catch, file open, stod). |
| **OOP & design** | [oop-concepts.md](oop-concepts.md) (encapsulation, inheritance, polymorphism, **static members**), [organizing-code.md](organizing-code.md) (header = spec, limiting exposure, embedding init), [headers-and-cpp.md](headers-and-cpp.md) (.h/.cpp split, include guards, extern), [DESIGN.md](DESIGN.md) (single responsibility, data vs behavior, when to refactor, PM lens). |
| **Domain & worksheet** | [ORDERBOOK.md](ORDERBOOK.md) (order book, bids/asks, CSV format), [orderbook-matching.md](orderbook-matching.md) (how matching works, using the data), [orderbook-worksheet.md](orderbook-worksheet.md) (teaching steps tied to OrderBookEntry), [merkel-main.md](merkel-main.md) (MerkelMain app, init/run, build). |
| **Reference** | [git-github-cheatsheet.md](git-github-cheatsheet.md) (clone, commit, push, branches). |

---

## Learning path (recommended order)

1. **[SETUP.md](SETUP.md)** — Run the app, build, compilers, IDE. Get something running first.
2. **[project-layout.md](project-layout.md)** — Where source, scripts, and data live; how to build each target.
3. **[cpp-basics.md](cpp-basics.md)** — Functions, variables, control flow. No prior C++ assumed.
4. **[cpp-classes.md](cpp-classes.md)** — Class vs object, defining a type, creating instances.
5. **[ClassesandData.md](ClassesandData.md)** — Types, struct vs class, bits/sizeof, data structures, error handling.
6. **[ORDERBOOK.md](ORDERBOOK.md)** — Domain: what an order book is, how the CSV is structured.
7. **[headers-and-cpp.md](headers-and-cpp.md)** — Header files and the .h/.cpp pattern: declarations vs definitions, include guards, extern.
8. **[oop-concepts.md](oop-concepts.md)** — Software engineering: encapsulation, inheritance, polymorphism. Tied to OrderBookEntry, MerkelMain.
9. **[vector-iteration.md](vector-iteration.md)** — Iterating vectors: index, iterator, range-for; by value vs reference vs const reference (snippets).
10. **[constructors-and-initialization.md](constructors-and-initialization.md)** — Constructors: default, parameterized, copy, move, delegating, = default, = delete; member init list; emplace_back vs push_back.
11. **[organizing-code.md](organizing-code.md)** — Organizing code: header = spec, .cpp = impl, namespacing, include guards, limiting exposure (private orders_), embedding init.
12. **[DESIGN.md](DESIGN.md)** — OOP, design, system thinking, when to refactor, PM lens. Read after you can read the code.
13. **[windows-gcc-setup.md](windows-gcc-setup.md)** — Windows: fix "g++ not recognized" (install compiler, PATH).
14. **[git-github-cheatsheet.md](git-github-cheatsheet.md)** — Git and GitHub commands.

---

## Doc map (one-line descriptions)

Alphabetical by filename. For grouped by category, see **Docs by category** above.

| Doc | What it teaches |
|-----|------------------|
| **ClassesandData.md** | Types, struct/class, **static members and static functions**, bits/sizeof, vectors, error handling, roadmap to wallet. |
| **constructors-and-initialization.md** | Many ways to create constructors: default, parameterized, copy, move, delegating, = default, = delete; member init list; emplace_back vs push_back. |
| **cpp-basics.md** | Functions (including `main`), variables, control flow (if/while/switch). |
| **cpp-classes.md** | Class vs object, defining a class, data members, creating and using objects. |
| **DESIGN.md** | OOP and design in plain language: single responsibility, data vs behavior, when to refactor, tradeoffs, PM lens. |
| **exception-handling.md** | Try/catch, when to use exceptions vs checks; file open (is_open), std::stod; tie to CSVReader.cpp (src/) and refactorMain.cpp (src/). |
| **headers-and-cpp.md** | Header files and .h/.cpp pattern: declarations vs definitions, include guards, extern. OrderBookEntry, MerkelMain, CSVReader (all in src/). |
| **INDEX.md** (this file) | How to use the docs; noob vs principal vs PM paths; docs by category; learning path; doc map. |
| **merkel-main.md** | MerkelMain: exchange app class (init, run, menu loop), OrderBookEntry integration, encapsulation, build with OrderBookEntry.cpp + CSVReader.cpp. |
| **oop-concepts.md** | Software engineering: encapsulation, inheritance, polymorphism, **static members and utility-style design**. Tied to OrderBookEntry, MerkelMain, CSVReader. |
| **orderbook-matching.md** | How order book matching works (bids vs asks, bid ≥ ask); using the CSV data and OrderBook / getOrders / matchOrders. |
| **orderbook-worksheet.md** | Teaching steps from 2313_v3.pdf: class definition, constructor, vector of objects, range-for, const ref, challenge (computeAveragePrice etc.). Ties worksheet to OrderBookEntry.h and OrderBookEntry.cpp. |
| **organizing-code.md** | Organizing code: header = spec, .cpp = impl, namespacing, include guards, limiting exposure (private orders_), embedding init. Tied to MerkelMain. |
| **ORDERBOOK.md** | Domain: order book, bids/asks, matching engine, CSV format. |
| **project-layout.md** | Project layout: src/, scripts/, data/, docs/; how to build each target; current C++ codebase as context. |
| **SETUP.md** | Run, build, compilers (Win/Mac/Linux), PATH, scripts, Git, troubleshooting. |
| **simple-classes-and-vectors.md** | Syntax for simple classes (Vec3D) and putting objects into a std::vector (push_back, entries[i]). |
| **tokenizer.md** | Splitting strings into tokens: std::getline(stream, token, delimiter) pattern; exception handling for tokenize/stringsToOBE; tie to CSVReader.cpp (src/). |
| **vector-iteration.md** | Three ways to iterate over a vector: index, iterator, range-for; range-for by value vs reference vs const reference (snippets). |
| **windows-gcc-setup.md** | Windows: download MSYS2, install g++, add PATH, run `g++` from terminal. |
| **git-github-cheatsheet.md** | Git and GitHub: clone, commit, push, branches, undo. |

---

## How we write (principal-engineer / PM style)

- **Teach, don’t just show** — We explain *why* something exists and what happens if you change it.
- **Noob-friendly** — Jargon is explained; we use analogies (e.g. class = blueprint, object = the thing).
- **Design choices are explicit** — When we pick one approach (e.g. `enum class` over `const int`), we say so and state the tradeoff.
- **Principal lens** — We call out intent, alternatives, and when to tighten design (e.g. private data, headers).
- **PM lens** — We say what to build first, when refactoring pays off, and when to escalate (e.g. "if product needs X, we’d add Y").

Use **INDEX.md** to choose your path; use **DESIGN.md** to think in OOP and design; use the rest to learn C++ and the project step by step.

---

## Related

- **[README.md](../README.md)** — Project overview, how to run, learning path, design philosophy (repo root).
- **[SETUP.md](../SETUP.md)** — Quick setup at repo root; full setup in [SETUP.md](SETUP.md) (this folder).
