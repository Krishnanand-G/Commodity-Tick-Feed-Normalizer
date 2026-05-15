#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include "Quote.h"

class Parser {
public:
    Parser();
    bool parseLine(const std::string& line, Quote& outQuote);
    void setPriceScaling(double scale);

private:
    std::unordered_map<std::string, std::string> symbolLookup;
    long long lastTimestamp;
    long long lastSequenceId;
    double priceScale;
};
