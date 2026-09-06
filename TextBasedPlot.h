#pragma once

#include <vector>
#include <string>
#include "CandlestickData.h"
#include <iostream>
#include <iomanip>
#include <limits>
#include <cmath>

class Textbasedplotter {
public:
    static std::string displayCandlestickData(const std::vector<CandlestickData>& data);
    double getMaxValue(const std::vector<CandlestickData>& candlesticks);
    double getMinValue(const std::vector<CandlestickData>& candlesticks);
    void displayCandlestickChart(const std::vector<CandlestickData>& candlesticks);
};