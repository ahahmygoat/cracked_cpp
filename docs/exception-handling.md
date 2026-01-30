# Exception handling: try, catch, and when to use it

This doc explains **what exceptions are** in C++, **when to use them** vs checks and return codes, and **how we use them** in this repo (file open, `std::stod`, CSV parsing).

**Takeaway:** Exceptions separate "something went wrong" from normal control flow. Use **checks** (e.g. `is_open()`, `cin.fail()`) when failure is expected and you can recover locally; use **try/catch** when a callee throws (e.g. `std::stod`) or when you want to propagate failure up. In **CSVReader.cpp** we check file open and return 0; we catch `std::exception` from `stod` and skip bad CSV lines. See [CSVReader.cpp](../src/CSVReader.cpp) (readCSVInto) and [refactorMain.cpp](../src/refactorMain.cpp) (cin.fail()) for the patterns.

---

## 1. What is an exception?

An **exception** is C++’s way of signaling that something went wrong and normal execution can’t continue. Code **throws** an exception; a caller can **catch** it and handle it (or let it propagate and eventually terminate the program).

- **`throw value;`** — throw an value (often an object derived from `std::exception`). Control jumps to the nearest matching **catch**.
- **`try { ... } catch (Type& e) { ... }`** — run the block; if something in the block throws and the type matches, run the catch block.
- If no catch matches, the exception propagates up until something catches it or `main` is left — then the program typically terminates.

**Why use exceptions?** So that deep callees (e.g. a parser) can signal failure without every function in between checking return codes. The caller that knows *how* to handle the failure (e.g. “skip this line”) does the catching.

---

## 2. When to use exceptions vs checks

| Situation | Typical approach | In this repo |
|-----------|-------------------|--------------|
| **Expected failure, easy to handle** | Check and return / retry (e.g. file open, invalid input) | `if (!file.is_open()) return 0;` — [CSVReader.cpp](../src/CSVReader.cpp). `cin.fail()` then clear/ignore — [refactorMain.cpp](../src/refactorMain.cpp). |
| **Callee can throw** (e.g. `std::stod`) | Wrap in try/catch and handle or rethrow | `try { ... std::stod(...); } catch (const std::exception&) { continue; }` — [CSVReader.cpp](../src/CSVReader.cpp). |
| **Must not fail** (e.g. logic bug) | Assert or let it crash in debug | We don’t throw from our own code for “impossible” cases; we validate at boundaries. |

**Principle:** Validate at the **boundary** (file open, user input, parsing). Use **checks** when failure is expected and you can recover (return 0, skip line, re-prompt). Use **try/catch** when you call something that throws (like `stod`) and you want to recover (e.g. skip bad line) instead of crashing.

---

## 3. File open: check, don’t throw

We **don’t** throw when a file fails to open. We check **`is_open()`** and return a sentinel (e.g. 0) so the caller can decide what to do.

```cpp
std::ifstream file(path);
if (!file.is_open()) {
    std::cerr << "Could not open file: " << path << std::endl;
    return 0;  // caller can check count and e.g. show a message
}
```

**Where:** **CSVReader.cpp** — `readCSVInto`: if the file doesn’t open, we log to stderr and return 0 (number of rows loaded). Callers (e.g. MerkelMain::init) use `CSVReader::readCSV(path, out)` and can check the return value.

---

## 4. Parsing numbers: `std::stod` can throw

**`std::stod(str)`** converts a string to `double`. It **throws** if the string doesn’t represent a valid number or is out of range:

- **`std::invalid_argument`** — string is not a valid numeric format (e.g. header row, empty, "N/A").
- **`std::out_of_range`** — value would overflow the representable range.

So when we parse CSV fields (amount, price), we wrap the conversion in **try/catch** and **skip** that line (or log and skip) instead of crashing the whole load.

### 4a. Catch and skip (what we do in CSVReader.cpp)

