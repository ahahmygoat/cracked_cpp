/*
 * CSVReader.cpp — definitions for CSV reading (tokenize, stringsToOBE, readCSV).
 *
 * PURPOSE: Implements CSVReader declared in CSVReader.h. Reads CSV lines, tokenizes by comma,
 * parses with try/catch; skips bad lines and logs to stderr.
 *
 * DOCS (embedded references):
 *   docs/tokenizer.md — tokenize(csvLine, ','); getline(ss, token, delimiter).
 *   docs/exception-handling.md — stringsToOBE throws; readCSVInto catches std::exception.
 *
 * CSV columns (file order): timestamp, product, orderType, amount, price.
 * OrderBookEntry ctor: (price, amount, timestamp, product, orderType). See CSVReader.h.
 */

#include "CSVReader.h"
#include <fstream>
#include <sstream>
#include <exception>
#include <stdexcept>

// -------- Constructor --------
CSVReader::CSVReader(const std::string& filename) : filename_(filename) {}

// -------- tokenize: split line by delimiter --------
// Does not throw for normal input (getline + push_back). See docs/tokenizer.md.
std::vector<std::string> CSVReader::tokenize(const std::string& csvLine, char delimiter) {
    std::vector<std::string> tokens;
    std::stringstream ss(csvLine);
    std::string token;
    while (std::getline(ss, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

// -------- stringsToOBE: [timestamp, product, orderType, amount, price] -> OrderBookEntry --------
// Throws: std::invalid_argument if tokens.size() < 5 or stod fails; std::out_of_range if value overflows.
// Caller (readCSVInto) catches std::exception and skips the line. See docs/tokenizer.md, docs/exception-handling.md.
OrderBookEntry CSVReader::stringsToOBE(const std::vector<std::string>& tokens) {
    if (tokens.size() < 5) {
        throw std::invalid_argument("CSV line has fewer than 5 columns");
    }
    const std::string& timestamp = tokens[0];
    const std::string& product = tokens[1];
    const std::string& orderTypeStr = tokens[2];
    double amount = std::stod(tokens[3]);   /* may throw invalid_argument, out_of_range */
    double price = std::stod(tokens[4]);    /* may throw invalid_argument, out_of_range */
    OrderBookType orderType = (orderTypeStr == "bid") ? OrderBookType::bid : OrderBookType::ask;
    return OrderBookEntry(price, amount, timestamp, product, orderType);
}

// -------- readCSVInto: load file into vector (private; see docs/exception-handling.md) --------
// File open: check is_open(), return 0 on failure. Per line: tokenize (no throw), then try stringsToOBE;
// catch std::exception and skip line (log to stderr). See docs/exception-handling.md.
int CSVReader::readCSVInto(const std::string& path, std::vector<OrderBookEntry>& out) {
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Could not open file: " << path << std::endl;
        return 0;
    }
    out.clear();
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        std::vector<std::string> tokens = tokenize(line, ',');
        if (tokens.size() < 5) continue;
        try {
            out.push_back(stringsToOBE(tokens));
        } catch (const std::exception& e) {
            std::cerr << "Skipped line (invalid number): " << e.what() << std::endl;
        }
    }
    return static_cast<int>(out.size());
}

/** Public API: return new vector of OrderBookEntry. Empty on open failure or parse errors. */
std::vector<OrderBookEntry> CSVReader::readCSV(const std::string& filename) {
    std::vector<OrderBookEntry> result;
    readCSVInto(filename, result);
    return result;
}

/** Public API: fill out (cleared first); returns count loaded. 0 on error. */
int CSVReader::readCSV(const std::string& filename, std::vector<OrderBookEntry>& out) {
    return readCSVInto(filename, out);
}
