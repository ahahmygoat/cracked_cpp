# Order book matching: how it works and how we use the data

This doc explains **how order book matching works** (bids vs asks, when a trade can happen) and **how the data and code in this repo support it**. It ties the CSV data and the `OrderBook` / `OrderBookEntry` types to the matching idea.

**Takeaway:** A **match** happens when a **bid price ≥ ask price** (and the exchange rules say so). The CSV gives us a **snapshot** of the book at a timestamp; we filter by product and timestamp, then a matching engine would compare bids and asks to decide trades. See [ORDERBOOK.md](ORDERBOOK.md) for the big picture; this doc focuses on **matching** and **using the data**.

---

## 1. What is matching?

**Matching** is the process of pairing **buy orders (bids)** with **sell orders (asks)** so that a **trade** can occur.

- A **bid** says: “I will buy this **amount** at this **price** (or better).”
- An **ask** says: “I will sell this **amount** at this **price** (or better).”
- A **match** is possible when the bidder’s price is **at least as high** as the asker’s price: **bid price ≥ ask price**.

When that’s true, the exchange can execute a trade: the buyer gets the asset, the seller gets the payment, at an agreed price (often the ask price or a rule-based price).

---

## 2. How matching works (step by step)

A typical **matching engine** does something like:

1. **Get the order book** for a product (e.g. ETH/BTC) at a given time (or for the next event).
2. **Split into two sides:** all **bids** and all **asks** for that product (and optionally that timestamp).
3. **Sort and prioritize:**  
   - Bids: usually **highest price first** (best bid first).  
   - Asks: usually **lowest price first** (best ask first).
4. **Compare:**  
   - **Best bid** = highest bid price.  
   - **Best ask** = lowest ask price.  
   - If **best bid ≥ best ask**, they can match.
5. **Execute:** Decide trade size (min of bid amount and ask amount), price (e.g. ask price or mid), then remove or reduce those orders and repeat until no more crosses.

So: **matching uses the data** by filtering to a product (and time), separating bids and asks, then comparing prices and amounts.

---

## 3. The data we have: CSV snapshot

Our file **`data/order_book_example.csv`** is a **snapshot**: each row is one **order book entry** at a single timestamp.

**Columns:** `timestamp`, `product` (trading pair), `orderType` (bid/ask), `amount`, `price`.

Example rows for **ETH/BTC** at **2020/03/17 17:01:24.884492**:

**Bids (buy orders):**

| amount     | price     |
| ---------- | --------- |
| 0.02187308 | 7.44564869 |
| 0.02187307 | 3.467434   |
| 0.02185561 | 9.14455526 |
| 0.02184944 | 130.       |
| …          | …          |

**Asks (sell orders):**

| amount     | price     |
| ---------- | --------- |
| 0.02189093 | 9.80492203 |
| 0.02189096 | 6.85752424 |
| 0.0219     | 0.04       |
| 0.02189434 | 130.       |
| …          | …          |

So at that timestamp we have **many bids** and **many asks** for ETH/BTC. A matching engine would:

- Take **all bids** for ETH/BTC at that time → sort by price descending (best bid first).
- Take **all asks** for ETH/BTC at that time → sort by price ascending (best ask first).
- Check: is **best bid ≥ best ask**? If yes, match and continue.

In this snapshot, for example, a bid at **130** and an ask at **0.04** satisfy **bid price ≥ ask price**, so they *could* match (subject to amount and exchange rules).

---

## 4. How the code uses the data

In this repo we **load the CSV** into a list of **`OrderBookEntry`** objects (timestamp, product, orderType, amount, price). The **`OrderBook`** class holds those entries and gives you filtered views that a matching engine would feed on.

### Loading the book

- **CSVReader::readCSV(filename)** loads the CSV into a `std::vector<OrderBookEntry>`.
- **OrderBook** (e.g. `OrderBook book("data/order_book_example.csv");`) uses that to fill its internal list of orders.

So: **one CSV row → one `OrderBookEntry`**; the order book in memory is the full snapshot.

### Filtering by product and timestamp

A matching engine needs “all bids for product X at time T” and “all asks for product X at time T”:

- **getOrders(type, product, timestamp)** — returns all entries for that **product**, **order type** (bid or ask), and **timestamp**.  
  Example: `getOrders(OrderBookType::bid, "ETH/BTC", "2020/03/17 17:01:24.884492")` gives you the bid side for that product and time.

- **matchOrders(product, timestamp)** — in our current code, returns **all** entries (bids and asks) for that product and timestamp. So you get the “slice” of the book for that market and time; a real matching engine would then split that into bids and asks and run the match logic.

So: **the data is used** by filtering to (product, timestamp) and optionally by type (bid/ask). The CSV columns map directly to `OrderBookEntry` fields.

### What “matching” means in code (next step)

Today, **matchOrders** only returns the set of orders for that product and timestamp; it does **not** run price comparison or produce trades. A full matching engine would:

1. Call `getOrders(OrderBookType::bid, product, timestamp)` and `getOrders(OrderBookType::ask, product, timestamp)`.
2. Sort bids by price descending, asks by price ascending.
3. While best bid ≥ best ask: compute trade size, record trade, remove or reduce orders, repeat.

So the **data** (and `OrderBook` / `OrderBookEntry`) give you the input; the **matching algorithm** is the logic that uses that input to produce trades.

---

## 5. Summary: matching and the data

| Idea | How we use the data |
|------|----------------------|
| **Match** | Bid price ≥ ask price → trade can happen. |
| **Data** | CSV rows = order book entries (timestamp, product, bid/ask, amount, price). |
| **Load** | `CSVReader::readCSV` → `OrderBook` holds `std::vector<OrderBookEntry>`. |
| **Filter** | `getOrders(type, product, timestamp)` = bids or asks for that product and time. |
| **Slice** | `matchOrders(product, timestamp)` = all orders for that product and time (input for a matching engine). |
| **Real matching** | Take filtered bids/asks, sort by price, compare best bid vs best ask, execute trades. |

---

## 6. Related docs

- [ORDERBOOK.md](ORDERBOOK.md) — Order book and dataset overview (bids/asks, CSV format).
- [orderbook-worksheet.md](orderbook-worksheet.md) — Teaching steps for OrderBookEntry and helpers.
- [tokenizer.md](tokenizer.md) — How we parse CSV lines (CSVReader).
- [INDEX.md](INDEX.md) — Doc map and learning path.
