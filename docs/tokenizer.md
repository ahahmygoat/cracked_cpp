# Tokenizer: splitting strings into parts

This doc explains **what a tokenizer is** and **several ways to split a string** in C++ (by delimiter or pattern). We tie the main pattern to the CSV parsing in **CSVReader.cpp**, where each line is tokenized by comma.

**Takeaway:** A **tokenizer** turns one string into a sequence of **tokens** (substrings) by a **delimiter** (e.g. comma) or by a pattern. The pattern we use in this repo is **`std::getline(stream, token, delimiter)`** in a loop on a **`std::stringstream`** — simple, standard, and good for CSV. See [CSVReader.cpp](../src/CSVReader.cpp) (`tokenize`, `readCSVInto`) for the actual code.

---

## 1. What is a tokenizer?

A **tokenizer** (or **split**) takes a single string and produces a sequence of **tokens** — the pieces between delimiters (e.g. commas, spaces, newlines) or matching some pattern.

**Examples:**

| Input string        | Delimiter | Tokens                          |
|---------------------|-----------|----------------------------------|
| `"a,b,c"`           | `','`     | `"a"`, `"b"`, `"c"`             |
| `"2020/03/17,ETH/BTC,bid,0.02,7.44"` | `','` | timestamp, product, type, amount, price |
| `"one two three"`   | `' '`     | `"one"`, `"two"`, `"three"`     |
| `"key=value"`       | `'='`     | `"key"`, `"value"`              |

**Why it matters:** File formats (CSV, config files, log lines) and text input often come as one line; you need to **tokenize** the line to get individual fields before converting or storing them.

---

## 2. Pattern we use: `std::getline` with a delimiter

The standard, portable way to split a string by a **single-character delimiter** in C++ is to put the string into a **`std::stringstream`** and then call **`std::getline(stream, token, delimiter)`** in a loop. Each call reads one token (everything up to the next delimiter or end of stream).

### 2a. Minimal snippet: split by comma

```cpp
#include <sstream>
#include <string>

// Split line by ','; tokens are overwritten into token one by one.
void parseCommaSeparated(const std::string& line) {
    std::stringstream ss(line);
    std::string token;
    while (std::getline(ss, token, ',')) {
        // use token (e.g. push_back into a vector, or assign to named variables)
    }
}
```

- **`std::stringstream ss(line)`** — build a stream that reads from the string `line`.
- **`std::getline(ss, token, ',')`** — read from `ss` into `token` until a comma or end of stream. Returns the stream (truthy if a token was read). So the loop runs once per token.

### 2b. In this repo: CSV line → named fields

In **CSVReader.cpp**, each CSV line has a fixed column order: **timestamp, product, orderType, amount, price**. We tokenize once per line (via `tokenize(line, ',')`) and then convert to an `OrderBookEntry` via `stringsToOBE(tokens)`. The equivalent “named variables” style (one getline per column) looks like:

```cpp
std::stringstream ss(line);
std::string cell;
std::getline(ss, cell, ',');
std::string timestamp = cell;
std::getline(ss, cell, ',');
std::string product = cell;
std::getline(ss, cell, ',');
std::string orderTypeStr = cell;
std::getline(ss, cell, ',');
std::string amountStr = cell;
std::getline(ss, cell, ',');
std::string priceStr = cell;
```

Then we convert `amountStr` and `priceStr` to `double` (e.g. `std::stod`) and `orderTypeStr` to `OrderBookType`. So: **tokenize first, then interpret** — keeps “split by comma” separate from “what each column means.”

**When to use:** CSV or any line where fields are separated by a single character (comma, tab, pipe). Simple and no extra dependencies.

---

## 3. Collecting tokens into a vector

If you don’t have a fixed number of columns, you can push each token into a **`std::vector<std::string>`**:

```cpp
#include <sstream>
#include <string>
#include <vector>

std::vector<std::string> split(const std::string& line, char delimiter = ',') {
    std::vector<std::string> tokens;
    std::stringstream ss(line);
    std::string token;
    while (std::getline(ss, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

// Usage:
// auto parts = split("a,b,c");           // {"a", "b", "c"}
// auto parts = split("x|y|z", '|');      // {"x", "y", "z"}
```

