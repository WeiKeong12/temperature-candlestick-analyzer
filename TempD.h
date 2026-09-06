#pragma once
#include <string>
#include <vector>
#include "TempLog.h"
#include "CandlestickData.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iostream>
#include <map>
#include <numeric>

class TempD {
public:
    TempD(const std::string& filename);
    void setCountry(const std::string& country);
    std::vector<std::string> getKnownCountries() const;
    double computeAverage(const std::vector<double>& temperatures) const;
    std::vector<CandlestickData> computeCandlestickData(const std::string& timeframe) const;
    std::vector<CandlestickData> filterDataByDateRange(const std::vector<CandlestickData>& candlesticks, const std::string& start, const std::string& end) const;
    std::vector<CandlestickData> filterDataByTemperatureRange(const std::vector<CandlestickData>& candlesticks, double minTemp, double maxTemp) const;
    std::pair<double, double> computeLinearRegression(const std::vector<int>& years, const std::vector<double>& values) const;
    std::vector<double> predictValues(const std::pair<double, double>& parameters, const std::vector<int>& futureYears) const;
    std::pair<std::vector<int>, std::vector<double>> prepareDataForPrediction(const std::vector<CandlestickData>& candlesticks, const std::string& valueType) const;
    std::vector<CandlestickData> predictCandlestickData(const std::vector<CandlestickData>& historicalData, const std::vector<int>& futureYears) const;

private:
    std::vector<TempLog> entries;
    std::string filePath;
    std::string country;
};