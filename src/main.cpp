/*
 * Exchange menu — simple C++ console app (single main(), one pass, no loop).
 *
 * PROJECT LAYOUT: Source lives in src/. Build from repo root with -Isrc.
 * Run: .\run.ps1 or .\scripts\build-main.ps1 (Windows); ./run.sh (macOS/Linux).
 * See docs/project-layout.md and docs/SETUP.md.
 *
 * DESIGN: This file is the "single main()" version. All logic lives in main() so you
 * can read one place and see the full flow (menu → input → validate → act).
 *
 * OTHER ENTRY POINTS (same repo):
 *   - refactorMain.cpp — Same menu with a loop; logic split into functions (printMenu,
 *     getUserOption, validateUserOption, handleUserOption) and enum class MenuOption.
 *     Teaching context: docs/DESIGN.md, docs/cpp-basics.md.
 *   - MerkelMain (MerkelMain.cpp) — Class-based app: init() loads CSV via CSVReader::readCSV,
 *     run() is the menu loop. Private orders_ (limiting exposure). Build: scripts/build-MerkelMain.ps1.
 *     See docs/merkel-main.md, docs/organizing-code.md.
 *   - OrderBookEntry demo (OrderBookEntry.cpp + CSVReader.cpp) — Loads CSV, prints entries and stats.
 *     Build: scripts/build-OrderBookEntry.ps1. CSV loading: CSVReader (docs/tokenizer.md).
 *
 * WHY THESE INCLUDES?
 *   <iostream> — standard input/output (cout, cin). Part of the C++ standard library.
 *   <string>   — for std::string (handy for text).
 *
 * WHY std::endl INSTEAD OF \n?
 *   std::endl flushes the output buffer so text appears immediately. \n is just a newline.
 *
 * WHY std:: IN FRONT OF EVERYTHING?
 *   cout, cin, endl live in the "std" namespace. std:: avoids name clashes.
 */

#include <iostream>
#include <string>

// Every C++ program has one main(). Execution starts here.

int main() {

    // -------- Show the menu (options 1–6) --------
    std::cout << "Welcome to the exchange" << std::endl;
    std::cout << "1. Print help" << std::endl;
    std::cout << "2. Print exchange stats" << std::endl;
    std::cout << "3. Make an offer" << std::endl;
    std::cout << "4. Make a bid" << std::endl;
    std::cout << "5. Print wallet" << std::endl;
    std::cout << "6. Continue (next action)" << std::endl;

    std::cout << "================================================" << std::endl;

    // -------- Get the user's choice --------
    std::cout << "Enter your choice: 1-6: " << std::endl;
    std::cout << "================================================" << std::endl;
    int userOption;
    std::cin >> userOption;
    std::cout << "You chose: " << userOption << std::endl;

    // -------- Validate: only 1–6 allowed --------
    while (userOption < 1 || userOption > 6) {
        std::cout << "Invalid choice. Choice 1-6 only." << std::endl;
        std::cout << "Enter your choice: 1-6: ";
        std::cin >> userOption;
    }

    // -------- Do the action for their choice --------
    if (userOption == 1) {
        std::cout << "Help = your aim is to make $$. Analyze..." << std::endl;
    }
    else if (userOption == 2) {
        std::cout << "Market looks good. Sell high, buy low." << std::endl;
    }
    else if (userOption == 3) {
        std::cout << "Making an offer" << std::endl;
        std::cout << "Enter the amount: ";
        int amount;
        std::cin >> amount;
        std::cout << "Enter the price: ";
        int price;
        std::cin >> price;
        std::cout << "Offer made for " << amount << " at " << price << std::endl;
    }
    else if (userOption == 4) {
        std::cout << "Making a bid" << std::endl;
        std::cout << "Enter the amount: ";
        int amount;
        std::cin >> amount;
        std::cout << "Enter the price: ";
        int price;
        std::cin >> price;
        std::cout << "Bid made for " << amount << " at " << price << std::endl;
    }
    else if (userOption == 5) {
        std::cout << "Wallet: (placeholder - no balance yet)" << std::endl;
    }
    else if (userOption == 6) {
        std::cout << "Continuing to next time step..." << std::endl;
    }

    return 0;
}
