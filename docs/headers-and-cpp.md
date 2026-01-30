# Header files and the .h / .cpp pattern

This doc explains **why C++ uses header (.h) and implementation (.cpp) files** and **how to split declarations from definitions**. The OrderBookEntry code is split into **OrderBookEntry.h** (declarations) and **OrderBookEntry.cpp** (definitions).

**Takeaway:** Declarations go in the **header** so other files can `#include` them and use the types/functions without recompiling the implementation. Definitions go in **one .cpp file** so the linker has a single definition. Include guards prevent the header from being pasted more than once. For **limiting exposure** (data in class scope, private members) and **embedding init** (load once in init, use in run), see [organizing-code.md](organizing-code.md).

---

## 1. Why split .h and .cpp?

### Declarations vs definitions

- **Declaration** — tells the compiler “this name exists and has this type.” Example: `void print();` or `class OrderBookEntry { void print() const; };`
- **Definition** — provides the actual code or storage. Example: `void OrderBookEntry::print() const { std::cout << ...; }` or `std::vector<OrderBookEntry> orders;`

A declaration can appear in many translation units (every .cpp that includes the header). A **definition** must appear in **exactly one** translation unit (otherwise you get “multiple definition” linker errors).

### What goes where

| In the header (.h) | In the .cpp |
|--------------------|-------------|
| Class definition (members, method **declarations**) | Method **definitions** (e.g. `OrderBookEntry::print()`) |
| Function **prototypes** (e.g. `int loadOrderBook(const std::string&);`) | Function **bodies** (e.g. `int loadOrderBook(...) { ... }`) |
| `extern` declaration of global variables | **Definition** of the global (e.g. `std::vector<OrderBookEntry> orders;`) |
| Inline functions / small helpers (e.g. `Format::price`) | Large or non-inline implementations |
| Include guards, `#include` for types used in declarations | `#include "OrderBookEntry.h"` and extra libs (e.g. `<fstream>`) |

**Why?** Other .cpp files (e.g. a future `main.cpp` that uses the order book) only need to **see the declarations** to compile. They `#include "OrderBookEntry.h"`. The linker later combines the compiled .cpp files and resolves calls to the definitions in OrderBookEntry.cpp.

---

## 2. Include guards

Headers are **pasted** into a .cpp file when you write `#include "OrderBookEntry.h"`. If the same header is included twice (e.g. from two other headers), the compiler would see the same class/function declared twice and complain.

**Include guards** wrap the header so it’s only processed once per translation unit:

```cpp
#ifndef ORDERBOOKENTRY_H
#define ORDERBOOKENTRY_H

// ... all declarations ...

#endif /* ORDERBOOKENTRY_H */
```

- First time: `ORDERBOOKENTRY_H` is not defined, so we define it and process the body.
- Second time (same .cpp): `ORDERBOOKENTRY_H` is already defined, so the body is skipped.

The name is usually the header name in uppercase with `_H` (or `_HPP` for .hpp). Keep it unique per header.

---

## 3. Declarations in the header: what we put in OrderBookEntry.h

### Types and helpers

- **OrderBookType** — `enum class OrderBookType { bid, ask };` (definition is small, stays in header.)
- **orderBookTypeToString(OrderBookType)** — small inline helper; stays in header.
- **Format::price**, **Format::sectionHeader** — inline helpers; stay in header so callers (e.g. `OrderBookEntry::print()`) see them.
- **OrderBookEntry** — class with data members and **declarations** of constructor and `print()`; no **bodies** for constructor/print in the header (bodies in .cpp).

### Global variable: extern

- In the header: `extern std::vector<OrderBookEntry> orders;`
- **extern** means “this variable exists somewhere; here we only declare it.” No storage is allocated in the header.
- In OrderBookEntry.cpp we have the **single definition**: `std::vector<OrderBookEntry> orders;`

### Functions

- **loadOrderBook**, **printOrderBook**, **printOrderBookByIndex/ByIterator/ByRange**, **computeAveragePrice**, **computeLowPrice**, **computeHighPrice**, **computePriceSpread** — all **declared** in the header (prototypes) and **defined** in OrderBookEntry.cpp.

---

## 4. Definitions in the .cpp: what we put in OrderBookEntry.cpp

- **#include "OrderBookEntry.h"** — so we see all declarations and can define the functions and the global.
- **Definition of `orders`** — `std::vector<OrderBookEntry> orders;` (exactly once in the project).
- **OrderBookEntry::OrderBookEntry(...)** — constructor body (member initialiser list + empty body).
- **OrderBookEntry::print()** — method body.
- **loadOrderBook**, **printOrderBook***, **compute*** — function bodies.
- **main()** — entry point for the order book demo.

We only need extra **#include**s for what we use in the **bodies** (e.g. `<fstream>`, `<sstream>`, `<algorithm>`, `<utility>`).

---

## 5. Build and link

To build the order book demo (one .cpp that includes the header):

```bash
g++ -std=c++17 -o OrderBookEntry OrderBookEntry.cpp
```

The compiler compiles **OrderBookEntry.cpp** (which includes **OrderBookEntry.h**). There is only one .cpp that defines the order book logic and main, so no linking step between several .o files is needed. If later you add another .cpp (e.g. **main.cpp** that uses `#include "OrderBookEntry.h"` and calls `loadOrderBook`), you would compile both and link:

```bash
g++ -std=c++17 -c OrderBookEntry.cpp -o OrderBookEntry.o
g++ -std=c++17 -c main.cpp -o main.o
g++ -o myapp OrderBookEntry.o main.o
```

Then **OrderBookEntry.o** contains the single definition of `orders`, `loadOrderBook`, etc., and **main.o** contains the code that calls them (thanks to the declarations from the header).

---

## 6. Quick reference

| Topic | Summary |
|-------|---------|
| **Header** | Declarations, include guards, types and prototypes. Other files `#include` it. |
| **.cpp** | Definitions: global variable, method bodies, function bodies. One definition per symbol. |
| **Include guards** | `#ifndef NAME_H` / `#define NAME_H` / `#endif` so the header is only processed once. |
| **extern** | Declare a global in the header; define it exactly once in one .cpp. |
| **Inline in header** | Small helpers (e.g. Format::price) can live in the header as inline so every caller sees the same definition. |

---

## 7. Where this appears in the repo

- **OrderBookEntry.h** — declarations (OrderBookType, OrderBookEntry, Format, orders, loadOrderBook, print*, compute*). Include guards: `ORDERBOOKENTRY_H`.
- **OrderBookEntry.cpp** — definitions (orders, constructor, print(), loadOrderBook, print*, compute*, main). Includes `"OrderBookEntry.h"`.

---

## Related docs

- [cpp-classes.md](cpp-classes.md) — Class syntax and member functions.
- [orderbook-worksheet.md](orderbook-worksheet.md) — Teaching steps tied to OrderBookEntry.
- [vector-iteration.md](vector-iteration.md) — Iterating over the vector (index, iterator, range-for).
- [organizing-code.md](organizing-code.md) — Limiting exposure, embedding init.
- [INDEX.md](INDEX.md) — Doc map and learning path.
