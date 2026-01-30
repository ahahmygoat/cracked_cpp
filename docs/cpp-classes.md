# C++ classes: definition, instance, and object

This document explains **what a class is**, **how it differs from an object**, and **how to define a class and create an instance** in C++. It follows the approach from the video: define a class for one order book entry, then create an object and use it.

**Design choices we make here:** We use **public** data first so you can see and set fields directly (simplest). Later you'd often use **private** data plus constructors and getters/setters so the class can enforce invariants. The code comments in the repo call out when we're choosing simplicity over "best practice" so you know what to tighten later.

**Principal takeaway:** A class is a *type* you define: data + behavior. One row in the domain (e.g. order book row) → one object. Public data = fast to build, no invariants; private data = one place to enforce rules. When product says "price can't be negative," that's when you switch to private + setter. See [DESIGN.md](DESIGN.md) for data vs behavior and when to encapsulate.

---


## 1. Class vs instance (object)

### The idea

- **Class** — a **specification** or **definition** of a type. It says _what_ data and behavior something has, but it doesn’t hold actual data by itself.
- **Instance / object** — an **actual** variable of that type. It holds the real data.

So: the **class** is the blueprint; the **object** is the thing you create from it.

### Analogy: `double` and a variable

- **`double`** — the _type_. It tells you “this is a floating‑point number” and how it’s represented. It doesn’t store a specific number.
- **`double myDouble;`** — a _variable_ of type `double`. This is where the actual value lives (e.g. `3.14`).

So:

- **`double`** = definition of something (the type).
- **`myDouble`** = instantiation of that type (the variable holding data).

Same for classes:

- **Class** = definition of something (the type you invent).
- **Object** = instantiation of that class (a variable of that type, holding real data).

---

## 2. Why define our own class?

We need to represent **one row** of the order book dataset: timestamp, trading pair, bid/ask, amount, price. Instead of five separate variables every time, we **define a type** that bundles them: one **order book entry** type. Then we create **objects** of that type (e.g. `order1`, `order2`), each holding one row’s data.

So: **one row in the order book = one object** of our class.

---

## 3. How to define a class

### Syntax

```cpp
class ClassName {
    // data and functions go here
};   // ← semicolon at the end is required
```

- **`class`** — keyword to start a class definition.
- **`ClassName`** — the name of your type (e.g. `OrderBookEntry`).
- **`{ }`** — body of the class (data members and/or member functions).
- **`;`** — you **must** put a semicolon after the closing `}`. Forgetting it is a common mistake.

### Data members (fields)

Inside the class you declare **data members** the same way you declare variables: **type** and **name**. They describe what each object of this class will contain.

Example: one row of the order book has price, amount, timestamp, product, and order type (bid/ask). So we put those inside the class:

```cpp
enum class OrderBookType { bid, ask };

class OrderBookEntry {
public:
    double price;
    double amount;
    std::string timestamp;
    std::string product;
    OrderBookType orderType;
};
```

- **`public:`** — for now we make the data **public** so we can read and set it from outside the class (e.g. in `main`). This is the simplest way to get started. Later you’ll often use **private** data and **member functions** to access it.
- The five **data members** correspond to the five columns in the order book CSV: price, amount, timestamp, product, order type.

So we’ve **specified the data** for our type: we gave the class a name and said what data each instance will have.

---

## 4. Creating an object (instantiating the class)

Declare a variable whose type is your class name. That variable is an **object** (an instance of the class):

```cpp
OrderBookEntry order1;
```

- **`OrderBookEntry`** — the type (the class we defined).
- **`order1`** — the name of the variable (the object). Can be any valid variable name.

That one line **creates an instance** of the class. `order1` is an object; it now has space for price, amount, timestamp, product, and orderType.

---

## 5. Putting data into the object

If the data members are **public**, you set them with the **dot** (`.`): `object.member`:

```cpp
order1.price     = 10000;
order1.amount    = 0.002;
order1.timestamp = "2020/03/17 17:01:24.884492";
order1.product   = "ETH/BTC";
order1.orderType = OrderBookType::bid;
```

- Each field is set separately. Later you’ll see better ways (constructors, initializer lists), but this is the direct way when everything is public.
- **`OrderBookType::bid`** — use the enum value for “bid” (or `OrderBookType::ask` for “ask”).

---

## 6. Using the object (reading data)

You also use the dot to **read** the data:

```cpp
std::cout << order1.price << std::endl;
```

So: **declare** the object, **set** its members, then **use** them (e.g. print, pass to functions).

---

## 7. Full sketch (what we did in the video)

1. **Define the type** — `class OrderBookEntry { ... };` with five data members (and an enum for bid/ask).
2. **Create an object** — `OrderBookEntry order1;`
3. **Set the data** — `order1.price = 10000;` etc.
4. **Use the data** — e.g. `std::cout << order1.price << std::endl;`
5. **Compile** — e.g. `g++ -std=c++11 main.cpp -o main` (C++11 so we can use `enum class` and other features).
6. **Run** — e.g. `./main` and see the printed price.

So we **defined a new type** (our class), **instantiated** it (created an object), **filled** it with data (one row of the order book), and **printed** part of it.

---

## 8. Summary table

| Concept         | Meaning                                                                            |
| --------------- | ---------------------------------------------------------------------------------- |
| **Class**       | Definition of a type: what data (and later, functions) it has. Like a blueprint.   |
| **Object**      | Instance of a class: a variable that holds actual data.                            |
| **Data member** | A variable declared inside the class; each object has its own copy.                |
| **`public:`**   | Members below this can be read/set from outside the class (simplest for learning). |
| **Instantiate** | Create an object: `OrderBookEntry order1;`                                         |
| **Dot (`.`)**   | Access a member: `order1.price`, `order1.timestamp`.                               |

---

## 9. One row = one object

The order book CSV has one row per entry (timestamp, pair, bid/ask, amount, price). Our **class** describes that shape. Each **object** of that class holds one row’s data. So when we load the CSV, we’ll create many **objects** (e.g. put them in a `std::vector<OrderBookEntry>`), one per row.

---

## 10. Next steps

- **Private data and member functions** — hide data behind `private:` and provide functions (e.g. `getPrice()`, `setPrice()`) for a better design.
- **Constructors** — initialize an object when it’s created instead of setting each field by hand. For many ways to define constructors (default, parameterized, copy, move, delegating, `= default`, `= delete`), see [constructors-and-initialization.md](constructors-and-initialization.md).
- **Header files** — put the class definition in a `.h` file and `#include` it where needed.

---

## Related docs

- [ClassesandData.md](ClassesandData.md) — Types and data (struct, class, sizeof).
- [ORDERBOOK.md](ORDERBOOK.md) — Order book and CSV format.
- [constructors-and-initialization.md](constructors-and-initialization.md) — Many ways to define constructors (default, parameterized, copy, move, = default, = delete).
- [DESIGN.md](DESIGN.md) — OOP and design (principal/PM level).
- [INDEX.md](INDEX.md) — Doc map and learning path.
