# Constructors and initialization: many ways to create objects

This doc shows **different ways to define and use constructors** in C++, with snippets you can copy and adapt. We explain *why* each form exists and when to use it.

**Takeaway:** Constructors initialize an object when it is created. Prefer **member initializer lists** over assignment in the body; use **`= default`** when you want the compiler-generated behavior; use **`= delete`** to disable copying or moving when that would be wrong. See [OrderBookEntry.h](../src/OrderBookEntry.h) and [OrderBookEntry.cpp](../src/OrderBookEntry.cpp) for a real parameterized + default constructor in this repo.

---

## 1. Default constructor (no arguments)

A **default constructor** can be called with no arguments. It’s used when you write `OrderBookEntry e;` or when a `std::vector<OrderBookEntry>` is resized.

### 1a. Compiler-generated (implicit)

If you don’t declare any constructors, the compiler generates a default constructor that default-initializes members (built-ins undefined, class types get their default constructor).

```cpp
class Simple {
public:
    int a;
    std::string s;
};
// Simple obj;  → obj.a is uninitialized, obj.s is empty
```

**When to use:** Only when “no constructor” is what you want (e.g. plain data bag, no invariants). Be aware that `int a;` is then uninitialized.

### 1b. Explicit default: `= default`

You declare the default constructor and ask the compiler to generate it. Good when you also have other constructors but still want default construction (e.g. for `vector::resize`).

```cpp
class OrderBookEntry {
public:
    double price{0.0};
    double amount{0.0};
    std::string product{"ETH/BTC"};
    // Parameterized constructor (see below)
    OrderBookEntry(double price, double amount, std::string timestamp,
                   std::string product, OrderBookType orderType);
    // Default: compiler-generated so we can e.g. resize a vector of OrderBookEntry
    OrderBookEntry() = default;
};
```

**When to use:** When you add a non-default constructor but still need default construction. Keeps the type “default constructible” for containers and value semantics.

### 1c. User-defined default (empty body)

You can write the default constructor yourself and initialize members in the body or (better) in a member initializer list.

```cpp
class Widget {
public:
    int x;
    std::string name;
    Widget() : x(0), name("unnamed") {}
};
```

**When to use:** When you need a specific default state that isn’t “zero/empty” for everything, or when you can’t use in-class initializers (e.g. older style).

### 1d. In-class member initializers (default state without writing a constructor)

Members can have default values in the class definition. The compiler-generated default constructor will use these.

```cpp
class OrderBookEntry {
public:
    double price{0.0};
    double amount{0.0};
    std::string timestamp{"2020/03/17 17:01:24.884492"};
    std::string product{"ETH/BTC"};
    OrderBookType orderType{OrderBookType::bid};
    // No constructor written → default constructor uses the above defaults
};
```

**When to use:** When a single default state for each member is enough and you don’t need a custom default constructor body.

---

## 2. Parameterized constructor (arguments)

A **parameterized constructor** takes arguments and initializes the object from them. This is the main way to create a fully initialized object (e.g. one row from a CSV).

### 2a. Member initializer list (preferred)

Initialize members in the **member initializer list** (after `:`). They are constructed once; no default-then-assign. Use **`std::move`** for sink parameters (e.g. `std::string`) to avoid extra copies.

```cpp
// In .h:
OrderBookEntry(double price, double amount, std::string timestamp,
               std::string product, OrderBookType orderType);

// In .cpp:
OrderBookEntry::OrderBookEntry(double price, double amount, std::string timestamp,
                               std::string product, OrderBookType orderType)
    : price(price), amount(amount), timestamp(std::move(timestamp)),
      product(std::move(product)), orderType(orderType) {}
```

**When to use:** Always prefer this for non-trivial types. Order of initialization follows **member declaration order** in the class, not the order in the initializer list.

### 2b. Assignment in body (discouraged for non-primitives)

You can assign in the constructor body instead of the initializer list. For `std::string` and other class types this means default construction then assignment (less efficient).

```cpp
OrderBookEntry::OrderBookEntry(double p, double a, std::string ts,
                               std::string prod, OrderBookType ot) {
    price = p;
    amount = a;
    timestamp = ts;   // timestamp was already default-constructed, then assigned
    product = prod;
    orderType = ot;
}
```

**When to use:** Only for very simple types or when you have no choice. Prefer initializer list for clarity and performance.

### 2c. Default arguments (single constructor for “default” and “full”)

One constructor can serve as both default and parameterized by giving default arguments.

```cpp
class Entry {
public:
    double price{0.0};
    std::string product{"ETH/BTC"};
    Entry(double p = 0.0, std::string prod = "ETH/BTC") : price(p), product(std::move(prod)) {}
};
Entry e1;           // same as Entry(0.0, "ETH/BTC")
Entry e2(100.0);    // same as Entry(100.0, "ETH/BTC")
Entry e3(50.0, "BTC/USDT");
```

**When to use:** When “no args” and “some args” are the same logical constructor with sensible defaults.

---

## 3. Copy constructor

The **copy constructor** creates an object from another object of the same type. It’s used for pass-by-value and when you copy a vector element (e.g. `vec.push_back(existing)` if not using move).

### 3a. Compiler-generated (implicit)