**When to use:** When the number of columns varies or you want a reusable “split this line” helper. For fixed columns (like our CSV), named variables are often clearer.

---

## 4. Other approaches (for context)

### 4a. Manual loop: `find` + `substr`

You can split by searching for the delimiter with **`std::string::find`** and taking substrings with **`substr`**:

```cpp
std::vector<std::string> tokens;
size_t start = 0;
size_t pos;
while ((pos = line.find(',', start)) != std::string::npos) {
    tokens.push_back(line.substr(start, pos - start));
    start = pos + 1;
}
tokens.push_back(line.substr(start));  // last token
```

**Tradeoff:** More code and easy to get wrong (off-by-one, last token). Prefer **`getline`** for a single-character delimiter.

### 4b. Multiple delimiters or patterns

**`std::getline`** only takes one character delimiter. For multiple delimiters (e.g. comma or semicolon) or regex-style patterns, you’d use a loop with **`find_first_of`** or **`std::regex`** — not needed for our CSV, but worth knowing when you step beyond “split by comma.”

---

## 5. Exception handling when using the tokenizer

**tokenize** — Does **not** throw for normal input; it only splits the string by delimiter. It can throw on allocation failure (e.g. extremely long lines), but we do not wrap it in try/catch for that.

**stringsToOBE** — **Can throw:**
- **`std::invalid_argument`** — if `tokens.size() < 5` (fewer than 5 columns), or if `std::stod(tokens[3])` / `std::stod(tokens[4])` receive non-numeric strings (e.g. header row, empty, "N/A").
- **`std::out_of_range`** — if the numeric string would overflow the representable range for `double`.

**readCSVInto** — Wraps each call to **stringsToOBE** in **try/catch (const std::exception&)**. On throw, it logs to stderr (e.g. "Skipped line (invalid number): ...") and **continues** to the next line, so one bad row does not stop the whole file.

So: **tokenize** is non-throwing for normal CSV lines; **stringsToOBE** is the step that can throw; **readCSVInto** catches and skips. See [exception-handling.md](exception-handling.md) for the full pattern (file open check, stod try/catch, logging).

---

## 6. Quick reference

| Goal                         | Approach                                      | In this repo              |
|-----------------------------|-----------------------------------------------|----------------------------|
| Split line by one character | `std::stringstream` + `std::getline(ss, token, delim)` | **CSVReader.cpp** (`tokenize`) |
| Fixed columns               | One `getline` per column; or `tokenize()` then index | CSVReader: `tokenize(line, ',')` then `tokens[0]`..`tokens[4]` |
| Variable columns            | Loop `getline` and `push_back` into a vector  | Use a `split()` helper as above |
| After tokenizing            | Convert tokens (e.g. `std::stod`, compare)    | CSVReader::stringsToOBE: `stod(tokens[3])`, `stod(tokens[4])`, bid/ask from tokens[2] |

---

## 7. Where this appears in the repo

- **src/CSVReader.cpp** — **`readCSVInto`**: reads the file line by line with **`std::getline(file, line)`**; for each line, calls **`CSVReader::tokenize(line, ',')`** (which uses **`std::getline(ss, token, ',')`** in a loop) to get tokens; then **`stringsToOBE(tokens)`** converts to **`OrderBookEntry`** and we **`push_back`**. See [CSVReader.h](../src/CSVReader.h) and [CSVReader.cpp](../src/CSVReader.cpp).

---

## Related docs

- [ORDERBOOK.md](ORDERBOOK.md) — CSV column layout (what each token means).
- [ClassesandData.md](ClassesandData.md) — Static members and static functions (CSVReader::readCSV, tokenize); reading the file and handling errors.
- [oop-concepts.md](oop-concepts.md) — Static members and utility-style design (when to use static vs instance).
- [exception-handling.md](exception-handling.md) — Try/catch and which tokenizer functions throw.
- [project-layout.md](project-layout.md) — Project layout (src/, scripts/).
- [INDEX.md](INDEX.md) — Doc map and learning path.
