# C++ Project Setup

Initial setup and multi-compiler support for **Windows**, **macOS**, and **Linux**. Same `main.cpp` runs on all three.

---

## 1. Quick run (all platforms)

From the project folder:

| Platform                 | Command                     |
| ------------------------ | --------------------------- |
| **Windows** (PowerShell) | `.\run.ps1`                 |
| **macOS / Linux**        | `./run.sh` or `bash run.sh` |

You should see: `Hello World`

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

1. **Changes to the script’s folder** so it always runs from the project root.
2. **Puts MinGW on PATH** for that session (`C:\msys64\mingw64\bin`) so `g++` can find its DLLs.
3. **Picks a compiler**: tries `g++`, then `clang++` (first one found).
4. **Builds** `main.cpp` → `main.exe` with `-std=c++17 -g`.
5. **Runs** `.\main.exe`.

So `run.ps1` only ever builds and runs **`main.cpp`**. To run another file, use the manual flow below.

### What `run.sh` does (macOS / Linux)

1. **Changes to the script’s folder** (project root).
2. **Picks a compiler**: `g++` by default, or `clang++` if you set `CXX=clang++` before running.
3. **Builds** `main.cpp` → `main` with `-std=c++17 -Wall -g`.
4. **Runs** `./main`.

So `run.sh` only ever builds and runs **`main.cpp`**. To run another file, use the manual flow below.

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
- **PATH**: Add the compiler’s `bin` folder to your user PATH, e.g. `C:\msys64\mingw64\bin` for MSYS2 MinGW.

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
├── .gitignore         # Git: ignore exe, a.out, build dirs, OS/IDE junk
├── .vscode/
│   ├── launch.json    # Run/Debug configs (Windows + macOS/Linux)
│   └── tasks.json     # Build tasks (GCC, MSVC, Clang)
├── main.cpp           # Entry point (portable C++)
├── run.sh             # Build & run on macOS / Linux
├── run.ps1            # Build & run on Windows
└── SETUP.md           # This file
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

### Clone and run (someone else’s repo)

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

### What’s ignored (`.gitignore`)

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
| `g++` / `clang++` not found            | Add the compiler’s `bin` folder to PATH (Windows: System Properties → Environment Variables). |
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
- **Git**: `git init` then `git add .` and `git commit`; `.gitignore` keeps exe, a.out, build dirs, and IDE/OS junk out of the repo.
- **IDE**: **Run Build Task** builds the active file; **F5** or **Run** uses `launch.json` to build and run.
- **Multi-compiler**: GCC, Clang, or (Windows) MSVC; keep the chosen compiler on PATH.
