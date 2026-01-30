/*
 * OrderBook.cpp — implementation of OrderBook: load CSV, filter by product/timestamp.
 *
 * PURPOSE: Constructor loads entries via CSVReader::readCSV and groups by (product, timestamp).
 * getOrders / matchOrders look up that map; getBestBid / getBestAsk return highest bid and lowest ask.
 *
 * DOCS (embedded references):
 *   docs/orderbook-matching.md — getOrders(type, product, timestamp) for matching.
 *   docs/trading-market-basics.md — Best bid = highest bid; best ask = lowest ask.
 *   docs/orderbook-time.md — Time helpers delegate to OrderBookEntry free functions.
 *
 * BUILD: Include in targets that use OrderBook (e.g. MerkelMain). Compile with -Isrc.
 */

#include "OrderBook.h"
#include <set>

// -------- Constructor --------
OrderBook::OrderBook(const std::string& filename) {
    load(filename);
}

// -------- load --------
// Clear map and (re)load from CSV; group by (product, timestamp).

void OrderBook::load(const std::string& filename) {
    ordersByProductTime_.clear();
    std::vector<OrderBookEntry> entries = CSVReader::readCSV(filename);
    for (const OrderBookEntry& e : entries) {
        ordersByProductTime_[{e.product, e.timestamp}].push_back(e);
    }
}

// -------- Known products --------
// Unique product names from map keys (one per product).

std::vector<std::string> OrderBook::getKnownProducts() const {
    std::set<std::string> products;
    for (const auto& kv : ordersByProductTime_) {
        products.insert(kv.first.first);  // product from (product, timestamp)
    }
    return std::vector<std::string>(products.begin(), products.end());
}

// -------- Filter by type, product, timestamp --------
// Look up (product, timestamp) in map; filter that bucket by bid/ask.

std::vector<OrderBookEntry> OrderBook::getOrders(OrderBookType type, const std::string& product, const std::string& timestamp) const {
    auto it = ordersByProductTime_.find({product, timestamp});
    if (it == ordersByProductTime_.end()) return {};
    std::vector<OrderBookEntry> filtered;
    for (const OrderBookEntry& e : it->second) {
        if (e.orderType == type) filtered.push_back(e);
    }
    return filtered;
}

// -------- Insert --------

void OrderBook::insertOrder(const OrderBookEntry& order) {
    ordersByProductTime_[{order.product, order.timestamp}].push_back(order);
}

// -------- Slice for matching --------
// Look up (product, timestamp); return that bucket (bids and asks).

std::vector<OrderBookEntry> OrderBook::matchOrders(const std::string& product, const std::string& timestamp) const {
    auto it = ordersByProductTime_.find({product, timestamp});
    if (it == ordersByProductTime_.end()) return {};
    return it->second;
}

// -------- Best bid / best ask --------
// Best bid = highest bid price (buyers compete for priority). Best ask = lowest ask price (sellers).
// Matching: trade when getBestBid() >= getBestAsk(). Returns 0.0 if no orders on that side.

double OrderBook::getBestBid(const std::string& product, const std::string& timestamp) const {
    std::vector<OrderBookEntry> bids = getOrders(OrderBookType::bid, product, timestamp);
    double best = 0.0;
    for (const OrderBookEntry& e : bids) {
        if (e.price > best) best = e.price;
    }
    return best;
}

double OrderBook::getBestAsk(const std::string& product, const std::string& timestamp) const {
    std::vector<OrderBookEntry> asks = getOrders(OrderBookType::ask, product, timestamp);
    if (asks.empty()) return 0.0;
    double best = asks[0].price;
    for (const OrderBookEntry& e : asks) {
        if (e.price < best) best = e.price;
    }
    return best;
}

// -------- getAllEntries --------
// Flat vector of all entries (for stats: computeAveragePrice, etc.).

std::vector<OrderBookEntry> OrderBook::getAllEntries() const {
    std::vector<OrderBookEntry> out;
    for (const auto& kv : ordersByProductTime_) {
        for (const OrderBookEntry& e : kv.second) {
            out.push_back(e);
        }
    }
    return out;
}

// -------- All entries at one timestamp --------
std::vector<OrderBookEntry> OrderBook::getAllEntriesAtTime(const std::string& timestamp) const {
    std::vector<OrderBookEntry> out;
    for (const auto& kv : ordersByProductTime_) {
        if (kv.first.second == timestamp) {
            for (const OrderBookEntry& e : kv.second) {
                out.push_back(e);
            }
        }
    }
    return out;
}

// -------- Time helpers (delegate to OrderBookEntry free functions) --------
std::string OrderBook::getEarliestTime() const {
    return ::getEarliestTime(getAllEntries());
}

std::string OrderBook::getLatestTime() const {
    return ::getLatestTime(getAllEntries());
}

std::string OrderBook::getNextTime(const std::string& currentTime) const {
    return ::getNextTime(currentTime, getAllEntries());
}

std::string OrderBook::getPreviousTime(const std::string& currentTime) const {
    return ::getPreviousTime(currentTime, getAllEntries());
}
