/*
 * MerkelMain.cpp — definitions for the Merkel exchange application.
 *
 * PURPOSE: Implements everything declared in MerkelMain.h. One translation unit holds
 * MerkelMain constructor, init(), run(), menu/action methods, and main().
 *
 * PROJECT LAYOUT: Source in src/. Build from repo root: scripts/build-MerkelMain.ps1 or
 * g++ -std=c++17 -Isrc -o MerkelMain.exe src/MerkelMain.cpp src/OrderBookEntry.cpp src/CSVReader.cpp
 *
 * EMBEDDING INIT: init() calls CSVReader::readCSV(orderBookPath_, orders_) so the order book is
 * loaded once and stored in private orders_. No global; limiting exposure.
 *
 * LIMITING EXPOSURE: orders_ is private (in MerkelMain.h). Only member functions see it.
 * CSVReader::readCSV(path, orders_) fills our vector; printMarketStats() uses orders_. See docs/organizing-code.md.
 *
 * FLOW: main() creates MerkelMain, calls init() once, then run() (menu loop until user picks Continue).
 */

#include "MerkelMain.h"
#include "CSVReader.h"
#include <iostream>
#include <string>
#include <limits>

// -------- Constructor --------
MerkelMain::MerkelMain() {
    std::cout << "MerkelMain constructor" << std::endl;
}

// -------- init(): one-time setup; embed CSV load into class-owned orders_ --------
void MerkelMain::init() {
    std::cout << "MerkelMain init" << std::endl;
    orderBookPath_ = "data/order_book_example.csv";
    ordersLoaded_ = CSVReader::readCSV(orderBookPath_, orders_);  /* load into private orders_; limits exposure */
    if (ordersLoaded_ > 0) {
        std::cout << "Loaded " << ordersLoaded_ << " orders from " << orderBookPath_ << std::endl;
    } else {
        std::cout << "No order book loaded (path: " << orderBookPath_ << "). Stats will show placeholder." << std::endl;
    }
}

// -------- run(): main menu loop --------
void MerkelMain::run() {
    std::cout << "MerkelMain run" << std::endl;
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
    std::cout << "3. Make an offer" << std::endl;
    std::cout << "4. Make a bid" << std::endl;
    std::cout << "5. Print wallet" << std::endl;
    std::cout << "6. Continue (next action)" << std::endl;
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
    std::cout << "You chose: " << userOption << std::endl;
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

// -------- readAmountAndPrice(): shared by offer and bid --------
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
        case MenuOption::Offer:
            makeOffer();
            break;
        case MenuOption::Bid:
            makeBid();
            break;
        case MenuOption::Wallet:
            printWallet();
            break;
        case MenuOption::Continue:
            continueToNextTimeStep();
            break;
    }
}

// -------- Action methods (one per menu option) --------
void MerkelMain::printHelp() {
    std::cout << "Help = your aim is to make $$. Analyze..." << std::endl;
}

void MerkelMain::printMarketStats() {
    if (ordersLoaded_ > 0 && !orders_.empty()) {
        std::cout << "Order book stats (" << ordersLoaded_ << " entries):" << std::endl;
        std::cout << "  Average price: " << Format::price(computeAveragePrice(orders_)) << std::endl;
        std::cout << "  Low price:     " << Format::price(computeLowPrice(orders_)) << std::endl;
        std::cout << "  High price:    " << Format::price(computeHighPrice(orders_)) << std::endl;
        std::cout << "  Price spread:  " << Format::price(computePriceSpread(orders_)) << std::endl;
    } else {
        std::cout << "Market looks good. Sell high, buy low. (No order book loaded.)" << std::endl;
    }
}

void MerkelMain::makeOffer() {
    std::cout << "Making an offer" << std::endl;
    int amount = 0, price = 0;
    readAmountAndPrice(amount, price);
    std::cout << "Offer made for " << amount << " at " << price << std::endl;
}

void MerkelMain::makeBid() {
    std::cout << "Making a bid" << std::endl;
    int amount = 0, price = 0;
    readAmountAndPrice(amount, price);
    std::cout << "Bid made for " << amount << " at " << price << std::endl;
}

void MerkelMain::printWallet() {
    std::cout << "Wallet: (placeholder - no balance yet)" << std::endl;
}

void MerkelMain::continueToNextTimeStep() {
    std::cout << "Continuing to next time step..." << std::endl;
}

// -------- Entry point --------
int main() {
    MerkelMain app;
    app.init();
    app.run();
    return 0;
}
