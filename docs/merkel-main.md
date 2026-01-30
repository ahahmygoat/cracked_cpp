# MerkelMain: exchange application

This doc describes **MerkelMain** — the main application class for the Merkel exchange. It covers the **flow** (constructor → init → run), the **menu loop**, **OrderBook** and **current time**, and how to build and run.

**Takeaway:** MerkelMain is the entry-point object: you create it, call **init()** once (load order book, set current time to earliest), then **run()** for the menu loop. It uses **OrderBook** (private **orderBook_**) for data; **currentTimestamp_** is the current time step. **Print exchange stats** (option 2) shows stats **for the current time window**; **Continue** (option 6) advances to the next timestamp. See [orderbook-time.md](orderbook-time.md) and [organizing-code.md](organizing-code.md).

---

## 1. What is MerkelMain?

**MerkelMain** is the main application class for the exchange. It:

- **Constructor** — runs when you create a `MerkelMain`.
- **init()** — one-time setup: load order book via **orderBook_.load(orderBookPath_)**, set **currentTimestamp_** to **orderBook_.getEarliestTime()**.
- **run()** — main loop: print menu, get user choice, validate, handle action, exit when user picks "Continue".

So the program flow is: **main() → create MerkelMain → init() → run()** until the user chooses option 6.

---

## 2. Flow: main → init → run

```
main()
  └── MerkelMain app;
  └── app.init();      // once: load order book, set currentTimestamp_ to earliest
  └── app.run();       // loop until user picks 6
        └── printMenu()
        └── getUserOption()
        └── validateUserOption(userOption)
        └── handleUserOption(choice)
        └── if choice == Continue → break
  └── return 0;
```

**init()** loads the order book with **orderBook_.load(orderBookPath_)** and sets **currentTimestamp_ = orderBook_.getEarliestTime()**. **printMarketStats()** uses **orderBook_.getAllEntriesAtTime(currentTimestamp_)** for stats **for the current time window** (count, average/low/high/spread, best bid/ask for first product). **continueToNextTimeStep()** sets **currentTimestamp_ = orderBook_.getNextTime(currentTimestamp_)**; if there is no next time, it prints "End of order book."

---

## 3. Menu options: MenuOption enum

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
|--------|---------|
| **MerkelMain()** | Constructor. |
| **init()** | One-time setup: load order book (**orderBook_.load**), set **currentTimestamp_** to earliest. |
| **run()** | Main loop: menu → get option → validate → handle → exit on Continue. |
| **printMarketStats()** | Stats **for current time window**: orders at current time, average/low/high/spread, best bid/ask (first product). Uses **orderBook_.getAllEntriesAtTime(currentTimestamp_)**. |
| **continueToNextTimeStep()** | Advance **currentTimestamp_** to **orderBook_.getNextTime(currentTimestamp_)**; "End of order book" if none. |
| **orderBook_** | Private **OrderBook**; holds entries by (product, timestamp). |
| **currentTimestamp_** | Private; current time step (earliest after init; advances on Continue). |

Other methods: **printMenu()**, **getUserOption()**, **validateUserOption()**, **readAmountAndPrice()**, **handleUserOption()**, **printHelp()**, **makeOffer()**, **makeBid()**, **printWallet()**.

---

## 5. Build and run

MerkelMain uses **OrderBook**, **OrderBookEntry**, and **CSVReader**. Output goes to **build/MerkelMain.exe** (avoids "Permission denied" when the exe in root is locked).

**From repo root:**

```powershell
.\run.ps1
# or
.\scripts\build-MerkelMain.ps1
```

Run from repo root so **data/order_book_example.csv** is found. The script creates **build/** if needed and runs **.\build\MerkelMain.exe**.

**Manual build (from repo root):**

```powershell
g++ -std=c++17 -Wall -g -Isrc -o build/MerkelMain.exe src/MerkelMain.cpp src/OrderBookEntry.cpp src/OrderBook.cpp src/CSVReader.cpp
.\build\MerkelMain.exe
```

---

## 6. Relation to other code

- **OrderBook** — Holds entries by (product, timestamp); **getEarliestTime**, **getNextTime**, **getAllEntriesAtTime**, **getBestBid**, **getBestAsk**. See [orderbook-matching.md](orderbook-matching.md) and [orderbook-time.md](orderbook-time.md).
- **OrderBookEntry** — Entry type and helpers (Format, compute*, getEarliestTime/getLatestTime/getNextTime/getPreviousTime over a vector).
- **refactorMain.cpp** — Same menu as free functions; MerkelMain wraps that flow in a class.
- **main.cpp** — Simpler single-pass menu; no OrderBook.

---

## 7. Quick reference

| Topic | Where |
|-------|--------|
| Declarations | MerkelMain.h (MenuOption, MerkelMain, private orderBookPath_, orderBook_, currentTimestamp_) |
| Definitions | MerkelMain.cpp (all method bodies + main); links with OrderBookEntry.cpp, OrderBook.cpp, CSVReader.cpp |
| Entry point | main() in MerkelMain.cpp → MerkelMain app; app.init(); app.run(); |
| Current time | currentTimestamp_; set in init(), advanced in continueToNextTimeStep(); stats use getAllEntriesAtTime(currentTimestamp_) |
| Build output | build/MerkelMain.exe (see [project-layout.md](project-layout.md)) |

---

## Related docs

- [orderbook-time.md](orderbook-time.md) — Time helpers, current time step, stepping.
- [orderbook-statistics.md](orderbook-statistics.md) — Why stats matter (mean, change vs prev), verify with test data.
- [orderbook-matching.md](orderbook-matching.md) — Matching and filtering by product/timestamp.
- [trading-market-basics.md](trading-market-basics.md) — Bid/ask, best bid/ask, spread.
- [organizing-code.md](organizing-code.md) — Limiting exposure, embedding init.
- [headers-and-cpp.md](headers-and-cpp.md) — .h/.cpp pattern.
- [project-layout.md](project-layout.md) — How to build MerkelMain (scripts, build/ output).
- [INDEX.md](INDEX.md) — Doc map and learning path.
