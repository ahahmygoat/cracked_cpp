/*
 * OrderBook.cpp — implementation of OrderBook: load CSV, filter by product/timestamp.
 *
 * PURPOSE: Constructor loads entries via CSVReader::readCSV. getOrders / matchOrders
 * filter the snapshot for a product and (optionally) timestamp and order type.
 * See docs/orderbook-matching.md for how matching uses getOrders(type, product, timestamp).
 *
 * BUILD: Include in targets that use OrderBook (e.g. MerkelMain). Compile with -Isrc.
 */

#include "OrderBook.h"

// -------- Constructor --------

OrderBook::OrderBook(const std::string& filename) {
    orders_ = CSVReader::readCSV(filename);
}

// -------- Known products --------
// Returns product names (trading pairs) that appear in the book.
// Currently returns one entry per order (duplicates possible); caller can dedupe if needed.

std::vector<std::string> OrderBook::getKnownProducts() const {
    std::vector<std::string> products;
    products.reserve(orders_.size());  // optional: reduce reallocations
    for (const OrderBookEntry& e : orders_) {
        products.push_back(e.product);
    }
    return products;
}

// -------- Filter by type, product, timestamp --------
// Used by a matching engine: get bids with getOrders(OrderBookType::bid, product, timestamp),
// asks with getOrders(OrderBookType::ask, product, timestamp).

std::vector<OrderBookEntry> OrderBook::getOrders(OrderBookType type, const std::string& product, const std::string& timestamp) const {
    std::vector<OrderBookEntry> filtered;
    for (const OrderBookEntry& e : orders_) {
        if (e.product == product && e.orderType == type && e.timestamp == timestamp) {
            filtered.push_back(e);
        }
    }
    return filtered;
}

// -------- Insert --------

void OrderBook::insertOrder(const OrderBookEntry& order) {
    orders_.push_back(order);
}

// -------- Slice for matching --------
// Returns all orders (bids and asks) for this product and timestamp.
// A matching engine would then split into bids/asks, sort by price, and compare best bid vs best ask.

std::vector<OrderBookEntry> OrderBook::matchOrders(const std::string& product, const std::string& timestamp) const {
    std::vector<OrderBookEntry> slice;
    for (const OrderBookEntry& e : orders_) {
        if (e.product == product && e.timestamp == timestamp) {
            slice.push_back(e);
        }
    }
    return slice;
}
