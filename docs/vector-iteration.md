# Iterating over a vector: three design patterns

This doc explains **three ways to iterate over a `std::vector`** in C++: **index-based**, **iterator-based**, and **range-based for**. We use the same task — "print the first N `OrderBookEntry` objects" — so you can compare. The pattern used in **OrderBookEntry.cpp** (lines 103–111) is the **range-based for** loop.

**Why three patterns?** Each has a use: index when you need the position; iterator when you need to insert/erase or use algorithms; range-based for when you just need each element. Noob-friendly; we call out tradeoffs.

---

## 1. Index-based loop (array indexing)

**Idea:** Loop over indices `0` to `size() - 1` and access each element with `vec[i]`. Same as iterating over a C-style array.

**Syntax:**

```cpp
for (size_t i = 0; i < entries.size(); ++i) {
    entries[i].print();
}
```

**Print first N entries (same task as OrderBookEntry.cpp):**

```cpp
void printOrderBook(const std::vector<OrderBookEntry>& entries, int maxRows = 5) {
    size_t end = std::min(entries.size(), static_cast<size_t>(maxRows));
    for (size_t i = 0; i < end; ++i) {
        entries[i].print();
    }
}
```

**Notes:**

- **`entries[i]`** — array indexing: access the element at position `i`. Valid only when `0 <= i < entries.size()`.
- **`size_t`** — type returned by `vector::size()`; unsigned. Use it for indices to avoid signed/unsigned mismatch.
- **When to use:** You need the **index** (e.g. "print row 3," "compare with next element at `i+1`"). Easy to read for anyone used to arrays.

**Tradeoff:** If you don't need the index, range-based for is simpler and less error-prone (no off-by-one, no wrong type for `i`).

---

## 2. Iterator-based loop

**Idea:** Loop using **iterators**: `begin()` points to the first element, `end()` points past the last. Increment the iterator to move to the next element; dereference `*it` to get the element.

**Syntax:**

```cpp
for (auto it = entries.begin(); it != entries.end(); ++it) {
    it->print();   // same as (*it).print()
}
```

**Print first N entries:**

```cpp
void printOrderBook(const std::vector<OrderBookEntry>& entries, int maxRows = 5) {
    int n = 0;
    for (auto it = entries.begin(); it != entries.end() && n < maxRows; ++it, ++n) {
        it->print();
    }
}
```

**Notes:**

- **`entries.begin()`** — iterator to the first element.
- **`entries.end()`** — iterator "past the last" element; you don't dereference it.
- **`*it`** — the element the iterator points to. **`it->print()`** — shorthand for `(*it).print()`.
- **`++it`** — move to the next element.
- **When to use:** You need to **insert** or **erase** in the middle (iterators stay valid in certain cases), or you're using STL algorithms that take iterators (`std::sort(vec.begin(), vec.end())`).

**Tradeoff:** More verbose for "just visit every element." Use when you need iterator semantics (algorithms, insert/erase).

---

## 3. Range-based for loop (what we use in OrderBookEntry.cpp)

**Idea:** "For each element in the range, do something." No index, no explicit iterator — the compiler generates the loop for you. Clean and hard to get wrong.

### 3a. By value, by reference, or by const reference?

The variable in the range-based for can be declared in three ways. **What you choose affects both performance and whether changes inside the loop affect the original vector.**

| Form | What happens | Modify element in vector? | When to use |
|------|--------------|----------------------------|-------------|
| `for (OrderBookEntry order : orders)` | **Copy** of each element into `order`. | **No** — you change the copy only. | Rarely: expensive for non-trivial types. |
| `for (OrderBookEntry& order : orders)` | **Reference** to each element (no copy). | **Yes** — you change the original. | When you need to modify elements in place. |
| `for (const OrderBookEntry& order : orders)` | **Const reference** (no copy, read-only). | **No** — compiler forbids modification. | **Preferred for read-only** (e.g. printing, computing). |

**Snippets:**

```cpp
// By value — copy per iteration; changes to order do NOT affect orders
for (OrderBookEntry order : orders) {
    std::cout << "The price is " << order.price << std::endl;
    order.price = 0.0;  // only changes the copy, not the vector
}

// By reference — no copy; changes to order DO affect orders
for (OrderBookEntry& order : orders) {
    order.price *= 1.01;  // updates the actual element in the vector
}

// By const reference — no copy, read-only (best for “just read each element”)
for (const OrderBookEntry& order : orders) {
    std::cout << "The price is " << order.price << std::endl;
    // order.price = 0.0;  // ERROR: cannot modify const reference
}
```

**Best practice for read-only:** Use **`const auto&`** (or `const OrderBookEntry&`) so the compiler enforces no modification and you avoid copies:

```cpp
for (const auto& entry : entries) {
    entry.print();
}
```

**Syntax (summary):**

```cpp
for (const auto& entry : entries) {
    entry.print();
}
```

**What we actually do in OrderBookEntry.cpp (lines 103–111):** Print only the first `maxRows` entries by counting with a variable `n` and breaking early:

```cpp
void printOrderBook(const std::vector<OrderBookEntry>& entries, int maxRows = 5) {
    int n = 0;
    for (const auto& entry : entries) {
        if (n >= maxRows) break;
        entry.print();
        ++n;
    }
}
```

**Notes:**

- **`const auto& entry`** — each loop, `entry` is a const reference to the current element. No copy; we don't modify the vector.
- **`entry`** — the element itself; use `.` to call methods (`entry.print()`).
- **When to use:** You just need **each element** and don't care about the index. Preferred when you're not inserting/erasing and don't need the position.

**Tradeoff:** You don't get the index for free. If you need "first N," either count (as above) or use an index-based loop up to `min(size(), N)`.

---

## Quick comparison

| Pattern           | Loop form                          | Access element   | Need index? | Need insert/erase? |
|-------------------|-------------------------------------|------------------|-------------|----------------------|
| **Index-based**   | `for (i = 0; i < vec.size(); ++i)` | `vec[i]`         | Yes         | No (or manual)       |
| **Iterator-based**| `for (it = begin(); it != end(); ++it)` | `*it` or `it->` | No (can compute) | Yes |
| **Range-based for** | `for (const auto& x : vec)`       | `x`              | No          | No                   |

**Range-based for variants:** Use `for (Type x : vec)` (copy), `for (Type& x : vec)` (reference, can modify), or `for (const Type& x : vec)` (const reference, read-only; preferred when not modifying).

---

## Array indexing: bounds and size

- **Valid indices:** `0` to `size() - 1`. Accessing `vec[size()]` or any index outside that range is **undefined behavior** (crash or wrong result).
- **`vec.at(i)`** — same as `vec[i]` but **checks bounds**; throws `std::out_of_range` if `i` is invalid. Use when you're not sure; use `vec[i]` when you've already ensured `i < size()`.
- **`vec.size()`** — number of elements. Type is `size_t` (unsigned). Prefer `size_t i` in index loops to avoid signed/unsigned warnings.

---

## Where this appears in the repo

- **src/OrderBookEntry.cpp**: **range-based for** over `entries`, with a counter to stop after `maxRows`. See the file for the full `printOrderBook` function. CSV loading (which fills the vector) is in **src/CSVReader.cpp**; see [project-layout.md](project-layout.md).

---

## Related docs

- [ClassesandData.md](ClassesandData.md) — Vectors and data structures.
- [DESIGN.md](DESIGN.md) — When to use which loop (design).
- [project-layout.md](project-layout.md) — Where OrderBookEntry.cpp and CSVReader.cpp live (src/).
