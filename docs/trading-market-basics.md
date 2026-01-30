# Trading and market basics: bid, ask, spread, best prices

This doc gives a short **trading/market** view of the order book: **bid**, **ask**, **best bid/ask**, **spread**, and why they matter. It ties to our code (OrderBook, getBestBid, getBestAsk) and data.

**Takeaway:** **Bids** = buy orders, **asks** = sell orders. **Best bid** = highest buy price; **best ask** = lowest sell price. The **spread** (best ask − best bid) is the gap between them; a trade can happen when best bid ≥ best ask. See [ORDERBOOK.md](ORDERBOOK.md) and [orderbook-matching.md](orderbook-matching.md).

---

## 1. Bid and ask

| Term | Meaning | In our data |
|------|--------|-------------|
| **Bid** | An order to **buy** a given amount at a given price (or better). | CSV column `orderType = bid`. |
| **Ask** | An order to **sell** a given amount at a given price (or better). | CSV column `orderType = ask`. |

The **order book** is the list of all bids and asks. One side wants to buy, the other to sell; the exchange matches them when prices cross.

---

## 2. Best bid and best ask

| Term | Meaning | In our code |
|------|--------|-------------|
| **Best bid** | The **highest** bid price (buyers compete; the top of the bid side). | **OrderBook::getBestBid(product, timestamp)**. |
| **Best ask** | The **lowest** ask price (sellers compete; the bottom of the ask side). | **OrderBook::getBestAsk(product, timestamp)**. |

For a given **product** and **timestamp**, we take all bids and pick the max price (best bid), and all asks and pick the min price (best ask). MerkelMain’s “Print exchange stats” (option 2) shows best bid and best ask for the first product at the **current time**.

---

## 3. Spread

The **spread** is:

**Spread = best ask − best bid**

- If the spread is **positive**, there’s a gap: no trade at that moment (highest buyer is below lowest seller).
- If **best bid ≥ best ask**, the spread can be zero or negative and a **match** is possible; the matching engine decides trades and price.

We also compute **price spread** over a set of entries (max price − min price) for stats; that’s a different notion (range of prices in the slice). See OrderBookEntry’s **computePriceSpread(entries)**.

---

## 4. Why it matters (engineering / product)

- **Liquidity:** Many orders near best bid/ask mean tighter spreads and easier execution.
- **Priority:** Price priority: best bid is served first among buyers; best ask first among sellers. Time priority is often used within the same price.
- **Display:** UIs show “best bid” and “best ask” (and sometimes depth) so users see where the market is.
- **Matching:** The engine compares best bid and best ask; when they cross, it executes. See [orderbook-matching.md](orderbook-matching.md).

---

## 5. In this repo

| Concept | Where |
|--------|--------|
| Bid/ask in data | CSV column orderType; OrderBookEntry.orderType. |
| Best bid / best ask | OrderBook::getBestBid, getBestAsk(product, timestamp). |
| Stats for current time | MerkelMain::printMarketStats uses getAllEntriesAtTime(currentTimestamp_) and shows best bid/ask for first product. |
| Spread (match view) | best ask − best bid; match when best bid ≥ best ask. |

---

## Related docs

- [ORDERBOOK.md](ORDERBOOK.md) — Order book and dataset.
- [orderbook-matching.md](orderbook-matching.md) — How matching uses bids/asks and data.
- [orderbook-time.md](orderbook-time.md) — Current time step and time helpers.
- [merkel-main.md](merkel-main.md) — Exchange app and stats.
- [INDEX.md](INDEX.md) — Doc map and learning path.
