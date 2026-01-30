/*
 * MerkelMain.cpp — definitions for the Merkel exchange application.
 *
 * PURPOSE: Implements everything declared in MerkelMain.h. One translation unit holds
 * MerkelMain constructor, init(), run(), menu/action methods, and main().
 *
 * DOCS (embedded references):
 *   docs/INDEX.md           — Doc map; noob vs principal paths.
 *   docs/merkel-main.md     — MerkelMain class, init/run, time-stepping.
 *   docs/orderbook-time.md  — Timestamps, getNextTime/getPreviousTime, currentTimestamp_.
 *   docs/orderbook-statistics.md — Mean, spread, change vs prev; computeAveragePrice, etc.
 *   docs/trading-market-basics.md — Bid, ask, best bid/ask, spread.
 *   docs/organizing-code.md  — Why orderBook_ is private; exposure via getters.
 *
 * PROJECT LAYOUT: Source in src/. Build from repo root: .\run.ps1 or scripts/build-MerkelMain.ps1 or
 *   g++ -std=c++17 -Isrc -o build/MerkelMain.exe src/MerkelMain.cpp src/OrderBookEntry.cpp src/OrderBook.cpp src/CSVReader.cpp
 *
 * EMBEDDING INIT: init() calls orderBook_.load(orderBookPath_) so the order book is loaded once.
 *
 * LIMITING EXPOSURE: orderBook_ is private. printMarketStats() uses orderBook_.getAllEntries(),
 * getAllEntriesAtTime(), and OrderBookEntry helpers (computeAveragePrice, computePriceChange, etc.).
 *
 * FLOW: main() → MerkelMain() → init() once → run() (menu loop until user picks Continue).
 */

#include "MerkelMain.h"
#include <iostream>
#include <string>
#include <limits>
#include <sstream>

/** Log helper: section(title), info/warn/error(msg), kv(key, value). All output to stdout for terminal visibility. */
namespace Log {
    void section(const std::string& title) {
        std::cout << "\n===== " << title << " =====\n" << std::endl;
    }
    void info(const std::string& msg) { std::cout << "[info] " << msg << std::endl; }
    void warn(const std::string& msg) { std::cout << "[warn] " << msg << std::endl; }
    void error(const std::string& msg) { std::cout << "[error] " << msg << std::endl; }
    template<typename T>
    void kv(const std::string& key, const T& value) {
        std::ostringstream os;
        os << key << "=" << value;
        std::cout << "[kv] " << os.str() << std::endl;
    }
}

// -------- Constructor --------
MerkelMain::MerkelMain() {}

// -------- init(): one-time setup; load order book --------
void MerkelMain::init() {
    Log::section("STARTUP");
    orderBookPath_ = "data/order_book_example.csv";
    orderBook_.load(orderBookPath_);
    size_t count = orderBook_.getAllEntries().size();
    if (count > 0) {
        currentTimestamp_ = orderBook_.getEarliestTime();
        Log::info("Order book loaded.");
        Log::kv("orders", count);
        Log::kv("path", orderBookPath_);
        Log::kv("currentTime", currentTimestamp_);
    } else {
        currentTimestamp_.clear();
        Log::warn("No order book loaded; stats will show placeholder.");
        Log::kv("path", orderBookPath_);
    }
}

// -------- run(): main menu loop --------
void MerkelMain::run() {
    Log::info("Exchange menu ready.");
    while (true) {
        printMenu();
        int userOption = getUserOption();
        validateUserOption(userOption);
        MenuOption choice = static_cast<MenuOption>(userOption);
        handleUserOption(choice);
        if (choice == MenuOption::Continue) {
            std::cout << "Goodbye." << std::endl;
            break;
        }
    }
}

// -------- printMenu(): private helper --------
void MerkelMain::printMenu() {
    const char SEP[] = "================================================";
    std::cout << SEP << std::endl;
    std::cout << "Welcome to the exchange" << std::endl;
    std::cout << "1. Print help" << std::endl;
    std::cout << "2. Print exchange stats" << std::endl;
    std::cout << "3. Enter ask" << std::endl;
    std::cout << "4. Enter bid" << std::endl;
    std::cout << "5. Print wallet" << std::endl;
    std::cout << "6. Continue (next time step)" << std::endl;
    std::cout << SEP << std::endl;
}

// -------- getUserOption(): read 1–6 from user --------
int MerkelMain::getUserOption() {
    const char SEP[] = "================================================";
    std::cout << "Enter your choice: 1-6: " << std::endl;
    std::cout << SEP << std::endl;
    int userOption = 0;
    std::cin >> userOption;
    if (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        userOption = 0;
    }
    Log::kv("choice", userOption);
    return userOption;
}

// -------- validateUserOption(): re-prompt until 1–6 --------
void MerkelMain::validateUserOption(int& userOption) {
    while (userOption < static_cast<int>(MenuOption::Help) || userOption > static_cast<int>(MenuOption::Continue)) {
        std::cout << "Invalid choice. Choice 1-6 only." << std::endl;
        std::cout << "Enter your choice: 1-6: ";
        std::cin >> userOption;
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            userOption = 0;
        }
    }
}

