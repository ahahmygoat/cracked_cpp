/*
 * OrderBook.h — order book: holds entries loaded from CSV and filters by product/timestamp.
 *
 * PURPOSE: Holds a snapshot of order book entries (bids and asks). Load from CSV via
 * CSVReader::readCSV; then filter by product, order type, and timestamp for matching
 * or display. See docs/orderbook-matching.md for how matching uses this data.
 *
 * USE: Include "OrderBook.h" and "OrderBookEntry.h"; link OrderBook.cpp. Build with -Isrc.
 */

#pragma once

#include "OrderBookEntry.h"
#include "CSVReader.h"
#include <vector>
#include <string>

class OrderBook {
public:
    /** Load order book from CSV file (e.g. data/order_book_example.csv). */
    explicit OrderBook(const std::string& filename);

    /** Unique product names (trading pairs) in the book. May contain duplicates if you don't dedupe; see .cpp. */
    std::vector<std::string> getKnownProducts() const;

    /** All entries for the given product, order type (bid/ask), and timestamp. Used to get bid side or ask side for matching. */
    std::vector<OrderBookEntry> getOrders(OrderBookType type, const std::string& product, const std::string& timestamp) const;

    /** Append one order to the book. */
    void insertOrder(const OrderBookEntry& order);

    /** All entries for the given product and timestamp (both bids and asks). Input for a matching engine. */
    std::vector<OrderBookEntry> matchOrders(const std::string& product, const std::string& timestamp) const;

private:
    /** Snapshot of order book entries (loaded from CSV or inserted). */
    std::vector<OrderBookEntry> orders_;
};
