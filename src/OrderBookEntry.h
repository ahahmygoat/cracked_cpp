/*
 * OrderBookEntry.h — declarations for order book entry type, formatting, and helpers.
 *
 * PURPOSE: Header file for the order book demo. Declares types (OrderBookType, OrderBookEntry),
 * formatting helpers (Format::price, Format::sectionHeader), the global orders vector, and
 * free functions (printOrderBook*, compute*). CSV loading is in CSVReader (readCSV); see CSVReader.h.
 *
 * PROJECT LAYOUT: Source in src/. Include as "OrderBookEntry.h" when building with -Isrc.
 *
 * WHY .h AND .cpp? Declarations go in the header so other .cpp files can #include "OrderBookEntry.h"
 * and use the types/functions without recompiling the implementation. See docs/headers-and-cpp.md.
 *
 * INCLUDE: Use #include "OrderBookEntry.h" in any .cpp that uses OrderBookEntry, printOrderBook, etc.
 * For loading CSV: #include "CSVReader.h" and use CSVReader::readCSV(path) or readCSV(path, out).
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

#endif /* ORDERBOOKENTRY_H */