// -------- readAmountAndPrice(): shared by enterAsk and enterBid --------
void MerkelMain::readAmountAndPrice(int& amount, int& price) {
    std::cout << "Enter the amount: ";
    std::cin >> amount;
    if (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        amount = 0;
    }
    std::cout << "Enter the price: ";
    std::cin >> price;
    if (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        price = 0;
    }
}

// -------- handleUserOption(): dispatch to action --------
void MerkelMain::handleUserOption(MenuOption choice) {
    switch (choice) {
        case MenuOption::Help:
            printHelp();
            break;
        case MenuOption::Stats:
            printMarketStats();
            break;
        case MenuOption::Ask:
            enterAsk();
            break;
        case MenuOption::Bid:
            enterBid();
            break;
        case MenuOption::Wallet:
            printWallet();
            break;
        case MenuOption::Continue:
            continueToNextTimeStep();
            break;
    }
}

// -------- Action methods (one per menu option; see MenuOption in MerkelMain.h) --------
/** Help: short guidance for the user. */
void MerkelMain::printHelp() {
    std::cout << "Help = your aim is to make $$. Analyze..." << std::endl;
}

/** Stats: current-time window (mean, low, high, spread, change vs prev, best bid/ask). See docs/orderbook-statistics.md, docs/trading-market-basics.md. */
void MerkelMain::printMarketStats() {
    std::vector<OrderBookEntry> all = orderBook_.getAllEntries();
    if (all.empty()) {
        std::cout << "Market looks good. Sell high, buy low. (No order book loaded.)" << std::endl;
        return;
    }
    std::vector<OrderBookEntry> atCurrent = orderBook_.getAllEntriesAtTime(currentTimestamp_);
    std::cout << "Order book (total " << all.size() << " entries, " << orderBook_.getKnownProducts().size() << " products)" << std::endl;
    std::cout << "  Current time:  " << currentTimestamp_ << std::endl;
    std::cout << "  Orders at current time: " << atCurrent.size() << std::endl;
    if (!atCurrent.empty()) {
        std::cout << "  --- Stats for current time window ---" << std::endl;
        std::cout << "  Mean price:    " << Format::price(computeAveragePrice(atCurrent)) << std::endl;
        std::cout << "  Low price:     " << Format::price(computeLowPrice(atCurrent)) << std::endl;
        std::cout << "  High price:    " << Format::price(computeHighPrice(atCurrent)) << std::endl;
        std::cout << "  Price spread:  " << Format::price(computePriceSpread(atCurrent)) << std::endl;
        std::string prevTime = orderBook_.getPreviousTime(currentTimestamp_);
        if (!prevTime.empty()) {
            std::vector<OrderBookEntry> atPrevious = orderBook_.getAllEntriesAtTime(prevTime);
            if (!atPrevious.empty()) {
                double change = computePriceChange(atCurrent, atPrevious);
                double pct = computePercentChange(atCurrent, atPrevious);
                std::cout << "  Change vs prev: " << Format::price(change) << " (" << Format::price(pct) << "%)" << std::endl;
            }
        } else {
            std::cout << "  Change vs prev: (no previous time)" << std::endl;
        }
        std::vector<std::string> products = orderBook_.getKnownProducts();
        if (!products.empty()) {
            const std::string& p = products[0];
            double bid = orderBook_.getBestBid(p, currentTimestamp_);
            double ask = orderBook_.getBestAsk(p, currentTimestamp_);
            std::cout << "  Best bid (" << p << "): " << Format::price(bid) << std::endl;
            std::cout << "  Best ask (" << p << "): " << Format::price(ask) << std::endl;
        }
    }
}

/** Enter ask: prompt amount/price, log result. (Order not yet sent to book; placeholder for matching.) */
void MerkelMain::enterAsk() {
    std::cout << "Enter ask" << std::endl;
    int amount = 0, price = 0;
    readAmountAndPrice(amount, price);
    std::cout << "Ask entered: " << amount << " @ " << price << std::endl;
}

/** Enter bid: prompt amount/price, log result. (Order not yet sent to book; placeholder for matching.) */
void MerkelMain::enterBid() {
    std::cout << "Enter bid" << std::endl;
    int amount = 0, price = 0;
    readAmountAndPrice(amount, price);
    std::cout << "Bid entered: " << amount << " @ " << price << std::endl;
}


/** Wallet: display balances. Currently placeholder; no wallet state yet. */
void MerkelMain::printWallet() {
    std::cout << "Wallet: no balance yet." << std::endl;
}

void MerkelMain::continueToNextTimeStep() {
    std::string next = orderBook_.getNextTime(currentTimestamp_);
    if (next.empty()) {
        std::cout << "End of order book (no next time step)." << std::endl;
    } else {
        currentTimestamp_ = next;
        std::cout << "Now at time: " << currentTimestamp_ << std::endl;
    }
}

// -------- Entry point (see docs/merkel-main.md for flow) --------
int main() {
    MerkelMain app;
    app.init();
    app.run();
    return 0;
}
