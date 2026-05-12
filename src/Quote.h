#pragma once
#include <string>

struct Quote {
    std::string symbol;
    long long timestamp;
    double bidPrice;
    int bidSize;
    double askPrice;
    int askSize;
    long long sequenceId;
};
