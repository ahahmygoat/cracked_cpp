# OOP concepts: encapsulation, inheritance, polymorphism

This doc explains **three core software-engineering ideas** in object-oriented programming: **encapsulation**, **inheritance**, and **polymorphism**. We tie each to this repo (OrderBookEntry, MerkelMain) and to when you’d use them.

**Takeaway:** **Encapsulation** = hide data, expose behavior. **Inheritance** = “is-a” and code reuse via a base class. **Polymorphism** = same interface, different behavior (e.g. base pointer, derived behavior). Use them when they reduce duplication, clarify intent, or enforce invariants.

---

## 1. Encapsulation

**Idea:** Hide **data** (implementation details) behind a **public interface** (methods). Callers use the interface; they don’t touch internal state directly. That lets you enforce rules (invariants) and change implementation without breaking callers.

### What it looks like

- **Private** data members — only the class (and friends) can read/write them.
- **Public** methods — getters (read), setters (write with validation), and actions (e.g. `deposit`, `withdraw`).

Example: a **Wallet** with a balance that must never be negative:

```cpp
class Wallet {
public:
    double getBalance() const { return balance_; }
    void deposit(double amount) {
        if (amount > 0) balance_ += amount;
    }
    void withdraw(double amount) {
        if (amount > 0 && amount <= balance_) balance_ -= amount;
    }
private:
    double balance_{0.0};  // hidden; only deposit/withdraw can change it
};
```

Here, encapsulation **enforces the invariant**: balance stays ≥ 0 because there’s no way to set it directly.

### In this repo

- **OrderBookEntry** — we use **public** data (price, amount, timestamp, product, orderType) for simplicity and to match the worksheet. So we’re **not** encapsulating the fields. If we later need rules (e.g. price ≥ 0, or validated timestamp), we’d switch to **private** data + getters/setters and enforce those rules in one place. See [DESIGN.md](DESIGN.md) (public vs private).
- **MerkelMain** — we use **private** state: `orderBookPath_`, `ordersLoaded_`. Only `init()` and `printMarketStats()` use them. That’s encapsulation: the rest of the program doesn’t touch that state directly.

**When to encapsulate:** When you have an **invariant** (e.g. “balance ≥ 0”, “price ≥ 0”) or when you want to change how data is stored without changing callers. If there’s no invariant and the type is just a data bag, public data is fine (e.g. OrderBookEntry for learning).

---

## 2. Inheritance

**Idea:** A **derived class** “is a” kind of **base class** and reuses its data and behavior. You define the base once; derived classes add or override behavior. Inheritance is for **is-a** and **code reuse**, not just “I want another class.”

### What it looks like

```cpp
class OrderBookEntry {
public:
    double price{0.0};
    double amount{0.0};
    std::string product{"ETH/BTC"};
    void print() const;  // base implementation
};

// "LimitOrder is a kind of OrderBookEntry" — inheritance
class LimitOrder : public OrderBookEntry {
public:
    double limitPrice{0.0};  // extra data
    void print() const override;  // override base behavior (polymorphism)
};
```

- **`: public OrderBookEntry`** — LimitOrder **inherits** from OrderBookEntry (is-a).
- **override** — replaces the base `print()` in derived objects (see polymorphism below).

### When to use inheritance

- You have a clear **is-a** relationship (e.g. LimitOrder is an OrderBookEntry with a limit price).
- Several types share **common data and behavior**; the base holds the common part, derived classes add the rest.

**When not to:** If you only need to **use** another type (e.g. MerkelMain uses OrderBookEntry by calling `loadOrderBook` and `orders`), use **composition** (holding a reference or calling functions), not inheritance. MerkelMain is not “a kind of” OrderBookEntry; it **uses** the order book. So we don’t inherit; we include OrderBookEntry.h and call its API.

**In this repo:** We don’t use inheritance yet. OrderBookEntry and MerkelMain are separate types. If we later add “LimitOrder” or “MarketOrder” as kinds of order, we’d consider a base class (e.g. OrderBookEntry) and derived classes.

---

## 3. Polymorphism

**Idea:** **Same interface, different behavior.** You call a method on a base-class pointer or reference; the actual object may be a derived class, and the **derived** implementation runs. That’s **runtime polymorphism** via **virtual** functions.

### What it looks like

```cpp
class OrderBookEntry {
public:
    virtual void print() const { /* base implementation */ }
};

class LimitOrder : public OrderBookEntry {
public:
    void print() const override { /* different implementation */ }
};

// Polymorphism: same call, different behavior depending on actual type
void show(const OrderBookEntry& entry) {
    entry.print();  // if entry is LimitOrder, LimitOrder::print() runs
}
```

- **virtual** — “this function can be overridden in a derived class.”
- **override** — “I’m overriding the base’s virtual function.”
- When you pass a **LimitOrder** to `show()`, `entry.print()` calls **LimitOrder::print()**, not OrderBookEntry::print(). The **type of the object** (at runtime) decides which implementation runs.