If you don’t declare a copy constructor, the compiler generates one that copies each member. Fine for many classes (e.g. OrderBookEntry).

```cpp
OrderBookEntry a(100.0, 0.5, "2020/01/01", "ETH/BTC", OrderBookType::bid);
OrderBookEntry b = a;   // or OrderBookEntry b(a);  → copy constructor
```

### 3b. Explicit copy constructor

Define it when you need to deep-copy resources (e.g. raw pointers) or maintain invariants.

```cpp
class Buffer {
public:
    Buffer(const Buffer& other) : size_(other.size_) {
        data_ = new char[size_];
        std::memcpy(data_, other.data_, size_);
    }
private:
    char* data_;
    size_t size_;
};
```

### 3c. Disable copying: `= delete`

Use when copying doesn’t make sense (e.g. unique resource, non-copyable type).

```cpp
class NonCopyable {
public:
    NonCopyable() = default;
    NonCopyable(const NonCopyable&) = delete;
    NonCopyable& operator=(const NonCopyable&) = delete;
};
```

---

## 4. Move constructor (C++11)

The **move constructor** takes an rvalue reference (`Type&&`) and “steals” the other object’s resources instead of copying. Used when the source is a temporary or explicitly moved with `std::move`.

### 4a. Compiler-generated (implicit)

If you don’t declare copy/move constructors or destructor, the compiler can generate a defaulted move constructor that moves each member.

### 4b. Explicit move constructor

Define when you manage resources (e.g. dynamic memory) and want efficient transfer.

```cpp
class Buffer {
public:
    Buffer(Buffer&& other) noexcept : data_(other.data_), size_(other.size_) {
        other.data_ = nullptr;
        other.size_ = 0;
    }
private:
    char* data_;
    size_t size_;
};
```

### 4c. Disable moving: `= delete`

Use when move would be unsafe or you want the type to be copy-only.

```cpp
MoveOnly(const MoveOnly&) = delete;
MoveOnly(MoveOnly&&) = default;
```

---

## 5. Delegating constructor

A **delegating constructor** calls another constructor of the same class in its initializer list. Reduces duplication.

```cpp
class Config {
public:
    Config() : Config(0, "default") {}
    Config(int value) : Config(value, "default") {}
    Config(int value, const std::string& name) : value_(value), name_(name) {}
private:
    int value_;
    std::string name_;
};
```

**When to use:** When several constructors share the same “full” initialization and differ only in default arguments or a single value.

---

## 6. Creating objects in a vector

Two common ways to add elements: **`push_back`** (copy or move) and **`emplace_back`** (construct in-place from arguments). Prefer **`emplace_back`** when you’re building the element from constructor arguments to avoid a temporary.

```cpp
// push_back: pass a temporary → move (or copy) into the vector
orders.push_back(OrderBookEntry(price, amount, timestamp, product, orderType));

// emplace_back: forward arguments to constructor; object is built in-place (no temporary)
orders.emplace_back(price, amount, timestamp, product, orderType);
```

**When to use:** Use **`emplace_back(args...)`** when you have the constructor arguments and the type has a matching constructor. Use **`push_back(obj)`** when you already have an object (or want to copy/move an existing one).

---

## Quick reference

| Kind | Syntax / idea | When to use |
|------|----------------|-------------|
| **Default (implicit)** | No ctor declared | Plain data, no invariants (watch uninitialized members). |
| **Default `= default`** | `Type() = default;` | Need default construction alongside other ctors (e.g. vector resize). |
| **Default (user)** | `Type() : member(0) {}` | Custom default state. |
| **In-class init** | `double price{0.0};` | Sensible default for each member. |
| **Parameterized** | `Type(args...) : member(args...) {}` | Main way to create a fully initialized object; prefer initializer list. |
| **Default args** | `Type(int a = 0);` | One ctor for “default” and “with args.” |
| **Copy** | `Type(const Type& other);` | Custom copy; otherwise compiler-generated or `= delete`. |
| **Move** | `Type(Type&& other);` | Resource types; otherwise compiler-generated or `= delete`. |
| **Delegating** | `Type() : Type(0) {}` | Reuse another constructor. |
| **`= delete`** | `Type(const Type&) = delete;` | Disable copy or move. |
| **In vector** | `vec.emplace_back(args...);` | Prefer over `push_back(Type(args...))` when you have args. |

---

## Where this appears in the repo

- **OrderBookEntry.h** — Parameterized constructor declaration; `OrderBookEntry() = default;`; in-class initializers for `price`, `amount`, `timestamp`, `product`, `orderType`.
- **OrderBookEntry.cpp** — Parameterized constructor definition with member initializer list and `std::move` for string parameters; `emplace_back` in `loadOrderBookInto`.

---

## Related docs

- [vector-iteration.md](vector-iteration.md) — Iterating over a vector (by value, reference, const reference).
- [cpp-classes.md](cpp-classes.md) — Class vs object and defining types.
- [DESIGN.md](DESIGN.md) — When to add constructors, invariants.
- [project-layout.md](project-layout.md) — Where OrderBookEntry.h/cpp and CSVReader live (src/).
- [INDEX.md](INDEX.md) — Doc map and learning path.
