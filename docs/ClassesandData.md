# Classes and data: C++ types, structures, and objects

A roadmap from **getting started** through **data types**, **classes**, **data structures**, and **error handling**. Ties into the exchange/wallet project (main.cpp, refactorMain.cpp).

**How we document:** We explain _why_ a type or structure exists and when you'd use it, not just the syntax. Design choices (e.g. struct vs class, public vs private) are called out so you can see the tradeoffs. See [README.md](../README.md) for the overall documentation philosophy.

**Principal takeaway:** Types are contracts: they tell the compiler and the reader what shape data has. Struct vs class is mostly default visibility; use struct for "plain data bag," class when you'll add invariants and behavior. Vectors and error handling (cin.fail(), file open, stod) are system boundaries: validate at the edge so the rest of the code can assume sane data. See [DESIGN.md](DESIGN.md) for system thinking and when to refactor.

---


## Roadmap

| Stage                                   | Topics                                                                          |
| --------------------------------------- | ------------------------------------------------------------------------------- |
| **Getting started**                     | C++ basics, text I/O (cout/cin), control flow (if, while, switch)               |
| **Data, variables, classes, functions** | Modeling data, combining variables, classes with member functions, **static members and static functions**, header files |
| **File parsing, CSV, IDE**              | Reading files, CSV reader, installing and using an IDE                          |
| **Data structures**                     | Vectors, objects, interaction between objects                                   |
| **Complete the wallet**                 | Implement wallet (balance, entries) and wire it into the menu                   |
| **Error handling**                      | Handling bad input, file errors, and failures                                   |

---

## 1. Modeling data: variable types and combining variables

### Single variables

When you declare a variable you **must** state what type of data it holds. C++ is **strongly typed**: the compiler checks that you use types consistently.

