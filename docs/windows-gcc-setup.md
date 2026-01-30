# Fix: Get g++ Working on Windows (Download, Setup, PATH)

This guide gets you from **"g++ is not recognized"** (or **"ERROR: g++ not found"** from the script) to running:

```powershell
g++ -std=c++17 -o OrderBookEntry OrderBookEntry.cpp; .\OrderBookEntry.exe
```

**Why it fails:** Windows doesn't know where `g++` is. You must:

1. **Install** MSYS2 and the GCC package inside it (`pacman -S mingw-w64-ucrt-x86_64-gcc`).
2. **Add** the MSYS2 `bin` folder to your **PATH** so any terminal (and the build scripts) can find `g++`.

If you skip Step 1, the build script will still say "g++ not found" even after adding PATH, because `g++.exe` won't exist yet.

---

## Step 1: Download and install MSYS2 (includes MinGW/GCC)

1. **Download MSYS2**
   - Go to: **https://www.msys2.org/**
   - Click **"Download"** and get the installer (e.g. `msys2-x86_64-...exe`).

2. **Run the installer**
   - Install to the default location: **`C:\msys64`** (we'll use this path for PATH).
   - When it finishes, leave the option to **"Run MSYS2 now"** checked if you want, or close it.

3. **Install the MinGW GCC compiler**
   - Open **MSYS2** from the Start Menu (search "MSYS2" → open **"MSYS2 UCRT64"** or **"MSYS2 MINGW64"**).
   - In the MSYS2 terminal, run:
     ```bash
     pacman -Syu
     ```
     (If it says "close the window and run again", do that, then run `pacman -Syu` again.)
   - Then install the compiler:
     ```bash
     pacman -S mingw-w64-ucrt-x86_64-gcc
     ```
   - When asked, press **Enter** to accept the default (all packages).
   - Wait until it finishes. You can close MSYS2 after that.

**Result:** GCC (g++) is now installed under `C:\msys64`. The folder we need on PATH is:

- **`C:\msys64\ucrt64\bin`** — if you used **UCRT64** shell (most common with `pacman -S mingw-w64-ucrt-x86_64-gcc`), or  
- **`C:\msys64\mingw64\bin`** — if you used **MINGW64** shell.

**Verify before Step 2:** Open File Explorer and check that **`g++.exe`** exists in one of these folders:

- `C:\msys64\ucrt64\bin\g++.exe`  
- `C:\msys64\mingw64\bin\g++.exe`

If neither exists, go back to Step 1 and run `pacman -S mingw-w64-ucrt-x86_64-gcc` in MSYS2 again. The build script (`build-OrderBookEntry.ps1`) and `run.ps1` try both paths automatically; they only work once `g++.exe` is in one of those folders.

---

## Step 2: Add the compiler to your PATH (system variable)

PATH is a list of folders Windows searches when you type a command. Adding the MinGW `bin` folder lets **any** terminal find `g++`.

### Option A — Using the GUI (recommended if you like clicking)

1. Press **Win**, type **environment**, and open **"Edit environment variables for your account"** (or "Edit the system environment variables" if you prefer system-wide).
2. Under **"User variables"** (or "System variables" if you chose system-wide), select **Path** → click **Edit**.
3. Click **New**.
4. Enter exactly (use the path that has g++.exe on your PC):
   - **`C:\msys64\mingw64\bin`**  
   - or **`C:\msys64\ucrt64\bin`**
5. Click **OK** on the "Edit environment variable" window.
6. Click **OK** on "Environment Variables".
7. Click **OK** on "System Properties".
8. **Close all open terminals and Cursor/VS Code** (or at least close every PowerShell/Command Prompt window). Open a **new** terminal. PATH is read when the terminal starts, so old windows still have the old PATH.

### Option B — Using PowerShell (one-time script)

1. Open **PowerShell** (no need for Admin).
2. Run this. It adds **both** `ucrt64` and `mingw64` so it works whichever one has `g++.exe`:

```powershell
$pathsToAdd = "C:\msys64\ucrt64\bin;C:\msys64\mingw64\bin"
$current = [Environment]::GetEnvironmentVariable("Path", "User")
if ($current -notlike "*ucrt64*" -and $current -notlike "*mingw64*") {
  [Environment]::SetEnvironmentVariable("Path", "$pathsToAdd;$current", "User")
  Write-Host "Added to user PATH: $pathsToAdd"
  Write-Host "Close and reopen your terminal (and Cursor) for it to take effect."
} else {
  Write-Host "MinGW path already in user PATH."
}
```

3. **Close and reopen** your terminal (and Cursor if you use it). New windows will see the new PATH.

---

## Step 3: Check that g++ works

1. Open a **new** PowerShell (or Command Prompt).
2. Run:

```powershell
g++ --version
```

You should see something like:

```
g++ (Rev...) ... x86_64-w64-mingw32-g++ ...
```

If you see **"g++ is not recognized"**:

- You didn’t close and reopen the terminal after editing PATH, or  
- The PATH entry is wrong (check for typos, and that the folder actually contains `g++.exe`).  
- Open **"Edit environment variables"** again and confirm the Path entry is exactly `C:\msys64\mingw64\bin` (or `C:\msys64\ucrt64\bin`). No extra `\` at the end is fine.

---

## Step 4: Build and run your program

1. Go to your project folder in the **same** (new) terminal:

```powershell
cd C:\Users\Joshu\Desktop\cracked_c++
```

2. Build and run in one line:

```powershell
g++ -std=c++17 -o OrderBookEntry OrderBookEntry.cpp; .\OrderBookEntry.exe
```

Or in two lines:

```powershell
g++ -std=c++17 -o OrderBookEntry OrderBookEntry.cpp
.\OrderBookEntry.exe
```

You should see:

```
Order: 0.002 ETH/BTC at 10000 on 2020/03/17 17:01:24.884492 bid
Order: 0.0015 ETH/BTC at 10050 on 2020/03/17 17:01:24.884492 ask
```

**For any other .cpp file:**

```powershell
g++ -std=c++17 -o main main.cpp; .\main.exe
g++ -std=c++17 -o hello hello.cpp; .\hello.exe
```

No `.ps1` scripts needed once PATH is set.

---

## Troubleshooting

| Problem | What to do |
|--------|------------|
| **"g++ is not recognized"** or **script says "ERROR: g++ not found"** | **First:** Install MSYS2 and run `pacman -Syu` then `pacman -S mingw-w64-ucrt-x86_64-gcc` in MSYS2. **Then** add `C:\msys64\ucrt64\bin` (and/or `C:\msys64\mingw64\bin`) to PATH. **Finally:** close all terminals and Cursor and open a new terminal. |
| **Script still fails after adding PATH** | Check that `g++.exe` exists: open File Explorer → `C:\msys64\ucrt64\bin` or `C:\msys64\mingw64\bin`. If it's not there, MSYS2 isn't installed or you didn't run the `pacman -S mingw-w64-ucrt-x86_64-gcc` step. |
| **"g++ is not recognized"** after adding PATH | Close **all** terminals and Cursor, open a new terminal. PATH is loaded at startup. |
| **Wrong path** | In "Edit environment variables" → Path → Edit, make sure the entry is exactly `C:\msys64\mingw64\bin` or `C:\msys64\ucrt64\bin` (no typo, correct drive). |
| **g++.exe not in that folder** | Open File Explorer, go to `C:\msys64\mingw64\bin` or `C:\msys64\ucrt64\bin`. If `g++.exe` isn’t there, run `pacman -S mingw-w64-ucrt-x86_64-gcc` again in MSYS2 (UCRT64 or MINGW64 shell). |
| **DLL not found when running .exe** | The folder on PATH must be the one that contains both `g++.exe` and the DLLs (e.g. `libstdc++-6.dll`). That’s the `bin` folder we added. Running from the same terminal that built the exe should work. |
| **Installed to a different drive (e.g. D:)** | Use your real path in PATH, e.g. `D:\msys64\ucrt64\bin`. Same steps, different letter. The build script only checks `C:\msys64`; if you use D:, add that path to PATH manually. |

---

## Summary

1. **Download & install** MSYS2 from https://www.msys2.org/ (default: `C:\msys64`).
2. **Open MSYS2** (UCRT64 or MINGW64), run: `pacman -Syu` then `pacman -S mingw-w64-ucrt-x86_64-gcc`.
3. **Verify** that `C:\msys64\ucrt64\bin\g++.exe` (or `mingw64\bin\g++.exe`) exists.
4. **Add to PATH** (once): `C:\msys64\ucrt64\bin` and/or `C:\msys64\mingw64\bin` (GUI or PowerShell Option B above adds both).
5. **Close and reopen** all terminals (and Cursor).
6. In a new terminal: `g++ --version` → then `g++ -std=c++17 -o OrderBookEntry OrderBookEntry.cpp; .\OrderBookEntry.exe`

After that, the standard `g++` CLI works in every terminal. The scripts (`run.ps1`, `scripts/build-OrderBookEntry.ps1`, `scripts/build-MerkelMain.ps1`) will also find `g++` automatically if it's in `C:\msys64\ucrt64\bin` or `C:\msys64\mingw64\bin`.

---

## Related docs

- **[SETUP.md](SETUP.md)** — Full setup: run, build, compilers, PATH, troubleshooting.
- **[project-layout.md](project-layout.md)** — Project layout (src/, scripts/); how to build each target.
- **[INDEX.md](INDEX.md)** — Doc map and learning path.
