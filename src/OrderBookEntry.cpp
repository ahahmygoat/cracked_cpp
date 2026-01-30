/*
 * OrderBookEntry.cpp — definitions for order book entry, print helpers, and stats.
 *
 * PURPOSE: Implements everything declared in OrderBookEntry.h: global orders, OrderBookEntry
 * members, printOrderBook*, compute* (average/low/high/spread/change/percent), time helpers,
 * and (when ORDERBOOK_STANDALONE) main() for the demo. CSV loading is in CSVReader; see CSVReader.h.
 *
 * DOCS (embedded references):
 *   docs/vector-iteration.md — const auto& for read-only loops; memory and iteration.
 *   docs/orderbook-statistics.md — computeAveragePrice, computePriceChange, computePercentChange.
 *   docs/orderbook-time.md — getEarliestTime, getLatestTime, getNextTime, getPreviousTime.
 *   docs/orderbook-worksheet.md — Teaching steps; docs/headers-and-cpp.md for .h/.cpp split.
 *
 * PROJECT LAYOUT: Source in src/. Build from repo root with -Isrc. See docs/project-layout.md.
 * CSV columns (file order): timestamp, product, orderType, amount, price.
 * Constructor parameter order: (price, amount, timestamp, product, orderType).
 */

#include "OrderBookEntry.h"
#include "CSVReader.h"
#include <algorithm> /* std::min for printOrderBookByIndex */
#include <set>
#include <utility>   /* std::move for constructor (efficiency) */

// -------- Global vector: single definition (declared extern in .h) --------
std::vector<OrderBookEntry> orders;

// -------- OrderBookEntry: constructor and print (declared in .h) --------
OrderBookEntry::OrderBookEntry(double price, double amount, std::string timestamp,
                               std::string product, OrderBookType orderType)
    : price(price), amount(amount), timestamp(std::move(timestamp)),
      product(std::move(product)), orderType(orderType) {}

void OrderBookEntry::print() const {
    std::cout << "Order: " << Format::price(amount) << " " << product
              << " at " << Format::price(price) << " on " << timestamp
              << " " << orderBookTypeToString(orderType) << std::endl;
}

// -------- Print first N entries: three styles (see docs/vector-iteration.md) --------
void printOrderBookByIndex(const std::vector<OrderBookEntry>& entries, int maxRows) {
    const size_t end = std::min(entries.size(), static_cast<size_t>(maxRows));
    for (size_t i = 0; i < end; ++i) {
        entries[i].print();
    }
}

void printOrderBookByIterator(const std::vector<OrderBookEntry>& entries, int maxRows) {
    int n = 0;
    for (auto it = entries.begin(); it != entries.end() && n < maxRows; ++it, ++n) {
        it->print();
    }
}

void printOrderBookByRange(const std::vector<OrderBookEntry>& entries, int maxRows) {
    int n = 0;
    for (const auto& entry : entries) {
        if (n >= maxRows) break;
        entry.print();
        ++n;
    }
}

void printOrderBook(const std::vector<OrderBookEntry>& entries, int maxRows) {
    printOrderBookByRange(entries, maxRows);
}

// -------- Worksheet challenge: compute stats over entries --------
double computeAveragePrice(const std::vector<OrderBookEntry>& entries) {
    if (entries.empty()) return 0.0;
    double sum = 0.0;
    for (const auto& e : entries) sum += e.price;
    return sum / static_cast<double>(entries.size());
}

double computeLowPrice(const std::vector<OrderBookEntry>& entries) {
    if (entries.empty()) return 0.0;
    double low = entries[0].price;
    for (const auto& e : entries)
        if (e.price < low) low = e.price;
    return low;
}

double computeHighPrice(const std::vector<OrderBookEntry>& entries) {
    if (entries.empty()) return 0.0;
    double high = entries[0].price;
    for (const auto& e : entries)
        if (e.price > high) high = e.price;
    return high;
}

double computePriceSpread(const std::vector<OrderBookEntry>& entries) {
    return computeHighPrice(entries) - computeLowPrice(entries);
}

