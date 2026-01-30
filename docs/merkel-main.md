# MerkelMain: exchange application

This doc describes **MerkelMain** — the main application class for the Merkel exchange. It covers the **flow** (constructor → init → run), the **menu loop**, and how it fits the **.h/.cpp** pattern.

**Takeaway:** MerkelMain is the entry-point object: you create it, call **init()** once for setup, then **run()** for the menu loop. **init()** embeds the load: it calls **loadOrderBook(orderBookPath_, orders_)** so data lives in **private orders_** (class scope). **Limiting exposure:** only member functions see orders_; main() cannot do app.orders_. See [organizing-code.md](organizing-code.md) and [oop-concepts.md](oop-concepts.md).

---

## 1. What is MerkelMain?

**MerkelMain** is the main application class for the exchange. It:

- **Constructor** — runs when you create a `MerkelMain` (e.g. log or default state).
- **init()** — one-time setup (e.g. load config, load order book). Called once before the loop.
- **run()** — main loop: print menu, get user choice, validate, handle action, exit when user picks "Continue".

So the program flow is: **main() → create MerkelMain → init() → run()** until the user chooses option 6.

---

## 2. Flow: main → init → run

```
main()
  └── MerkelMain app;
  └── app.init();      // once
  └── app.run();       // loop until user picks 6
        └── printMenu()
        └── getUserOption()
        └── validateUserOption(userOption)
        └── handleUserOption(choice)
        └── if choice == Continue → break
  └── return 0;
```

**init()** embeds the load: calls **loadOrderBook(orderBookPath_, orders_)** (overload that fills a vector). Data lives in **private orders_** (limiting exposure). **printMarketStats()** uses **computeAveragePrice**, **computeLowPrice**, **computeHighPrice**, **computePriceSpread** when orders are loaded; otherwise shows a placeholder. **run()** contains the same logic as refactorMain’s main loop: menu, get/validate option, dispatch to the right action.

---

## 3. Menu options: MenuOption enum

Menu choices are an **enum class** so we use a type instead of magic numbers:

```cpp
enum class MenuOption {
    Help     = 1,
    Stats    = 2,
    Offer    = 3,
    Bid      = 4,
    Wallet   = 5,
    Continue = 6
};
```

Values 1–6 match what the user types. We read an **int**, then **static_cast** to **MenuOption** and pass to **handleUserOption(MenuOption choice)**.

---

## 4. MerkelMain methods

| Method | Purpose |
|--------|--------|
| **MerkelMain()** | Constructor; runs when the object is created. |
| **init()** | One-time setup: load order book (OrderBookEntry), set orderBookPath_, ordersLoaded_. |
| **run()** | Main loop: menu → get option → validate → handle → exit on Continue. |
| **printMenu()** | Private: print the 1–6 menu. |
| **orderBookPath_**, **ordersLoaded_**, **orders_** | Private state (limiting exposure). orders_ holds loaded entries; only init() and printMarketStats() (and other members) see it. See organizing-code.md. |
| **getUserOption()** | Read int from user; handle cin.fail(). |
| **validateUserOption(int&)** | Re-prompt until 1–6. |
| **readAmountAndPrice(int&, int&)** | Read amount and price (shared by offer and bid). |
| **handleUserOption(MenuOption)** | Switch on choice; call the right action. |
| **printHelp()** | Action for option 1. |
| **printMarketStats()** | Action for option 2. |
| **makeOffer()** | Action for option 3; uses readAmountAndPrice. |
| **makeBid()** | Action for option 4; uses readAmountAndPrice. |
| **printWallet()** | Action for option 5. |
| **continueToNextTimeStep()** | Action for option 6; run() exits after this. |

---

## 5. .h and .cpp split

- **MerkelMain.h** — Declarations: **MenuOption** enum, **MerkelMain** class (constructor, init, run, and all public/private method **declarations**). Include guards: **MERKELMAIN_H**.
- **MerkelMain.cpp** — Definitions: constructor body, **init()**, **run()**, **printMenu()**, **getUserOption()**, **validateUserOption()**, **readAmountAndPrice()**, **handleUserOption()**, and all action method bodies. Plus **main()** that creates MerkelMain, calls init(), then run().

See [headers-and-cpp.md](headers-and-cpp.md) for the general pattern.

---

## 6. Build and run

MerkelMain uses **OrderBookEntry** (loadOrderBook, orders, compute*), so you must **link OrderBookEntry.cpp**:

```bash
g++ -std=c++17 -o MerkelMain MerkelMain.cpp OrderBookEntry.cpp
./MerkelMain
```

Or on Windows:

```bash
g++ -std=c++17 -o MerkelMain.exe MerkelMain.cpp OrderBookEntry.cpp
.\MerkelMain.exe
```

Run from the project root so **data/order_book_example.csv** is found (init() loads it).

---

## 7. Relation to other code

- **refactorMain.cpp** — Same menu and actions as MerkelMain, but as free functions and a single main loop. MerkelMain wraps that flow in a class (init + run).
- **main.cpp** — Simpler “single main()” menu; no loop, one pass.
- **OrderBookEntry** — Later, **init()** can call **loadOrderBook** and store orders; **printMarketStats()** could use **computeAveragePrice** etc. from OrderBookEntry.

---

## 8. Quick reference

| Topic | Where |
|-------|--------|
| Declarations | MerkelMain.h (MenuOption, MerkelMain, private orderBookPath_/ordersLoaded_) |
| Definitions | MerkelMain.cpp (all method bodies + main); links with OrderBookEntry.cpp |
| Entry point | main() in MerkelMain.cpp → MerkelMain app; app.init(); app.run(); |
| Menu loop | run() → printMenu, getUserOption, validateUserOption, handleUserOption; exit on Continue |
| Encapsulation | Private orderBookPath_, ordersLoaded_; see [oop-concepts.md](oop-concepts.md) |

---

## Related docs

- [organizing-code.md](organizing-code.md) — Header = spec, .cpp = impl, include guards, limiting exposure.
- [headers-and-cpp.md](headers-and-cpp.md) — .h/.cpp pattern.
- [oop-concepts.md](oop-concepts.md) — Encapsulation, inheritance, polymorphism.
- [ORDERBOOK.md](ORDERBOOK.md) — Exchange domain (order book, bids/asks).
- [project-layout.md](project-layout.md) — How to build MerkelMain (scripts/build-MerkelMain.ps1).
- [INDEX.md](INDEX.md) — Doc map and learning path.