- **Primitive types**: `int`, `double`, `char`, `bool` (see [C++ types](#c-types-primitive-and-standard) below).
- **Combining multiple values**: use a **struct** or **class** to group related data (e.g. a wallet with balance and currency, or an order with amount and price).

### Combining multiple variables: struct and class

- **`struct`** — a type that groups several member variables. You can add member functions later.
- **`class`** — like struct, but by default members are private; you control access with `public:` / `private:`.

Example: model an "order" (amount + price) or a "wallet" (balance + currency):

```cpp
struct Order {
    int amount;
    int price;
};

// use it
Order o;
o.amount = 10;
o.price = 50;
```

---

## 2. Classes with functions

### Operating on objects with functions

- **Member functions** (methods): functions that belong to a class. They can read and modify the object's data.
- **Operating on objects**: you call methods on an object, e.g. `wallet.deposit(100);` or `order.print();`.

Example idea:

```cpp
class Wallet {
public:
    double getBalance() const { return balance; }
    void deposit(double amount) { balance += amount; }
private:
    double balance = 0.0;
};
```

### Header files

- **`.h` / `.hpp`** — declare classes, function prototypes, and constants. Other `.cpp` files **#include** the header to use those declarations.
- **`.cpp`** — define the functions (and contain `main()` in one file). Keeps the project organized and compiles faster when you change only one file.

Typical layout:

- `Wallet.h` — `class Wallet { ... };` and any other declarations.
- `Wallet.cpp` — `#include "Wallet.h"` and the definitions of `Wallet`'s member functions.
- `main.cpp` — `#include "Wallet.h"` and your menu loop.

### Static members and static member functions

So far we’ve talked about **per-object** data and methods: each `Wallet` has its own `balance`, and you call `wallet.deposit(100)` on a specific object. Sometimes you want **per-class** data or functions: one copy shared by the whole type, or a function you call **on the type** (e.g. `CSVReader::readCSV(path)`) without creating an object.

- **Static data member** — one copy for the **entire class**, not one per object. All objects of that class share it. Use for shared state: counters, config, caches.
- **Static member function** — called as `ClassName::functionName(args)`. It does **not** have a `this` pointer, so it cannot access non-static members unless you pass an object (or its data) as an argument. Use for **utilities** that don’t need instance state: parsing, factories, helpers.

Example — a class used only for its static API (utility-style):

```cpp
class CSVReader {
public:
    /** Read CSV from path; returns a vector of OrderBookEntry. Called as CSVReader::readCSV(path). */
    static std::vector<OrderBookEntry> readCSV(const std::string& filename);

private:
    static std::vector<std::string> tokenize(const std::string& line, char delim);
    static OrderBookEntry stringsToOBE(const std::vector<std::string>& tokens);
};
```

Usage: `auto orders = CSVReader::readCSV("data/order_book_example.csv");` — no `CSVReader` object is created. The static methods do all the work using only their arguments (and other static helpers). See [tokenizer.md](tokenizer.md) and [src/CSVReader.h](../src/CSVReader.h) in this repo.

Example — static data member (shared counter):

```cpp
class IdGenerator {
public:
    static int nextId() { return nextId_; }
private:
    static int nextId_;  // one copy for the whole class
};
// In .cpp: int IdGenerator::nextId_ = 0;
```

- **Defining static data members:** In C++, a static data member is **declared** in the class and **defined** once (typically in a `.cpp` file), e.g. `int IdGenerator::nextId_ = 0;`. In C++17 you can use **inline** static data members and define them in the header.
- **C++ has no “static class”:** Unlike C#, there is no keyword that makes “the whole class static.” To get a “utility class,” use a class whose **members are all static** (and optionally make the constructor **private** so nobody can create instances). `CSVReader` in this repo is close: it has static `readCSV`/`tokenize`/`stringsToOBE` and also a (currently public) constructor if you ever want to hold a filename in an object; you can still call `CSVReader::readCSV(path)` without constructing one.

**When to use static:** Use **static member functions** when the operation doesn’t need per-object state — e.g. “read this file and return a vector,” “parse this string.” Use **static data members** when you need one shared value for the whole type (counters, config, singletons). Use **normal (instance) members** when each object has its own state (e.g. each `Wallet` has its own balance).

---

## 3. File parsing, CSV reader, IDE

- **File I/O**: use `#include <fstream>`, open a file with `std::ifstream`, read line by line or field by field.
- **CSV reader**: read a line, split by comma (or use a small helper), convert fields to numbers/strings as needed. Store rows in a `std::vector` of structs or objects.
- **IDE**: use VS Code/Cursor with the C/C++ extension, or Visual Studio, and follow [SETUP.md](SETUP.md) for compiler (e.g. MinGW) and run/debug.

---

## 4. Data structures: vectors, objects, interaction

- **Vectors** (`#include <vector>`): dynamic array. `std::vector<Order> orders;` — add with `push_back`, access with `[i]` or iterate.
- **Objects**: instances of your classes (e.g. one `Wallet`, many `Order` objects). They interact by calling each other's member functions or by passing data (e.g. wallet applies a completed order).
- **Interaction**: e.g. menu creates an `Order`, validates it, then passes it to a `Wallet` or an exchange object that updates balance or order book.

---

## 5. Complete the wallet

- **Data**: wallet holds balance (and optionally currency, or a list of transactions).
- **Operations**: deposit, withdraw (or apply a bid/offer), print balance. Expose these as member functions.
- **Menu**: option "Print wallet" calls `wallet.print()` or prints `wallet.getBalance()`; "Make bid/offer" can create an order and then update the wallet when confirmed.

---

## 6. Error handling

- **Invalid input**: check `std::cin.fail()` after reads; clear and ignore bad input (as in refactorMain.cpp). Re-prompt until valid.
- **File errors**: check if the file opened (`if (!file.is_open())`) and handle missing or malformed data.
- **Logic errors**: validate amounts (e.g. withdraw ≤ balance) and handle edge cases. Optionally use assertions or return bool/error codes from functions.
- **Exceptions**: when a callee can throw (e.g. `std::stod`), wrap in **try/catch** and handle (e.g. skip bad CSV line). See [exception-handling.md](exception-handling.md) for try/catch, file open vs exceptions, and where we use it in OrderBookEntry.cpp and refactorMain.cpp.

**Snippet — catch conversion errors and skip the line:**

```cpp
try {
    amount = std::stod(amountStr);
    price = std::stod(priceStr);
} catch (const std::exception& e) {
    std::cerr << "Skipped line: " << e.what() << std::endl;
    continue;  // skip this line
}
```

---

## Bits, bytes, and sizeof()

### Bits and bytes

- **Bit** — smallest unit: 0 or 1. Everything in memory is ultimately bits.
- **Byte** — on almost all systems, **1 byte = 8 bits**. Memory is addressed in bytes; each variable occupies one or more bytes.
- **Size of `char`** — In C++, **`char` is exactly 1 byte** (by definition). So `sizeof(char)` is always **1**. That one byte is at least 8 bits (so a `char` can represent at least 0–255 as unsigned, or about -128–127 as signed, depending on the compiler).

### sizeof()

**`sizeof`** is an operator that gives you the **size in bytes** of a type or of an expression’s result. Useful for understanding how much memory a variable uses and for buffer/array math.

- **Syntax**: `sizeof(type)` or `sizeof variable` (parentheses optional for variables).
- **Result**: `std::size_t` (an unsigned integer type). You can print it with `std::cout << sizeof(int);`.
- **Important**: Sizes are **implementation-defined**. They depend on the compiler and platform (e.g. 32-bit vs 64-bit). So we say “often” below.

Examples:

```cpp
sizeof(char);      // 1 (always 1 byte in C++)
sizeof(int);       // often 4
sizeof(double);    // often 8
sizeof(bool);      // often 1

int x;
sizeof(x);         // same as sizeof(int)
sizeof x;          // same; parentheses optional for variables
```

### Typical sizes (common on 32/64-bit desktop)

| Type          | Typical size (bytes) | Notes                        |
| ------------- | -------------------- | ---------------------------- |
| `char`        | 1                    | Always 1 byte by definition. |
| `bool`        | 1                    | Often 1 byte.                |
| `short`       | 2                    | At least 16 bits.            |
| `int`         | 4                    | At least 16 bits; often 32.  |
| `long`        | 4 or 8               | Platform-dependent.          |
| `long long`   | 8                    | At least 64 bits.            |
| `float`       | 4                    |                              |
| `double`      | 8                    |                              |
| `long double` | 8, 12, or 16         | Platform-dependent.          |

For **structs/classes**, `sizeof` includes padding (extra bytes the compiler may insert for alignment). So `sizeof(Order)` might be more than `sizeof(int) + sizeof(int)` if the compiler aligns fields.

---

## C++ types: primitive and standard

C is **strongly typed** but **weakly checked** (e.g. implicit conversions, fewer checks). C++ adds **stronger type checking** and more types. When you declare a variable you **must** state what is stored there.

### Character types

| Type       | Size (bytes) | Use                                                                 |
| ---------- | ------------ | ------------------------------------------------------------------- |
| `char`     | **1**        | Single character (1 byte by definition); can be signed or unsigned. |
| `char16_t` | 2            | UTF-16 code unit (C++11).                                           |
| `char32_t` | 4            | UTF-32 code unit (C++11).                                           |
| `wchar_t`  | 2 or 4       | Wide character (platform-dependent; use sizeof to check).           |

### Integer types (signed)

| Type          | Typical use                         |
| ------------- | ----------------------------------- |
| `signed char` | Small integer (e.g. -128 to 127).   |
| `short`       | Short integer.                      |
| `int`         | Default integer (e.g. -2e9 to 2e9). |
| `long`        | Longer integer.                     |
| `long long`   | At least 64-bit integer (C++11).    |

### Integer types (unsigned)

| Type                 | Typical use              |
| -------------------- | ------------------------ |
| `unsigned char`      | Byte (0 to 255).         |
| `unsigned short`     | Unsigned short.          |
| `unsigned int`       | Unsigned integer.        |
| `unsigned long`      | Unsigned long.           |
| `unsigned long long` | Unsigned 64-bit (C++11). |

### Floating-point types

| Type          | Use                                                          |
| ------------- | ------------------------------------------------------------ |
| `float`       | Single precision (~7 decimal digits).                        |
| `double`      | Double precision (~15 decimal digits). Default for decimals. |
| `long double` | Extended precision.                                          |

### Boolean type

| Type   | Use                                                                                             |
| ------ | ----------------------------------------------------------------------------------------------- |
| `bool` | `true` or `false`. Used in conditions (`if`, `while`). Result of comparisons (`==`, `<`, etc.). |

---

## Quick reference

- **Bits / bytes**: 1 byte = 8 bits (typical); `char` is always 1 byte. Use **`sizeof(type)`** or **`sizeof variable`** to get size in bytes (implementation-defined).
- **Strongly typed**: every variable has a type; compiler checks usage.
- **Modeling data**: use `struct` or `class` to group variables (e.g. Order, Wallet).
- **Classes + functions**: member functions operate on the object; **static** members/functions belong to the class (one copy, no `this`); use header (`.h`) and implementation (`.cpp`).
- **Data structures**: use `std::vector<T>` for dynamic lists of objects.
- **Error handling**: validate input (cin.fail()), check file open, validate amounts and state.

---

## Related docs

- [tokenizer.md](tokenizer.md) — CSV parsing with static helpers (CSVReader::tokenize, stringsToOBE).
- [simple-classes-and-vectors.md](simple-classes-and-vectors.md) — Worked example: simple class (Vec3D), public/private, std::vector<Vec3D>.
- [cpp-basics.md](cpp-basics.md) — Basics: functions, variables, control flow.
- [SETUP.md](SETUP.md) — Build and run.
- [DESIGN.md](DESIGN.md) — OOP and design (principal/PM level).
- [exception-handling.md](exception-handling.md) — Try/catch, file open, stod (error handling).
- [INDEX.md](INDEX.md) — Doc map and learning path.
