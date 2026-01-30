/*
 * MerkelMain.h — declarations for the Merkel exchange application.
 *
 * PURPOSE: Header for the main exchange loop. Declares MenuOption (enum) and
 * MerkelMain (class with init, run, and menu/action methods). Definitions live in MerkelMain.cpp.
 *
 * PROJECT LAYOUT: Source in src/. Include as "MerkelMain.h" when building with -Isrc.
 *
 * LIMITING EXPOSURE: Order book data lives in class scope (private orders_). Only MerkelMain
 * member functions (init, printMarketStats) can see it; main() cannot do app.orders_. init()
 * embeds the load: it calls CSVReader::readCSV(path, orders_) so data is loaded once and persists
 * for the app. See docs/organizing-code.md and docs/oop-concepts.md.
 *
 * INCLUDE GUARDS: #ifndef MERKELMAIN_H / #define / #endif prevent this header from being
 * pasted twice. See docs/headers-and-cpp.md.
 */

#ifndef MERKELMAIN_H
#define MERKELMAIN_H

#include <iostream>
#include <string>
#include <vector>
#include "OrderBookEntry.h"

// -------- Menu options: enum class --------
enum class MenuOption {
    Help     = 1,
    Stats    = 2,
    Offer    = 3,
    Bid      = 4,
    Wallet   = 5,
    Continue = 6
};

// -------- MerkelMain: exchange application --------
class MerkelMain {
public:
    MerkelMain();

    /** One-time setup (e.g. load config, order book). Called once before run(). */
    void init();

    /** Main loop: print menu, get option, validate, handle, exit on Continue. */
    void run();

    // -------- Menu actions (one per option) --------
    void printHelp();
    void printMarketStats();
    void makeOffer();
    void makeBid();
    void printWallet();
    void continueToNextTimeStep();

    /** Dispatch: call the action for the given menu choice. */
    void handleUserOption(MenuOption choice);

    /** Read user choice 1–6; returns int. Caller may then cast to MenuOption. */
    int getUserOption();

    /** Re-prompt until choice is 1–6. Pass by reference so we can update the value. */
    void validateUserOption(int& userOption);

    /** Read amount and price from stdin (shared by offer and bid). */
    void readAmountAndPrice(int& amount, int& price);

private:
    void printMenu();

    std::string orderBookPath_;
    int ordersLoaded_{0};
    std::vector<OrderBookEntry> orders_;
};

#endif /* MERKELMAIN_H */