### When to use polymorphism

- You have a **family of types** (base + derived) and you want to **treat them uniformly** (e.g. “print any order”) while each type can have its own implementation.
- You store pointers/references to the base type (e.g. `std::vector<OrderBookEntry*>`) but some elements are actually derived types; virtual functions ensure the right method runs.

**In this repo:** OrderBookEntry::print() is not virtual because we don’t have derived order types yet. When we add inheritance (e.g. LimitOrder), we’d make print() virtual and use override in derived classes so that “print this order” does the right thing for each kind.

---

## 4. Static members and utility-style design

**Idea:** **Static** members belong to the **class**, not to any single object. There is **one copy** of a static data member for the whole type; a **static member function** is called as `ClassName::functionName(args)` and has **no `this`** — it cannot access non-static members unless you pass an object (or its data) as an argument. That makes static members ideal for **utility-style** APIs: operations that don't need per-object state (e.g. "read this file and return a vector").

### What it looks like

- **Static data member** — one shared value for the entire class (e.g. a counter, config, cache). Declared in the class; defined once (typically in a `.cpp` file, or inline in C++17).
- **Static member function** — called as `CSVReader::readCSV(path)`. No object required. Can only use static data or arguments; cannot use instance members without an object reference.

Example (utility-style class):

```cpp
class CSVReader {
public:
    static std::vector<OrderBookEntry> readCSV(const std::string& filename);
private:
    static std::vector<std::string> tokenize(const std::string& line, char delim);
    static OrderBookEntry stringsToOBE(const std::vector<std::string>& tokens);
};
// Usage: auto orders = CSVReader::readCSV("data/order_book_example.csv");
```

C++ has **no "static class"** keyword (unlike C#). To get a "utility class," use a class whose **members are all static**; optionally make the constructor **private** so nobody can create instances. See [ClassesandData.md](ClassesandData.md) for static data members (defining them, inline in C++17) and when to use static vs instance members.

### When to use static

- **Static member functions:** When the operation doesn't need per-object state — e.g. parsing a file, converting strings to objects, factory functions. Call as `ClassName::function(args)`.
- **Static data members:** When you need one shared value for the whole type (counters, config, singletons).
- **Instance members:** When each object has its own state (e.g. each Wallet has its own balance).

**In this repo:** **CSVReader** uses static `readCSV`, `tokenize`, and `stringsToOBE`. You call `CSVReader::readCSV(path)` without creating a CSVReader object. That's utility-style design: the API is on the class, not on an instance. See [tokenizer.md](tokenizer.md) and [src/CSVReader.h](../src/CSVReader.h).

---

## 5. Quick comparison

| Concept | Idea | In this repo |
|--------|------|-------------------------------|
| **Encapsulation** | Hide data; expose behavior. Enforce invariants. | MerkelMain: private `orderBookPath_`, `ordersLoaded_`. OrderBookEntry: public data for simplicity (see DESIGN.md). |
| **Inheritance** | “Is-a”; base class + derived class, reuse code. | Not used yet. Would use for e.g. LimitOrder : OrderBookEntry. |
| **Polymorphism** | Same interface, different behavior; virtual/override. | Not used yet. Would use when we have derived order types and call print() via base reference. |
| **Static members** | Per-class (one copy); static functions called as `Class::fn()`, no `this`. | CSVReader: static readCSV, tokenize, stringsToOBE; call CSVReader::readCSV(path). |

---

## 6. Where to see it in the code

- **Encapsulation:** MerkelMain.h — private `orderBookPath_`, `ordersLoaded_`; only init() and printMarketStats() use them. OrderBookEntry.h — comment that we use public data for simplicity; see oop-concepts.md for encapsulation.
- **Inheritance / polymorphism:** No base/derived yet. When added, base would declare `virtual void print() const;`, derived would declare `void print() const override;`.
- **Static members:** CSVReader.h / CSVReader.cpp — static readCSV, tokenize, stringsToOBE; usage: `CSVReader::readCSV(path)`.

---

## 7. Related docs

- [DESIGN.md](DESIGN.md) — Single responsibility, data vs behavior, public vs private, when to refactor; design tradeoffs (when to add encapsulation, when to introduce inheritance).
- [ClassesandData.md](ClassesandData.md) — struct vs class, types, **static members and static member functions**, data structures.
- [cpp-classes.md](cpp-classes.md) — Class vs object, defining a class, members.
- [tokenizer.md](tokenizer.md) — CSVReader static helpers (tokenize, stringsToOBE).
- [headers-and-cpp.md](headers-and-cpp.md) — .h/.cpp split, declarations vs definitions.
- [organizing-code.md](organizing-code.md) — Limiting exposure, embedding init.
- [INDEX.md](INDEX.md) — Doc map and learning path.
