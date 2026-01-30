# Order book entry: worksheet teaching steps

This doc mirrors the **coding workshop** in **2313_v3.pdf** (Modelling an order book entry as a class). It gives the **teaching/context** for each step and points to the code in **OrderBookEntry.h** and **OrderBookEntry.cpp**.

**Code layout:** Declarations (class, types, function prototypes, `extern` orders) live in **OrderBookEntry.h**; definitions (constructor body, `print()`, loadOrderBook, print*, compute*, and the global `orders`) live in **OrderBookEntry.cpp**. See [headers-and-cpp.md](headers-and-cpp.md) for the .h/.cpp pattern.

**Worksheet flow:** (1) Write out the class definition → (2) Add all data members from the CSV → (3) Add a constructor (prototype + implementation, member initialiser list) → (4) Create a vector of objects, push_back, access and iterate → (5) Challenge: compute average/low/high/spread.

---

## 1. Write out the class definition

**Idea:** A class is a **new data type**. You define it with the keyword `class`, a name, and a body. Use **`public:`** so members are visible from outside. End the class with a **semicolon**.

**Minimal example (worksheet):**

```cpp
class OrderBookEntry {
public:
    double price;
};
```

**In the repo:** OrderBookEntry.h declares `class OrderBookEntry` with all data members (price, amount, timestamp, product, orderType). OrderBookEntry.cpp defines the constructor and `print()`. See [headers-and-cpp.md](headers-and-cpp.md).

---

## 2. Add all data members from the dataset

**Idea:** One row in the CSV = one object. The class should have one **data member** per column.

**CSV columns (order in file):** timestamp, product, orderType, amount, price.

**Example row:** `2020/03/17 17:01:24.884492,ETH/BTC,bid,0.02186299,0.1`

**Field names from the worksheet:**

| Field     | Example value              |
| --------- | -------------------------- |
| timestamp | 2020/03/17 17:01:24.884492 |
| product   | ETH/BTC                    |
| type      | bid                        |
| price     | 0.02187308                 |
| amount    | 7.44564869                 |

**Use an enum for bid/ask:** The worksheet uses `OrderBookType` to store `'bid'` or `'ask'`:

```cpp
enum class OrderBookType { bid, ask };
OrderBookType orderType = OrderBookType::ask;
```

**In the repo:** OrderBookEntry has `double price`, `double amount`, `std::string timestamp`, `std::string product`, `OrderBookType orderType`. See the class definition in OrderBookEntry.h.

---

## 3. Add a constructor

**Idea:** A **constructor** runs when we create an object. It has the **same name as the class** and no return type. You write a **prototype** inside the class and the **implementation** outside, namespaced with `ClassName::`.

### Prototype (inside the class)

```cpp
class OrderBookEntry {
public:
    /** Create a new OrderBookEntry with the price set to the sent value */
    OrderBookEntry(double _price);
    double price;
};
```

### Implementation (outside the class)

```cpp
OrderBookEntry::OrderBookEntry(double _price) {
    price = _price;
}
```

The **`OrderBookEntry::`** prefix means “this function belongs to OrderBookEntry.” Because it’s namespaced into the class, it can access the class’s data members (including private ones). The name is the same as the class because it’s a constructor.

### Member initialiser list (preferred in modern C++)

Assigning inside the body (`price = _price;`) works, but **member initialiser lists** are preferred:

```cpp
OrderBookEntry::OrderBookEntry(double _price)
    : price(_price)
{
}
```

The **`: price(_price)`** part initialises the member `price` from the parameter. No need to assign inside the body.

**In the repo:** OrderBookEntry has a constructor that takes all five columns (price, amount, timestamp, product, orderType); the **declaration** is in OrderBookEntry.h and the **implementation** (member initialiser list) is in OrderBookEntry.cpp.

