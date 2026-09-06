#include "CandlestickData.h"

CandlestickData::CandlestickData(const std::string& timeframe, double open, double high, double low, double close)
    : timeframe(timeframe), open(open), high(high), low(low), close(close) {}