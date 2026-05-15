#include "Parser.h"
#include <sstream>
#include <iostream>

Parser::Parser() : lastTimestamp(0), lastSequenceId(-1), priceScale(1.0) {
    symbolLookup["WTI_CRD"] = "CL";
    symbolLookup["BRNT"] = "B";
    symbolLookup["GLD_FUT"] = "GC";
}

void Parser::setPriceScaling(double scale) {
    priceScale = scale;
}

bool Parser::parseLine(const std::string& line, Quote& outQuote) {
    if (line.empty()) return false;

    std::stringstream ss(line);
    std::string token;
    std::vector<std::string> tokens;

    while (std::getline(ss, token, ',')) {
        tokens.push_back(token);
    }

    if (tokens.size() != 7) return false;

    try {
        long long seqId = std::stoll(tokens[0]);
        if (seqId <= lastSequenceId) {
            return false; // Duplicate or out of order sequence ID
        }
        lastSequenceId = seqId;
        outQuote.sequenceId = seqId;

        long long ts = std::stoll(tokens[1]);
        if (ts < lastTimestamp) {
            return false; // Stale timestamp
        }
        lastTimestamp = ts;
        outQuote.timestamp = ts;

        std::string rawSymbol = tokens[2];
        if (symbolLookup.find(rawSymbol) != symbolLookup.end()) {
            outQuote.symbol = symbolLookup[rawSymbol];
        } else {
            outQuote.symbol = rawSymbol; // Fallback
        }

        outQuote.bidPrice = std::stod(tokens[3]) * priceScale;
        outQuote.bidSize = std::stoi(tokens[4]);
        outQuote.askPrice = std::stod(tokens[5]) * priceScale;
        outQuote.askSize = std::stoi(tokens[6]);
        
        return true;
    } catch (const std::exception& e) {
        return false; // Malformed row
    }
}