// -------- Change since previous time frame (see docs/orderbook-statistics.md) --------
double computePriceChange(const std::vector<OrderBookEntry>& current, const std::vector<OrderBookEntry>& previous) {
    if (previous.empty()) return 0.0;
    double meanPrev = computeAveragePrice(previous);
    double meanCurr = computeAveragePrice(current);
    return meanCurr - meanPrev;
}

double computePercentChange(const std::vector<OrderBookEntry>& current, const std::vector<OrderBookEntry>& previous) {
    if (previous.empty()) return 0.0;
    double meanPrev = computeAveragePrice(previous);
    if (meanPrev == 0.0) return 0.0;
    double meanCurr = computeAveragePrice(current);
    return (meanCurr - meanPrev) / meanPrev * 100.0;
}

// -------- Time helpers (used by OrderBook and MerkelMain; see docs/orderbook-time.md) --------
// Scan entries for min/max timestamp; next/prev use set of unique timestamps in sorted order.

std::string getEarliestTime(const std::vector<OrderBookEntry>& entries) {
    if (entries.empty()) return "";
    std::string earliest = entries[0].timestamp;
    for (const auto& e : entries) {
        if (e.timestamp < earliest) earliest = e.timestamp;
    }
    return earliest;
}

std::string getLatestTime(const std::vector<OrderBookEntry>& entries) {
    if (entries.empty()) return "";
    std::string latest = entries[0].timestamp;
    for (const auto& e : entries) {
        if (e.timestamp > latest) latest = e.timestamp;
    }
    return latest;
}

std::string getNextTime(const std::string& currentTime, const std::vector<OrderBookEntry>& entries) {
    std::set<std::string> timestamps;
    for (const auto& e : entries) timestamps.insert(e.timestamp);
    auto it = timestamps.upper_bound(currentTime);
    return (it != timestamps.end()) ? *it : "";
}

std::string getPreviousTime(const std::string& currentTime, const std::vector<OrderBookEntry>& entries) {
    std::set<std::string> timestamps;
    for (const auto& e : entries) timestamps.insert(e.timestamp);
    auto it = timestamps.lower_bound(currentTime);  // first >= currentTime
    if (it == timestamps.begin()) return "";
    return *std::prev(it);
}

// -------- Entry point for the order book demo (only when building standalone) --------
// Build standalone: scripts/build-OrderBookEntry.ps1 (defines ORDERBOOK_STANDALONE)
// MerkelMain build does not define it, so this main() is omitted and MerkelMain.cpp provides main.
#ifdef ORDERBOOK_STANDALONE
// Expects data/order_book_example.csv relative to the current working directory.
int main() {
    const char* path = "data/order_book_example.csv";
    orders = CSVReader::readCSV(path);
    int count = static_cast<int>(orders.size());
    if (count == 0) {
        std::cerr << "No orders loaded. Check path: " << path << std::endl;
        return 1;
    }

    const int maxRows = 5;
    std::cout << "Loaded " << count << " orders. Showing first " << maxRows << " (3 ways):" << std::endl;

    Format::sectionHeader("1. Index-based (entries[i])");
    printOrderBookByIndex(orders, maxRows);

    Format::sectionHeader("2. Iterator-based (it->print())");
    printOrderBookByIterator(orders, maxRows);

    Format::sectionHeader("3. Range-based for (const auto& entry)");
    printOrderBookByRange(orders, maxRows);

    Format::sectionHeader("Stats (worksheet challenge)");
    std::cout << "Average price: " << Format::price(computeAveragePrice(orders)) << std::endl;
    std::cout << "Low price:     " << Format::price(computeLowPrice(orders)) << std::endl;
    std::cout << "High price:    " << Format::price(computeHighPrice(orders)) << std::endl;
    std::cout << "Price spread:  " << Format::price(computePriceSpread(orders)) << std::endl;

    return 0;
}
#endif // ORDERBOOK_STANDALONE
