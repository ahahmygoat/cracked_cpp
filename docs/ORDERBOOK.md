# Order book and dataset overview

This document describes **what an order book is**, **how it works**, and **how the example dataset** (`data/order_book_example.csv`) is structured. It’s the underlying data we use to build the exchange simulation.

**Why this doc:** So you understand the _domain_ (order book, bids/asks, matching) before modeling it in code. The design of our `Order` struct and CSV reader follows this layout; see [cpp-classes.md](cpp-classes.md) and [ClassesandData.md](ClassesandData.md).

**Principal / PM takeaway:** Domain first, code second. One row in the CSV = one object in memory (`OrderBookEntry`). The matching engine is a separate *system* component: it consumes the order book and produces trades. When product says "add a new market" or "change match rules," you know: new market = new product column / filter; match rules = matching engine logic. See [DESIGN.md](DESIGN.md) for system boundaries.

---

## 1. What is an order book?

An **order book** is the list of **buy** and **sell** orders that participants have placed on an exchange. It’s the core of how a currency (or asset) exchange works.

- **Bids** — people offering to **buy** at a certain price and amount.
- **Asks** — people offering to **sell** at a certain price and amount.

The order book is “people offering to buy things and people offering to sell things.” The exchange then **matches** those bids and asks so that trades happen when conditions are met.

---

## 2. How does it work?

### Matching engine

A **matching engine** is the logic that:

1. Takes all the bids and asks in the order book.
2. **Matches** them when a bid price is at least as high as an ask price (and other rules).
3. Decides **which trades actually happen** and at **what price**.

So: if I **offer to sell** at a price, and you **bid** that same price (or more), we can be matched and a trade occurs.

### Price priority and what you pay

- **Price priority** — if you **bid higher** than others, your order can be served **first** (you’re at the front of the queue for that side).
- **What you pay** — even if you bid very high, you typically **only pay the seller’s ask price** (the price they were willing to sell at). So:
  - You get **priority** by bidding high.
  - You still **pay** the matched ask price, not your full bid (depending on exchange rules).

That’s how “if I bid really high, my bid goes in first, but I’ll still only pay what the person’s asking.” There are ways to use this (e.g. aggressive bids for priority), but there is risk (e.g. accidentally paying more than intended if rules or data are wrong).

### When a bid and an ask don’t match

- **Bid** — “I will buy at **5,319** (per unit).”
- **Ask** — “I will sell at **5,400** (per unit).”

The bid is **below** the ask, so they **do not match**. No trade. For a match we need the bid to be **at or above** the ask (and the matching engine applies the exact rules).

---

## 3. The dataset: what is it?

The dataset is a **list of order book entries**, one per row, **time-stamped**. It was pulled from an open exchange API (e.g. with a script that connects and downloads the data). We use it to:

- **Test** our simulation.
- **Run** the simulation against real-looking order book data.

The file we use is **`data/order_book_example.csv`**: a **CSV (comma-separated values)** file with many rows and several columns. Each row is **one entry** in the order book at a given time.

---

## 4. CSV format: columns

The example file has **no header row**. The columns are (in order):

| Column           | Meaning                                                                       |
| ---------------- | ----------------------------------------------------------------------------- |
| **timestamp**    | Date and time of the order book snapshot (e.g. `2020/03/17 17:01:24.884492`). |
| **trading_pair** | The market, e.g. `ETH/BTC` (ETH quoted in BTC).                               |
| **order_type**   | Either `bid` (buy) or `ask` (sell).                                           |
| **amount**       | Quantity the participant is willing to buy or sell (in base asset units).     |
| **price**        | Price per unit they are willing to buy or sell at.                            |

So each row is: **at this time**, **in this market**, **one bid or ask** of **this amount** at **this price**. For how we split each line into these columns in code (tokenizer pattern), see [tokenizer.md](tokenizer.md).

### Example rows (from the file)

**Bid (offer to buy):**

```text
2020/03/17 17:01:24.884492,ETH/BTC,bid,0.02187308,7.44564869
```

Meaning: at that time, on ETH/BTC, someone is **bidding** to buy **0.02187308** (amount) at **7.44564869** (price per unit).

**Ask (offer to sell):**

```text
2020/03/17 17:01:24.884492,ETH/BTC,ask,0.02189093,9.80492203
```

Meaning: at that time, on ETH/BTC, someone is **asking** to sell **0.02189093** (amount) at **9.80492203** (price per unit).

Whether these two match depends on the matching rules and how bid/ask prices compare (e.g. bid price ≥ ask price for a match).

---

## 5. What we have in the file

- **Thousands of rows** — each row is one order book entry (one bid or one ask).
- **Same timestamp** — many rows share the same timestamp (one snapshot of the full book).
- **Bids and asks** — both sides of the book are present; the matching engine would decide which pairs actually trade.

---

## 6. Summary: order types and fields

| Concept             | Meaning                                                                                 |
| ------------------- | --------------------------------------------------------------------------------------- |
| **Order book**      | List of bids (buy orders) and asks (sell orders).                                       |
| **Bid**             | “I will buy this **amount** at this **price**.”                                         |
| **Ask**             | “I will sell this **amount** at this **price**.”                                        |
| **Matching engine** | Algorithm that matches bids and asks and decides which trades happen and at what price. |
| **Dataset**         | Time-stamped CSV: each row = one bid or ask (timestamp, pair, type, amount, price).     |

---

## 7. Next step: modeling with code

The next step is to **model this in code** using object-oriented techniques: e.g. types or classes for an **order** (timestamp, trading pair, bid/ask, amount, price) and structures for the **order book** and **matching engine**, then feed the simulation from **`data/order_book_example.csv`**.

**Worksheet teaching steps:** The coding workshop in **2313_v3.pdf** (Modelling an order book entry as a class) is mirrored in [orderbook-worksheet.md](orderbook-worksheet.md): class definition, data members, constructor (prototype + implementation, member initialiser list), vector of objects, push_back/emplace_back, range-based for, const reference, and the challenge functions (computeAveragePrice, computeLowPrice, computeHighPrice, computePriceSpread). The code is split into **OrderBookEntry.h** (declarations) and **OrderBookEntry.cpp** (definitions); see [headers-and-cpp.md](headers-and-cpp.md) for the .h/.cpp pattern.

---

## Related docs

- [SETUP.md](SETUP.md) — Project setup and running the app.
- [ClassesandData.md](ClassesandData.md) — Data types and classes.
- [DESIGN.md](DESIGN.md) — OOP and system boundaries (principal/PM).
- [orderbook-worksheet.md](orderbook-worksheet.md) — Teaching steps tied to OrderBookEntry (src/).
- [tokenizer.md](tokenizer.md) — How we split each CSV line in code (CSVReader).
- [INDEX.md](INDEX.md) — Doc map and learning path.
