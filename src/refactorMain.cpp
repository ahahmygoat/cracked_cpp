/*
 * Exchange menu — refactored: functions, enum class, Order struct.
 *
 * FOR NOOBS: Read this whole block first. It explains what this file does,
 * the main design choices, and why we use certain C++ features. Then read
 * the DESIGN / WHY comments above each section as you scroll down.
 *
 * PROJECT LAYOUT: Source in src/. Build from repo root with -Isrc. See docs/project-layout.md.
 *
 * DESIGN: This file is the "refactored" version. Same behavior as main.cpp, but:
 * - Logic is split into functions (printMenu, getUserOption, validateUserOption, handleUserOption)
 *   so main() just orchestrates and each function has one job.
 * - Menu options are an enum class (MenuOption) so we use a type instead of magic numbers.
 * - We handle cin.fail() so non-numeric input doesn't leave garbage and break the loop.
 * - Order struct groups one order book row (price, amount, timestamp, product, orderType);
 *   we use public data for simplicity; later you can add private data and constructors.
 *
 * TEACHING CONTEXT IN DOCS: Same ideas (single responsibility, enum, validation) are in
 * DESIGN.md, cpp-basics.md. For class-based app with init/run and private state (limiting
 * exposure), see MerkelMain and docs/merkel-main.md, docs/organizing-code.md.
 *
 * See main.cpp for the "single main()" version; MerkelMain for the class-based version.
 */

#include <iostream>
#include <string>
#include <limits>   // needed for std::numeric_limits (used when we clear bad input)

// -------- Menu options: enum class --------
enum class MenuOption {
    Help     = 1,
    Stats    = 2,
    Offer    = 3,
    Bid      = 4,
    Wallet   = 5,
    Continue = 6
};

const char SEP[] = "================================================";

void printMenu() {
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

int getUserOption() {
    std::cout << "Enter your choice: 1-6: " << std::endl;
    std::cout << SEP << std::endl;
    int userOption;
    std::cin >> userOption;
    if (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        userOption = 0;
    }
    std::cout << "You chose: " << userOption << std::endl;
    return userOption;
}

void validateUserOption(int& userOption) {
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

void readAmountAndPrice(int& amount, int& price) {
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

void printHelp() { std::cout << "Help = your aim is to make $$. Analyze..." << std::endl; }
void printMarketStats() { std::cout << "Market looks good. Sell high, buy low." << std::endl; }

void makeOffer() {
    std::cout << "Making an offer" << std::endl;
    int amount, price;
    readAmountAndPrice(amount, price);
    std::cout << "Offer made for " << amount << " at " << price << std::endl;
}

void makeBid() {
    std::cout << "Making a bid" << std::endl;
    int amount, price;
    readAmountAndPrice(amount, price);
    std::cout << "Bid made for " << amount << " at " << price << std::endl;
}

void printWallet() { std::cout << "Wallet: (placeholder - no balance yet)" << std::endl; }
void continueToNextTimeStep() { std::cout << "Continuing to next time step..." << std::endl; }

void handleUserOption(MenuOption choice) {
    switch (choice) {
        case MenuOption::Help:     printHelp(); break;
        case MenuOption::Stats:    printMarketStats(); break;
        case MenuOption::Offer:    makeOffer(); break;
        case MenuOption::Bid:     makeBid(); break;
        case MenuOption::Wallet:   printWallet(); break;
        case MenuOption::Continue: continueToNextTimeStep(); break;
    }
}

struct Order {
    double price{0.0};
    double amount{0.0};
    std::string timestamp{"2020/03/17 17:01:24.884492"};
    std::string product{"ETH/BTC"};
    std::string orderType{"bid"};
    void print() const {
        std::cout << "Order: " << amount << " " << product << " at " << price << " on " << timestamp << std::endl;
    }
};

int main() {
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
    return 0;
}
