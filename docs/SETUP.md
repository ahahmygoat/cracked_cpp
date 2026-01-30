# C++ Project Setup

Initial setup and multi-compiler support for **Windows**, **macOS**, and **Linux**. Source lives in **src/**; build from repo root with **-Isrc**.

**Project layout:** All C++ source is in **src/** (main.cpp, refactorMain.cpp, MerkelMain, OrderBookEntry, CSVReader). Build scripts are in **scripts/** (build-main.ps1, build-OrderBookEntry.ps1, build-MerkelMain.ps1). See [project-layout.md](project-layout.md) for the full layout and how to build each target.

**Documentation style:** We document _why_ (design choices, tradeoffs) as well as _what_. Code comments and [README.md](../README.md) (at repo root) explain intent and when to change things. Aimed so a noob can follow and a principal engineer can see the rationale.

**Principal takeaway:** Build and run are the first feedback loop. Scripts (run.ps1, run.sh, scripts/build-*.ps1) set PATH so g++ works without global config; that's a deliberate tradeoff (simplicity for learners, one-time PATH for daily use). Full doc index and design/OOP: [INDEX.md](INDEX.md), [DESIGN.md](DESIGN.md).

---


## 1. Quick run (all platforms)

From the project folder:

| Platform                 | Command                     |
| ------------------------ | --------------------------- |
| **Windows** (PowerShell) | `.\run.ps1`                 |
| **macOS / Linux**        | `./run.sh` or `bash run.sh` |

You should see the exchange menu (Welcome to the exchange, options 1–6).

---

## 2. Flow: how to test / run C++

### Options at a glance

| What you want             | Windows                                | macOS / Linux                        |
| ------------------------- | -------------------------------------- | ------------------------------------ |
| Run the project default   | `.\run.ps1`                            | `./run.sh`                           |
| Run **any** single `.cpp` | `g++ file.cpp -o file` then `.\file`   | `g++ file.cpp -o file` then `./file` |
| Build only (no run)       | `g++ main.cpp -o main`                 | `g++ main.cpp -o main`               |
| Build + run in IDE        | **Run Build Task** → **Run** or **F5** | Same                                 |

### What `run.ps1` does (Windows)

1. **Changes to the script's folder** so it always runs from the project root.
2. **Puts MinGW on PATH** for that session (`C:\msys64\ucrt64\bin` or `mingw64\bin`) so `g++` can find its DLLs.
3. **Picks a compiler**: tries `g++`, then `clang++` (first one found).
4. **Builds** `src/main.cpp` → `main.exe` with `-std=c++17 -Wall -g -Isrc`.
5. **Runs** `.\main.exe`.

So `run.ps1` only ever builds and runs **src/main.cpp**. To run other targets (OrderBookEntry demo, MerkelMain), use **scripts/build-OrderBookEntry.ps1** or **scripts/build-MerkelMain.ps1** (see [project-layout.md](project-layout.md)).

### What `run.sh` does (macOS / Linux)

1. **Changes to the script's folder** (project root).
2. **Picks a compiler**: `g++` by default, or `clang++` if you set `CXX=clang++` before running.
3. **Builds** `src/main.cpp` → `main` with `-std=c++17 -Wall -g -Isrc`.
4. **Runs** `./main`.

So `run.sh` only ever builds and runs **src/main.cpp**. To run other targets, build from repo root with `-Isrc` and the right .cpp files (see [project-layout.md](project-layout.md)).

### Run any single `.cpp` file (vanilla)

Replace `file` with your source name (no `.cpp` in the executable name).

**Windows (PowerShell):**

```powershell
g++ file.cpp -o file
.\file
```

**macOS / Linux:**

```bash
g++ file.cpp -o file
./file
```

**Default executable name (no `-o`):**

- Windows: `g++ file.cpp` → produces `a.exe` → run `.\a.exe`
- Mac/Linux: `g++ file.cpp` → produces `a.out` → run `./a.out`

**With warnings and C++17:**

```powershell
g++ file.cpp -o file -std=c++17 -Wall -g
.\file
```

---

## 3. Prerequisites by platform

### Windows

- **Terminal**: PowerShell or Command Prompt.
- **Compiler**: At least one of:
  - **GCC (MinGW)** — [MSYS2](https://www.msys2.org/) → install, then: `pacman -S mingw-w64-ucrt-x86_64-gcc`
  - **Clang** — [LLVM](https://releases.llvm.org/) or MSYS2: `pacman -S mingw-w64-ucrt-x86_64-clang`
  - **MSVC** — [Visual Studio Build Tools](https://visualstudio.microsoft.com/visual-cpp-build-tools/) (optional; use Developer PowerShell)
- **PATH**: Add the compiler's `bin` folder to your user PATH, e.g. `C:\msys64\mingw64\bin` for MSYS2 MinGW.

**Add MinGW to your User PATH (permanent)** — so you don't need to set `$env:PATH` each time:

- **Option A — GUI**
  1. Press **Win**, type **environment**, open **Edit environment variables for your account**.
  2. Under **User variables**, select **Path** → **Edit**.
  3. **New** → enter: `C:\msys64\mingw64\bin`
  4. **OK** on all dialogs.
  5. Close and reopen your terminal (or Cursor).

- **Option B — PowerShell (run once)**  
  Run in PowerShell (no admin; adds to your **user** PATH):

  ```powershell
  $current = [Environment]::GetEnvironmentVariable("Path", "User")
  if ($current -notlike "*mingw64*") {
    [Environment]::SetEnvironmentVariable("Path", "C:\msys64\mingw64\bin;$current", "User")
    Write-Host "Added MinGW to user PATH. Close and reopen the terminal."
  } else {
    Write-Host "MinGW already on PATH."
  }
  ```

  After that, in any **new** terminal you can run `g++ main.cpp -o main.exe` and `.\main.exe` without setting PATH first.

### macOS

- **Terminal**: built-in Terminal or iTerm.
- **Compiler**: Install Xcode Command Line Tools (includes Clang):
  ```bash
  xcode-select --install
  ```
  Or install GCC via Homebrew: `brew install gcc`
- **PATH**: Usually no change; `clang++` and `g++` are in default PATH after install.

### Linux

- **Terminal**: any shell (bash, etc.).
- **Compiler**: Install GCC or Clang:

  ```bash
  # Debian/Ubuntu
  sudo apt update && sudo apt install build-essential

  # Fedora
  sudo dnf install gcc-c++

  # Arch
  sudo pacman -S base-devel
  ```

- **PATH**: Usually no change after package install.

---

## 4. Project layout

```
cracked_c++/
├── .gitignore              # Git: ignore exe, a.out, build dirs, OS/IDE junk
├── .vscode/
│   ├── launch.json         # Run/Debug configs (Windows + macOS/Linux)
│   └── tasks.json          # Build tasks (GCC, MSVC, Clang)
├── main.cpp                # Entry point (simple menu)
├── refactorMain.cpp        # Refactored menu + Order struct (order book row)
├── hello.cpp               # Optional hello-world style file
├── run.sh                  # Build & run on macOS / Linux
├── run.ps1                 # Build & run on Windows
├── README.md               # Start here: learning path, design philosophy
├── SETUP.md                # Root index (links to all docs)
├── docs/                   # All written documentation
│   ├── SETUP.md            # Full C++ setup (this file)
│   ├── cpp-basics.md       # Functions, variables, control flow
│   ├── cpp-classes.md      # Classes: definition, instance, object
│   ├── ClassesandData.md   # Types, classes, bits/sizeof
│   ├── ORDERBOOK.md        # Order book and dataset overview
│   ├── git-github-cheatsheet.md   # Git + GitHub cheat sheet
│   └── notes-intro.txt     # Optional learning notes
└── data/
    └── order_book_example.csv  # Example order book dataset (for simulation)
```

---

## 5. Git

### First-time setup (this repo)

From the project folder:

```bash
git init
git add .
git commit -m "Initial: C++ setup, run scripts, SETUP"
```

`.gitignore` is already in place so `*.exe`, `a.out`, `main`, build dirs, and common IDE/OS files are not committed.

### Clone and run (someone else's repo)

```bash
git clone <repo-url>
cd cracked_c++
.\run.ps1
```

(On macOS/Linux: `./run.sh` or `bash run.sh`.)

### Daily flow

| What you want    | Command                           |
| ---------------- | --------------------------------- |
| See status       | `git status`                      |
| Stage changes    | `git add .` or `git add main.cpp` |
| Commit           | `git commit -m "Your message"`    |
| Push (if remote) | `git push`                        |
| Pull updates     | `git pull`                        |

**Full cheat sheet:** see [git-github-cheatsheet.md](git-github-cheatsheet.md).

### What's ignored (`.gitignore`)

- **Build outputs**: `*.exe`, `*.out`, `a.out`, `a.exe`, `main`, `main.exe`, `*.o`, `*.obj`, etc.
- **IDE**: most of `.vscode/` (tasks and launch are kept), `.idea/`
- **OS**: `.DS_Store`, `Thumbs.db`, `desktop.ini`
- **Build dirs**: `build/`, `out/`, `cmake-build-*/`

So you can build and run locally without committing executables or junk.

---

## 6. Build & run by platform

All commands assume you are in the project root.

### Windows

**Option A — Run script (easiest)**

```powershell
.\run.ps1
```

**Option B — Manual (GCC or Clang)**

Ensure MinGW or Clang is on PATH (e.g. `C:\msys64\mingw64\bin`), then:

```powershell
g++ -std=c++17 -g -o main.exe main.cpp
.\main.exe
```

Or with Clang:

```powershell
clang++ -std=c++17 -g -o main.exe main.cpp
.\main.exe
```

**Option C — MSVC (Developer PowerShell)**

```cmd
cl /EHsc /Zi /Fe:main.exe main.cpp
main.exe
```

### macOS / Linux

**Option A — Run script (easiest)**

```bash
chmod +x run.sh
./run.sh
```

Or without chmod: `bash run.sh`

**Option B — Manual (GCC or Clang)**

```bash
g++ -std=c++17 -Wall -g -o main main.cpp
./main
```

Or with Clang:

```bash
clang++ -std=c++17 -Wall -g -o main main.cpp
./main
```

**Use a specific compiler via env:**

```bash
CXX=clang++ ./run.sh
```

---

## 7. Supported compilers (all platforms)

| Compiler  | Windows        | macOS               | Linux                         |
| --------- | -------------- | ------------------- | ----------------------------- |
| **GCC**   | MinGW (MSYS2)  | Homebrew `gcc`      | `build-essential` / `gcc-c++` |
| **Clang** | LLVM or MSYS2  | Xcode CLT (default) | `clang` package               |
| **MSVC**  | VS Build Tools | —                   | —                             |

- **Windows**: executable is `main.exe`.
- **macOS / Linux**: executable is `main` (no extension).

---

## 8. VS Code / Cursor tasks

Use **Terminal → Run Build Task** (or **Ctrl+Shift+B** / **Cmd+Shift+B**).

- **Build (GCC)** — default on Windows (MinGW) or use `g++` on PATH on Mac/Linux.
- **Build (MSVC)** — Windows only; use Developer PowerShell.
- **Build (Clang)** — any platform where `clang++` is on PATH.

On macOS/Linux, the built executable is `main` (no `.exe`). On Windows it is `main.exe`.

---

## 9. Debugging

- **Windows**: Use the **Run main.exe** config; it builds with GCC then runs.
- **macOS / Linux**: Use the **Run main (macOS/Linux)** config; it builds then runs `main`.

Ensure the C/C++ extension is installed. Build with debug info (`-g` or `/Zi`) is already set in the tasks.

---

## 10. Troubleshooting

| Issue                                  | What to try                                                                                   |
| -------------------------------------- | --------------------------------------------------------------------------------------------- |
| `g++` / `clang++` not found            | Add the compiler's `bin` folder to PATH (Windows: System Properties → Environment Variables). |
| Windows: g++ fails or DLL errors       | Add `C:\msys64\mingw64\bin` to PATH, or run `.\run.ps1` (script sets PATH for that run).      |
| `cl` not found (Windows)               | Open **Developer PowerShell for VS** or run `vcvarsall.bat x64`.                              |
| Mac: `xcode-select` errors             | Run `xcode-select --install` and accept the license.                                          |
| Linux: permission denied on `./run.sh` | Run `chmod +x run.sh` or use `bash run.sh`.                                                   |
| Wrong C++ standard                     | Use `-std=c++17` (GCC/Clang) or `/std:c++17` (MSVC).                                          |

---

## Summary

- **Run default**: Windows → `.\run.ps1`; macOS/Linux → `./run.sh` (builds and runs `main.cpp` only).
- **Run any `.cpp`**: `g++ file.cpp -o file` then `.\file` (Windows) or `./file` (macOS/Linux).
- **Scripts**: `run.ps1` / `run.sh` set the folder, pick g++ or clang++, build one file, run it; they do not take a filename — use manual `g++` for other files.
- **Git**: See [git-github-cheatsheet.md](git-github-cheatsheet.md). `.gitignore` keeps exe, a.out, build dirs, and IDE/OS junk out of the repo.
- **IDE**: **Run Build Task** builds the active file; **F5** or **Run** uses `launch.json` to build and run.
- **Multi-compiler**: GCC, Clang, or (Windows) MSVC; keep the chosen compiler on PATH.

---

## Related docs

- **[INDEX.md](INDEX.md)** — Doc map, learning path, docs by category.
- **[project-layout.md](project-layout.md)** — src/, scripts/, how to build each target (main, OrderBookEntry, MerkelMain).
- **[windows-gcc-setup.md](windows-gcc-setup.md)** — Windows: fix "g++ not recognized" (MSYS2, PATH).
- **[git-github-cheatsheet.md](git-github-cheatsheet.md)** — Git and GitHub commands.
- **[README.md](../README.md)** — Project overview (repo root).