**Further reading:** [cplusplus.com – classes](https://www.cplusplus.com/doc/tutorial/classes/) (constructors, member initialization). Degree textbook: Chapter 11 p.388 (constructors), p.394 (member initialiser lists).

---

## 4. Create a vector of objects

**Idea:** Store **many** `OrderBookEntry` objects in a **`std::vector<OrderBookEntry>`**. Use **`push_back`** to add objects; use **indexing** or a **range-based for** loop to access them.

### Declare and push

```cpp
std::vector<OrderBookEntry> entries;
entries.push_back(obe1);
entries.push_back(obe2);
```

### Access by index

```cpp
std::cout << entries[0].price << std::endl;
```

`entries[0]` is the first `OrderBookEntry`; `.price` is its member.

### Range-based for loop

```cpp
for (OrderBookEntry& e : entries) {
    std::cout << e.price << std::endl;
}
```

**Should we use a const reference?** If we **only read** from the objects (e.g. print), we should use **`const`** so we don’t accidentally modify them, and **reference** to avoid copies:

```cpp
for (const OrderBookEntry& e : entries) {
    std::cout << e.price << std::endl;
}
```

**In the repo:** OrderBookEntry.h declares **`extern std::vector<OrderBookEntry> orders`**; OrderBookEntry.cpp **defines** `orders` and fills it in `loadOrderBook` with **`emplace_back`** (constructs in-place; no temporary). Printing uses index-based, iterator-based, and range-based for; the range-based version uses `const auto& entry`. See OrderBookEntry.h/cpp, and [vector-iteration.md](vector-iteration.md).

**Further reading:** [Microsoft – range-based for](https://docs.microsoft.com/en-us/cpp/cpp/range-based-for-statement-cpp?view=msvc-170). Degree textbook: Chapter 6 p.216 (references and range loops).

---

## 5. Challenge: compute functions

**Idea:** Write **free functions** (not member functions) that take a **vector of entries** and return a single value. They go in the .cpp file **before** `main`, not inside the class.

**Function names (from worksheet):**

- `double computeAveragePrice(std::vector<OrderBookEntry>& entries)`
- `double computeLowPrice(std::vector<OrderBookEntry>& entries)`
- `double computeHighPrice(std::vector<OrderBookEntry>& entries)`
- `double computePriceSpread(std::vector<OrderBookEntry>& entries)`

**In the repo:** OrderBookEntry.h declares them; OrderBookEntry.cpp implements all four. They take **`const std::vector<OrderBookEntry>& entries`** (const reference: we don’t modify the vector or the entries). `main` in OrderBookEntry.cpp prints the stats after loading the CSV. See the “Worksheet challenge” section and the end of `main`.

---

## 6. Summary: worksheet → code map

| Worksheet step              | In OrderBookEntry.h / .cpp / docs                     |
| --------------------------- | ----------------------------------------------------- |
| Class definition, data members | OrderBookEntry.h: `class OrderBookEntry` with price, amount, timestamp, product, orderType |
| Constructor (all 5 columns) | .h: prototype; .cpp: implementation with member initialiser list |
| Vector of objects           | .h: `extern std::vector<OrderBookEntry> orders`; .cpp: definition, `loadOrderBook`, `emplace_back` |
| push_back, entries[i]       | .cpp: `emplace_back` in loader; `entries[i].print()` in printOrderBookByIndex |
| Range-based for             | .cpp: `printOrderBookByRange` (and compute functions)  |
| Const reference when reading | `const auto& entry`, `const std::vector<OrderBookEntry>&` |
| Challenge: compute functions | .h: prototypes; .cpp: `computeAveragePrice`, `computeLowPrice`, `computeHighPrice`, `computePriceSpread` |
| .h / .cpp split             | [headers-and-cpp.md](headers-and-cpp.md)              |

---

## 7. Related docs

- [headers-and-cpp.md](headers-and-cpp.md) — Why and how we split declarations (.h) and definitions (.cpp); include guards, extern.
- [ORDERBOOK.md](ORDERBOOK.md) — Domain: what an order book is, dataset columns.
- [vector-iteration.md](vector-iteration.md) — Three ways to iterate over a vector (index, iterator, range-for).
- [simple-classes-and-vectors.md](simple-classes-and-vectors.md) — Syntax for simple classes and putting objects in a vector.
- [project-layout.md](project-layout.md) — Where OrderBookEntry.h and OrderBookEntry.cpp live (src/).
- [INDEX.md](INDEX.md) — Doc map and learning path.

**Full code:** **src/OrderBookEntry.h** (declarations) and **src/OrderBookEntry.cpp** (definitions). CSV loading: **src/CSVReader.cpp**.
