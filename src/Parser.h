#pragma once
#include <string_view>
#include <vector>
#include <unordered_map>
#include "Quote.h"

class Parser {
public:
    Parser();
    bool parseLine(std::string_view line, Quote& outQuote);
    void setPriceScaling(double scale);

private:
    std::unordered_map<std::string_view, std::string_view> symbolLookup;
    long long lastTimestamp;
    long long lastSequenceId;
    double priceScale;
};
