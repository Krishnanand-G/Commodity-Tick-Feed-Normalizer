#include "Parser.h"
#include <charconv>

Parser::Parser() : lastTimestamp(0), lastSequenceId(-1), priceScale(1.0) {
    symbolLookup["WTI_CRD"] = "CL";
    symbolLookup["BRNT"] = "B";
    symbolLookup["GLD_FUT"] = "GC";
}

void Parser::setPriceScaling(double scale) {
    priceScale = scale;
}

bool Parser::parseLine(std::string_view line, Quote& outQuote) {
    if (line.empty()) return false;

    std::string_view tokens[7];
    size_t start = 0;
    for (int i = 0; i < 7; ++i) {
        size_t end = line.find(',', start);
        if (i < 6 && end == std::string_view::npos) return false;
        tokens[i] = line.substr(start, end - start);
        start = end + 1;
    }

    auto parseLong = [](std::string_view sv, long long& val) {
        auto [p, ec] = std::from_chars(sv.data(), sv.data() + sv.size(), val);
        return ec == std::errc();
    };
    
    auto parseInt = [](std::string_view sv, int& val) {
        auto [p, ec] = std::from_chars(sv.data(), sv.data() + sv.size(), val);
        return ec == std::errc();
    };

    auto parseDouble = [](std::string_view sv, double& val) {
        // Simple double parsing since std::from_chars for double requires C++17 <charconv> 
        // which might not be fully supported on all compilers. But assuming GCC 11+ / MSVC
        auto [p, ec] = std::from_chars(sv.data(), sv.data() + sv.size(), val);
        return ec == std::errc();
    };

    long long seqId;
    if (!parseLong(tokens[0], seqId)) return false;
    if (seqId <= lastSequenceId) return false;
    
    long long ts;
    if (!parseLong(tokens[1], ts)) return false;
    if (ts < lastTimestamp) return false;

    std::string_view rawSymbol = tokens[2];
    auto it = symbolLookup.find(rawSymbol);
    if (it != symbolLookup.end()) {
        outQuote.symbol = std::string(it->second);
    } else {
        outQuote.symbol = std::string(rawSymbol);
    }

    double bid, ask;
    int bSize, aSize;
    if (!parseDouble(tokens[3], bid) || !parseInt(tokens[4], bSize) ||
        !parseDouble(tokens[5], ask) || !parseInt(tokens[6], aSize)) {
        return false;
    }

    lastSequenceId = seqId;
    outQuote.sequenceId = seqId;
    lastTimestamp = ts;
    outQuote.timestamp = ts;
    outQuote.bidPrice = bid * priceScale;
    outQuote.bidSize = bSize;
    outQuote.askPrice = ask * priceScale;
    outQuote.askSize = aSize;

    return true;
}

