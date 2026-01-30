/*
 * CSVReader.h — declarations for CSV file reading into OrderBookEntry vectors.
 *
 * PURPOSE: Single place for CSV loading. readCSV tokenizes each line (see docs/tokenizer.md),
 * parses amount/price with try/catch (docs/exception-handling.md), and returns or fills
 * a vector of OrderBookEntry. CSV format: timestamp, product, orderType, amount, price.
 *
 * PROJECT LAYOUT: Source in src/. Include as "CSVReader.h" when building with -Isrc.
 *
 * USE: Include "CSVReader.h" and "OrderBookEntry.h"; call CSVReader::readCSV(path) or
 * readCSV(path, out). Link CSVReader.cpp. OrderBookEntry no longer provides loadOrderBook.
 */

#pragma once

#include "OrderBookEntry.h"
#include <string>
#include <vector>

class CSVReader {
public:
    CSVReader(const std::string& filename);
    CSVReader() = default;

    /** Read CSV from path; returns a new vector of OrderBookEntry. Empty on open failure. */
    static std::vector<OrderBookEntry> readCSV(const std::string& filename);

    /** Read CSV from path into out (clears out first). Returns count loaded; 0 on error. */
    static int readCSV(const std::string& filename, std::vector<OrderBookEntry>& out);

private:
    /** Split line by delimiter. Does not throw for normal input. See docs/tokenizer.md. */
    static std::vector<std::string> tokenize(const std::string& csvLine, char delimiter);

    /** Convert 5 tokens to OrderBookEntry. Throws std::invalid_argument or std::out_of_range (stod).
        Caller (readCSVInto) catches std::exception and skips the line. See docs/exception-handling.md. */
    static OrderBookEntry stringsToOBE(const std::vector<std::string>& tokens);

    /** Open file, read lines, tokenize, parse; catch exceptions per line and skip. Returns count loaded. */
    static int readCSVInto(const std::string& path, std::vector<OrderBookEntry>& out);

    std::string filename_;
};