```cpp
double amount = 0.0, price = 0.0;
try {
    amount = std::stod(amountStr);
    price = std::stod(priceStr);
} catch (const std::exception& e) {
    // Skip this line (e.g. header or malformed); optionally log
    std::cerr << "Skipped line (invalid number): " << e.what() << std::endl;
    continue;
}
// use amount, price to build OrderBookEntry
```

- **`catch (const std::exception& e)`** — catches `std::invalid_argument`, `std::out_of_range`, and any other standard exception. Catching by **const reference** avoids slicing and is standard practice.
- **`e.what()`** — a short message describing the error (useful for logging).
- **`continue`** — skip this line and go to the next; the rest of the load continues.

**Where:** **CSVReader.cpp** — `readCSVInto`: we use this pattern so one bad row doesn’t stop the whole file. See [exception-handling.md](exception-handling.md) and the comment in the source.

### 4b. Catch specific types (optional)

If you only want to handle conversion errors and let other exceptions propagate:

```cpp
try {
    amount = std::stod(amountStr);
    price = std::stod(priceStr);
} catch (const std::invalid_argument&) {
    continue;  // not a number
} catch (const std::out_of_range&) {
    continue;  // number too large
}
```

For our CSV loader, catching **`std::exception`** is enough and keeps the code short.

---

## 5. User input: check `cin.fail()`, don’t rely on exceptions

For keyboard input we **don’t** use exceptions. We read, then check **`std::cin.fail()`**; if input was invalid we clear the stream and ignore the rest of the line, then re-prompt.

```cpp
std::cin >> userOption;
if (std::cin.fail()) {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    userOption = 0;  // so caller sees "invalid" and can re-prompt
}
```

**Where:** **refactorMain.cpp** — `getUserOption`, `readAmountAndPrice`. See [ClassesandData.md](ClassesandData.md) (Error handling) and [cpp-basics.md](cpp-basics.md).

---

## 6. Throwing your own exceptions (when needed)

If you want to signal an error from your own code, throw an exception type (prefer something derived from **`std::exception`**):

```cpp
#include <stdexcept>

if (price < 0) {
    throw std::invalid_argument("price must be non-negative");
}
```

Callers can then **try/catch** and handle or rethrow. We don’t throw from OrderBookEntry in this repo; we validate at the boundary (file open, stod) and return or skip.

---

## 7. Quick reference

| What | How | In this repo |
|------|-----|----------------|
| File didn’t open | `if (!file.is_open()) { cerr; return 0; }` | CSVReader.cpp readCSVInto |
| String → number (can throw) | `try { x = std::stod(s); } catch (const std::exception& e) { ... }` | CSVReader.cpp: skip bad CSV line, optionally log e.what() |
| Keyboard input invalid | `if (std::cin.fail()) { clear(); ignore(...); }` | refactorMain.cpp getUserOption, readAmountAndPrice |
| Throw from our code | `throw std::invalid_argument("message");` | Not used in OrderBookEntry; validate and return/skip instead |
| Catch by reference | `catch (const std::exception& e)` | Avoids slicing; use e.what() for logging |

---

## 8. Where this appears in the repo

- **src/CSVReader.cpp** — **readCSVInto**: file open checked with `is_open()`; return 0 on failure. `stringsToOBE` uses `std::stod`; **catch (const std::exception&)** in the read loop to skip malformed lines and optionally log. See [CSVReader.h](../src/CSVReader.h) and [CSVReader.cpp](../src/CSVReader.cpp).
- **src/refactorMain.cpp** — **getUserOption**, **readAmountAndPrice**: use **cin.fail()**, **cin.clear()**, **cin.ignore()** to handle invalid numeric input without exceptions.

---

## Related docs

- [DESIGN.md](DESIGN.md) — Error-handling strategy (validate at boundaries, skip bad CSV lines).
- [ClassesandData.md](ClassesandData.md) — Types and file/CSV context.
- [tokenizer.md](tokenizer.md) — Which tokenizer functions throw (tokenize vs stringsToOBE) and who catches (§5).
- [project-layout.md](project-layout.md) — Where CSVReader.cpp and refactorMain.cpp live (src/).
- [INDEX.md](INDEX.md) — Doc map and learning path.
