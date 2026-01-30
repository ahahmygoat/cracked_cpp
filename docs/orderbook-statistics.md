# Order book statistics: why they matter and how to verify

This doc explains **why more sophisticated statistics** matter for a currency trading system, **what we compute** (mean, low, high, spread, change since last time frame), and **how to verify** your algorithms with test data.

**Takeaway:** Users need to **examine patterns** in currency values over time to make better trading decisions. Low and high give a range; the **mean** (average) gives a typical level; **change since the previous time frame** shows how the market moved — together they give greater insight into market dynamics. **Learning goals:** write functions that calculate these statistics; use test data (e.g. the CSV and the app) to verify your algorithm.

---

## 1. Why more sophisticated statistics?

If your purpose is to build a **currency trading system**, it is important for users to be able to **examine the patterns** that happen in the values of the currencies as they change over time. Armed with this knowledge, users can make **better trading decisions**.

- **Low and high** in a time frame tell you the range — what’s the lowest and highest price in that window.
- **Mean (average) value** tells you a typical level — where the market is “centered” in that window.
- **Change in value since the last time frame** tells you whether price is moving up or down between consecutive windows — momentum or reversion.

With mean and change (and low/high/spread), users gain **greater insight into market dynamics** and can trade more effectively. So we add statistics beyond raw min/max: **mean** and **change vs previous time**.

---

## 2. What we calculate

| Statistic | Meaning | In code |
|-----------|---------|--------|
| **Mean price** | Average price over the entries in a time window. | `computeAveragePrice(entries)` |
| **Low price** | Minimum price in the window. | `computeLowPrice(entries)` |
| **High price** | Maximum price in the window. | `computeHighPrice(entries)` |
| **Price spread** | High − low (range of prices). | `computePriceSpread(entries)` |
| **Change vs previous** | Mean(current window) − mean(previous window). | `computePriceChange(current, previous)` |
| **Percent change** | (Mean(current) − mean(previous)) / mean(previous) × 100. | `computePercentChange(current, previous)` |

All of these are in **OrderBookEntry.cpp** (declared in OrderBookEntry.h). **MerkelMain::printMarketStats()** (option 2) shows stats for the **current time window** and, when there is a previous timestamp, **change vs prev** (absolute and percent).

---

## 3. Learning goals: write functions and verify with test data

**Goal 1 — Write functions that calculate statistics:**  
We implement `computeAveragePrice`, `computeLowPrice`, `computeHighPrice`, `computePriceSpread`, `computePriceChange`, and `computePercentChange` over vectors of `OrderBookEntry`. Each takes `const std::vector<OrderBookEntry>&` (and for change, current and previous). See [OrderBookEntry.cpp](../src/OrderBookEntry.cpp) and [OrderBookEntry.h](../src/OrderBookEntry.h).

**Goal 2 — Use test data to verify your algorithm:**  
We use the **CSV order book** as test data:

1. **Run the app** from repo root: `.\run.ps1` (or `.\scripts\build-MerkelMain.ps1`).
2. **Print exchange stats** (option 2). You see mean, low, high, spread for the **current time** and, if not at the earliest time, **change vs prev** (absolute and percent).
3. **Advance one time step** (option 6). Current time moves to the next timestamp.
4. **Print stats again** (option 2). Mean and change vs prev update; you can check that “change vs prev” is consistent with the previous window’s mean (e.g. if mean went up, change is positive).
5. **Sanity check:** At the first timestamp, “Change vs prev” shows “(no previous time)”. After stepping, you get a numeric change — verify it matches mean(current) − mean(previous) by hand if you like.

So the **test data** is the loaded order book; the **verification** is: run the app, step through time, and confirm the printed statistics (especially mean and change) match your expectations. You can also add unit tests (e.g. a small vector of entries with known prices and assert on the computed mean and change).

---

## 4. Where this appears in the code

- **OrderBookEntry.h / OrderBookEntry.cpp** — Declarations and definitions of `computeAveragePrice`, `computeLowPrice`, `computeHighPrice`, `computePriceSpread`, `computePriceChange`, `computePercentChange`.
- **MerkelMain.cpp** — `printMarketStats()` gets entries at current time (and at previous time when available), calls the compute functions, and prints mean, low, high, spread, and change vs prev.

---

## 5. Quick reference

| Goal | What to do |
|------|------------|
| **Why these stats** | Mean = typical level; change vs prev = how market moved; low/high/spread = range. |
| **Implement stats** | Add/use functions in OrderBookEntry that take a vector of entries (and for change, current + previous). |
| **Verify with test data** | Run app, option 2 (stats), option 6 (next time), option 2 again; check mean and change vs prev. |

---

## Related docs

- [orderbook-time.md](orderbook-time.md) — Current time step, stepping, getAllEntriesAtTime.
- [merkel-main.md](merkel-main.md) — Exchange app, printMarketStats, build/run.
- [orderbook-worksheet.md](orderbook-worksheet.md) — Teaching steps (computeAveragePrice, etc.).
- [vector-iteration.md](vector-iteration.md) — Iterating with const reference (used inside the compute functions).
- [INDEX.md](INDEX.md) — Doc map and learning path.
