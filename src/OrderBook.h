/*
 * OrderBook.h — order book: holds entries loaded from CSV and filters by product/timestamp.
 *
 * PURPOSE: Holds a snapshot of order book entries (bids and asks). Load from CSV via
 * CSVReader::readCSV; then filter by product, order type, and timestamp for matching
 * or display.
 *
 * DOCS (embedded references):
 *   docs/orderbook-matching.md — How matching uses getOrders(type, product, timestamp).
 *   docs/trading-market-basics.md — Best bid/ask, spread; getBestBid/getBestAsk.
 *   docs/orderbook-time.md — getEarliestTime, getNextTime, getPreviousTime.
 *
 * USE: Include "OrderBook.h" and "OrderBookEntry.h"; link OrderBook.cpp. Build with -Isrc.
 */

#pragma once

#include "OrderBookEntry.h"
#include "CSVReader.h"
#include <map>
#include <string>
#include <vector>

class OrderBook {
public:
    /** Empty order book; call load(filename) to load from CSV. */
    OrderBook() = default;

    /** Load order book from CSV file (e.g. data/order_book_example.csv). */
    explicit OrderBook(const std::string& filename);

    /** (Re)load from CSV; clears current book and fills from file. */
    void load(const std::string& filename);

    /** Unique product names (trading pairs) in the book. */
    std::vector<std::string> getKnownProducts() const;

    /** All entries for the given product, order type (bid/ask), and timestamp. Used to get bid side or ask side for matching. */
    std::vector<OrderBookEntry> getOrders(OrderBookType type, const std::string& product, const std::string& timestamp) const;

    /** Append one order to the book. */
    void insertOrder(const OrderBookEntry& order);

    /** All entries for the given product and timestamp (both bids and asks). Input for a matching engine. */
    std::vector<OrderBookEntry> matchOrders(const std::string& product, const std::string& timestamp) const;

    /** Best bid: highest bid price for this product and timestamp. Returns 0.0 if no bids. */
    double getBestBid(const std::string& product, const std::string& timestamp) const;

    /** Best ask: lowest ask price for this product and timestamp. Returns 0.0 if no asks. */
    double getBestAsk(const std::string& product, const std::string& timestamp) const;

    /** All entries (flat vector) for stats e.g. computeAveragePrice(getAllEntries()). */
    std::vector<OrderBookEntry> getAllEntries() const;

    /** All entries at the given timestamp (any product). For current-time-window stats. */
    std::vector<OrderBookEntry> getAllEntriesAtTime(const std::string& timestamp) const;

    /** Earliest / latest timestamp in the book. Empty string if no entries. */
    std::string getEarliestTime() const;
    std::string getLatestTime() const;
    /** Next / previous timestamp in sorted order. Empty string if none. */
    std::string getNextTime(const std::string& currentTime) const;
    std::string getPreviousTime(const std::string& currentTime) const;

private:
    using ProductTime = std::pair<std::string, std::string>;
    /** Orders grouped by (product, timestamp) for O(log n) lookup. */
    std::map<ProductTime, std::vector<OrderBookEntry>> ordersByProductTime_;
};
