# Order book time: timestamps, current time step, stepping

This doc explains **how time works** in our order book: timestamps in the data, **earliest/latest/next/previous** time helpers, and the **current time window** used by MerkelMain for stats and stepping.

**Takeaway:** Each CSV row has a **timestamp**; the book can be queried by (product, timestamp). We provide **getEarliestTime**, **getLatestTime**, **getNextTime**, **getPreviousTime** so the app can step through time. **Stats** (option 2) show the **current time window** only; option 6 advances to the next timestamp. See [orderbook-matching.md](orderbook-matching.md) and [merkel-main.md](merkel-main.md).

---

## 1. Timestamps in the data

The CSV has a **timestamp** column (e.g. `2020/03/17 17:01:24.884492`). Many rows share the same timestamp — that’s one **snapshot** of the book at that instant. The order book is stored by **(product, timestamp)** so we can look up “all orders for ETH/BTC at that time.”

---

## 2. Time helpers (OrderBookEntry and OrderBook)

**OrderBookEntry** (free functions in OrderBookEntry.h/cpp):

| Function | Meaning |
|----------|---------|
| **getEarliestTime(entries)** | Minimum timestamp in the vector (scan; entries need not be sorted). |
| **getLatestTime(entries)** | Maximum timestamp in the vector. |
| **getNextTime(currentTime, entries)** | Next timestamp after `currentTime` in sorted order (unique timestamps from entries). Empty string if none. |
| **getPreviousTime(currentTime, entries)** | Previous timestamp before `currentTime`. Empty string if none. |

**OrderBook** (methods that delegate to the above using `getAllEntries()`):

| Method | Meaning |
|--------|---------|
| **getEarliestTime()** | Earliest timestamp in the whole book. |
| **getLatestTime()** | Latest timestamp in the whole book. |
| **getNextTime(currentTime)** | Next timestamp after current (for stepping). |
| **getPreviousTime(currentTime)** | Previous timestamp. |
| **getAllEntriesAtTime(timestamp)** | All orders at that timestamp (any product). Used for **current time window** stats. |

---

## 3. Current time step in MerkelMain

MerkelMain keeps a **current timestamp** (`currentTimestamp_`):

- **init()** sets it to **orderBook_.getEarliestTime()** after loading.
- **printMarketStats()** (option 2) shows stats **only for the current time window**: it uses **orderBook_.getAllEntriesAtTime(currentTimestamp_)** and prints count, average/low/high/spread, and best bid/ask for the first product at that time.
- **continueToNextTimeStep()** (option 6) sets **currentTimestamp_ = orderBook_.getNextTime(currentTimestamp_)**. If there is no next time, it prints “End of order book.”

So “current time” is the slice of the book we’re looking at; stats and stepping are based on that slice.

---

## 4. Summary

| Idea | Where |
|------|--------|
| Timestamp per row | CSV column; OrderBookEntry.timestamp. |
| Book keyed by (product, timestamp) | OrderBook map; getOrders, matchOrders, getAllEntriesAtTime. |
| Earliest / latest / next / previous | OrderBookEntry free functions; OrderBook methods. |
| Current time window | MerkelMain.currentTimestamp_; set in init, advanced in continueToNextTimeStep. |
| Stats for current time | printMarketStats uses getAllEntriesAtTime(currentTimestamp_). |

---

## Related docs

- [orderbook-matching.md](orderbook-matching.md) — Matching and filtering by product/timestamp.
- [merkel-main.md](merkel-main.md) — MerkelMain flow, menu, current time, build/run.
- [ORDERBOOK.md](ORDERBOOK.md) — Order book and CSV format.
- [INDEX.md](INDEX.md) — Doc map and learning path.
