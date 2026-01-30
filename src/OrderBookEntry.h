/*
 * OrderBookEntry.h — declarations for order book entry type, formatting, and helpers.
 *
 * PURPOSE: Declares types (OrderBookType, OrderBookEntry), Format helpers (price, sectionHeader),
 * global orders vector, and free functions (printOrderBook*, compute*, time helpers). CSV loading
 * is in CSVReader (readCSV); see CSVReader.h.
 *
 * DOCS (embedded references):
 *   docs/headers-and-cpp.md — Why .h vs .cpp; declarations in header.
 *   docs/vector-iteration.md — const auto& for read-only loops; memory and iteration.
 *   docs/orderbook-statistics.md — computeAveragePrice, computePriceChange, computePercentChange.
 *   docs/orderbook-time.md — getEarliestTime, getNextTime, getPreviousTime (free functions).
 *
 * PROJECT LAYOUT: Source in src/. Include "OrderBookEntry.h" when building with -Isrc.
 * For CSV: #include "CSVReader.h"; CSVReader::readCSV(path) or readCSV(path, out).
 */

#ifndef ORDERBOOKENTRY_H
#define ORDERBOOKENTRY_H

#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
#include <sstream>

// -------- Output formatting --------
namespace Format {
    inline std::string price(double value, int decimals = 8) {
        std::ostringstream os;
        os << std::fixed << std::setprecision(decimals) << value;
        return os.str();
    }
    inline void sectionHeader(const std::string& title) {
        std::cout << "\n----- " << title << " -----" << std::endl;
    }
}

// -------- OrderBookType: bid or ask --------
enum class OrderBookType { bid, ask };

inline const char* orderBookTypeToString(OrderBookType t) {
    return (t == OrderBookType::bid) ? "bid" : "ask";
}

// -------- OrderBookEntry class --------
class OrderBookEntry {
public:
    double price{0.0};
    double amount{0.0};
    std::string timestamp{"2020/03/17 17:01:24.884492"};
    std::string product{"ETH/BTC"};
    OrderBookType orderType{OrderBookType::bid};

    OrderBookEntry(double price, double amount, std::string timestamp, std::string product, OrderBookType orderType);
    OrderBookEntry() = default;

    void print() const;
};

// -------- Global vector of orders (loaded via CSVReader::readCSV) --------
extern std::vector<OrderBookEntry> orders;

/** Print first maxRows entries (default 5). Three styles (see docs/vector-iteration.md): */
void printOrderBookByIndex(const std::vector<OrderBookEntry>& entries, int maxRows = 5);
void printOrderBookByIterator(const std::vector<OrderBookEntry>& entries, int maxRows = 5);
void printOrderBookByRange(const std::vector<OrderBookEntry>& entries, int maxRows = 5);
void printOrderBook(const std::vector<OrderBookEntry>& entries, int maxRows = 5);

/** Worksheet challenge: stats over a vector of entries. All take const ref; empty vector returns 0.0. */
double computeAveragePrice(const std::vector<OrderBookEntry>& entries);
double computeLowPrice(const std::vector<OrderBookEntry>& entries);
double computeHighPrice(const std::vector<OrderBookEntry>& entries);
double computePriceSpread(const std::vector<OrderBookEntry>& entries);

/** Change since previous time frame: mean(current) - mean(previous). Empty previous → 0.0. */
double computePriceChange(const std::vector<OrderBookEntry>& current, const std::vector<OrderBookEntry>& previous);
/** Percent change: (mean(current) - mean(previous)) / mean(previous) * 100. Empty or zero previous → 0.0. */
double computePercentChange(const std::vector<OrderBookEntry>& current, const std::vector<OrderBookEntry>& previous);

/** Time helpers: earliest/latest timestamp in entries; next/previous in sorted order. Empty string if none. */
std::string getEarliestTime(const std::vector<OrderBookEntry>& entries);
std::string getLatestTime(const std::vector<OrderBookEntry>& entries);
std::string getNextTime(const std::string& currentTime, const std::vector<OrderBookEntry>& entries);
std::string getPreviousTime(const std::string& currentTime, const std::vector<OrderBookEntry>& entries);

#endif /* ORDERBOOKENTRY_H */
