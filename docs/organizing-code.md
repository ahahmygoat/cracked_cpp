# Organizing code: headers, implementation, and limiting exposure

This doc explains **how we organize C++ code into headers and .cpp files** and **limiting exposure** (keeping data in class scope so only the right code can see it). It captures the ideas from the videos: header = specification (promises), .cpp = implementation, namespacing, include guards, and putting data in the **private** section of a class so it’s visible only to member functions.

**Takeaway:** Put **declarations** (promises) in the **.h**; put **definitions** (implementation) in the **.cpp**. Use **include guards** (or `#pragma once`) so a header is only included once. Put data that must be shared between member functions in **class scope** (private) so the rest of the program can’t see it — that’s **limiting exposure**.

---

## 1. Header = specification, .cpp = implementation

**Idea:** The **header (.h)** holds the **specification**: what types and functions exist (the “promises”). The **.cpp** holds the **implementation**: the actual code that makes good on those promises.

- **Header** — Class definition (members, method **declarations**), function **prototypes**, `extern` declarations. No function **bodies** (except inline/template).
- **.cpp** — Function and method **bodies**, definition of globals. Each .cpp that uses the header **#include**s it so the compiler sees the declarations.

So we **declare** in the header and **define** in the .cpp. Other files that only need to *use* the type or function just include the header; they don’t need to see the implementation.

**In this repo:** MerkelMain.h declares the MerkelMain class and MenuOption; MerkelMain.cpp defines the constructor, init(), run(), and all method bodies. OrderBookEntry.h declares OrderBookEntry and loadOrderBook; OrderBookEntry.cpp defines them. See [headers-and-cpp.md](headers-and-cpp.md).

---

## 2. Namespacing: ClassName::functionName

When you define a **member function** outside the class, you must say which class it belongs to. You use the **namespace** syntax:

```cpp
// In Vec3D.h: promise that getLength exists
class Vec3D {
public:
    double getLength() const;
};

// In Vec3D.cpp: implementation belongs to Vec3D
double Vec3D::getLength() const {
    return std::sqrt(x*x + y*y + z*z);
}
```

**Vec3D::** means “this function is in the Vec3D class.” The compiler then ties the definition to the declaration. Without the `Vec3D::` prefix, you’d be defining a free function, not a member, and you’d get link or redefinition errors.

**In this repo:** MerkelMain.cpp uses `MerkelMain::init()`, `MerkelMain::run()`, etc. OrderBookEntry.cpp uses `OrderBookEntry::OrderBookEntry(...)` and `OrderBookEntry::print()`.

---

## 3. #include and include guards

**#include** pulls the contents of another file into the current file. So when you write `#include "MerkelMain.h"`, the preprocessor replaces that line with the full text of MerkelMain.h. That way the compiler sees all the declarations (MerkelMain, MenuOption, etc.).

**Problem:** If MerkelMain.h includes OrderBookEntry.h, and MerkelMain.cpp includes both MerkelMain.h and OrderBookEntry.h, the compiler might see OrderBookEntry.h twice. So it would see the class OrderBookEntry defined twice → **redefinition** error.

**Solution: include guards** (or `#pragma once`). Wrap the header so it’s only processed once per translation unit:

```cpp
#ifndef MERKELMAIN_H
#define MERKELMAIN_H
// ... all declarations ...
#endif /* MERKELMAIN_H */
```

First time: MERKELMAIN_H is not defined, so we define it and process the body. Second time (same .cpp): MERKELMAIN_H is already defined, so the body is skipped. No double definition.

**#pragma once** does the same thing in a single line: “only include this file once.” Many compilers support it. We use **#ifndef / #define / #endif** in this repo so it’s standard C++.

**In this repo:** Every header has include guards (ORDERBOOKENTRY_H, MERKELMAIN_H). That’s why MerkelMain.h can safely `#include "OrderBookEntry.h"` and MerkelMain.cpp only `#include "MerkelMain.h"` — OrderBookEntry.h is still only included once.

---

## 4. Limiting exposure: data in class scope (private)

**Idea:** If data lives **inside a function**, only that function can see it (local scope). If you need **several member functions** to use the same data (e.g. loadOrderBook fills it, printMarketStats reads it), put the data in **class scope** — as a **private** member. Then only the class’s member functions can see it; the rest of the program (e.g. main()) cannot. That’s **limiting exposure**.

**Wrong (local scope):** If `orders` is a local variable inside `loadOrderBook`, it disappears when the function returns. `printMarketStats` can’t see it.

**Right (class scope, private):** Put `std::vector<OrderBookEntry> orders_` in the **private** section of MerkelMain. Then:
- **init()** can call `loadOrderBook(path, orders_)` and fill `orders_`.
- **printMarketStats()** can use `orders_` (e.g. computeAveragePrice(orders_)).
- **main()** cannot do `app.orders_` — it’s private. So exposure is limited to the class.

So we “take orders out of local scope and put it in the private section of the class.” The data persists for the life of the object and is visible only to member functions.

**In this repo:** MerkelMain has private `orders_`. init() calls `loadOrderBook(orderBookPath_, orders_)`; printMarketStats() uses `orders_`. main() only calls app.init() and app.run(); it never touches orders_ directly. See [merkel-main.md](merkel-main.md) and [oop-concepts.md](oop-concepts.md).

---

## 5. Embedding init: load once, use in run

**Idea:** The app loads the order book **once** in **init()**, and the menu loop in **run()** uses that data. So “init embeds the load”: init() calls loadOrderBook(path, orders_), and the loaded data lives in private orders_. Later, printMarketStats() (and any other member) can use orders_ without loading again.

**In this repo:** main() does `MerkelMain app; app.init(); app.run();`. init() sets orderBookPath_ and calls loadOrderBook(orderBookPath_, orders_), so orders_ is filled and ordersLoaded_ is set. run() loops; when the user picks “Print exchange stats,” handleUserOption calls printMarketStats(), which uses orders_. No global; all exposure is limited to the class.

---

## 6. Quick reference

| Topic | Summary |
|-------|---------|
| **Header** | Specification (declarations, promises). No implementation. |
| **.cpp** | Implementation (definitions). #include the header so the compiler sees declarations. |
| **ClassName::** | Namespace: this function belongs to this class. |
| **#include** | Pulls the header into the current file. |
| **Include guards** | #ifndef NAME_H / #define / #endif (or #pragma once) so the header is only included once. |
| **Limiting exposure** | Put shared data in private class members; only member functions can see it. |
| **Embedding init** | init() loads data into private members (e.g. orders_); run() uses that data. |

---

## 7. Where this appears in the repo

- **MerkelMain.h** — Private orders_, orderBookPath_, ordersLoaded_. Include guards MERKELMAIN_H. Includes OrderBookEntry.h.
- **MerkelMain.cpp** — init() calls loadOrderBook(orderBookPath_, orders_); printMarketStats() uses orders_. Only includes MerkelMain.h (OrderBookEntry comes via MerkelMain.h).
- **OrderBookEntry.h** — loadOrderBook(path, out) overload so callers can load into their own vector (e.g. orders_). Include guards ORDERBOOKENTRY_H.

---

## Related docs

- [headers-and-cpp.md](headers-and-cpp.md) — .h/.cpp pattern in more detail.
- [oop-concepts.md](oop-concepts.md) — Encapsulation and private state.
- [DESIGN.md](DESIGN.md) — Single responsibility, data vs behavior, when to refactor.
- [merkel-main.md](merkel-main.md) — MerkelMain: init/run, private orders_.
- [INDEX.md](INDEX.md) — Doc map and learning path.
