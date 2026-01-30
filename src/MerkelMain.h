/*
 * MerkelMain.h — declarations for the Merkel exchange application.
 *
 * PURPOSE: Header for the main exchange loop. Declares MenuOption (enum) and
 * MerkelMain (class with init, run, and menu/action methods). Definitions live in MerkelMain.cpp.
 *
 * DOCS (embedded references):
 *   docs/merkel-main.md     — Class overview, init/run, time-stepping.
 *   docs/orderbook-time.md  — currentTimestamp_, getNextTime/getPreviousTime.
 *   docs/organizing-code.md — Why orderBook_ is private.
 *   docs/headers-and-cpp.md — Include guards, .h vs .cpp.
 *
 * PROJECT LAYOUT: Source in src/. Include as "MerkelMain.h" when building with -Isrc.
 *
 * INCLUDE GUARDS: #ifndef MERKELMAIN_H / #define / #endif prevent double-include. See docs/headers-and-cpp.md.
 */

#ifndef MERKELMAIN_H
#define MERKELMAIN_H

#include <iostream>
#include <string>
#include <vector>
#include "OrderBook.h"
#include "OrderBookEntry.h"

/** Menu options (1–6). Cast getUserOption() result to MenuOption for handleUserOption(). See docs/merkel-main.md. */
enum class MenuOption {
    Help     = 1,  /** Print help text */
    Stats    = 2,  /** Print exchange stats (order book, current time, mean/spread/change, best bid/ask) */
    Ask      = 3,  /** Enter an ask (sell order) */
    Bid      = 4,  /** Enter a bid (buy order) */
    Wallet   = 5,  /** Print wallet (placeholder) */
    Continue = 6   /** Advance to next time step; exit loop when chosen */
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
    void enterAsk();
    void enterBid();
    void printWallet();
    void continueToNextTimeStep();

    /** Dispatch: call the action for the given menu choice. */
    void handleUserOption(MenuOption choice);

    /** Read user choice 1–6; returns int. Caller may then cast to MenuOption. */
    int getUserOption();

    /** Re-prompt until choice is 1–6. Pass by reference so we can update the value. */
    void validateUserOption(int& userOption);

    /** Read amount and price from stdin (shared by enterAsk and enterBid). */
    void readAmountAndPrice(int& amount, int& price);

private:
    void printMenu();

    std::string orderBookPath_;
    OrderBook orderBook_;
    /** Current time step (earliest after init; advances on Continue). */
    std::string currentTimestamp_;
};

#endif /* MERKELMAIN_H */
