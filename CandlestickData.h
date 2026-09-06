#pragma once
#include <string>

class CandlestickData {
public:
    CandlestickData(const std::string& date, double open, double high, double low, double close);

    std::string timeframe;
    double open;
    double high;
    double low;
    double close;
};