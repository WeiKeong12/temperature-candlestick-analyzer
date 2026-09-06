#include "TempD.h"
#include "fileReader.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <algorithm>
#include <string>

// Define CandlestickData class
class CandlestickData {
public:
    std::string timeframe;
    double open, high, low, close;

    CandlestickData(const std::string& timeframe, double open, double high, double low, double close)
        : timeframe(timeframe), open(open), high(high), low(low), close(close) {}
};

// Define TempD class
class TempD {
private:
    std::string filePath;
    std::string country;

public:
    // Constructor to initialize TempD with a file path
    TempD(const std::string& filename) : filePath(filename) {}

    // Set the country for which we want to analyze the Temp data
    void setCountry(const std::string& country) { this->country = country; }

    // Get the list of known countries from the CSV file
    std::vector<std::string> getKnownCountries() const {
        std::ifstream file(this->filePath); // Open the file
        std::string line;
        std::getline(file, line); // Read the header line
        std::istringstream lineStream(line); // Stream the header line
        std::vector<std::string> headers;
        std::string header;
        while (std::getline(lineStream, header, ',')) {
            headers.push_back(header); // Store each header (column name)
        }
        std::vector<std::string> countries;
        countries.reserve(headers.size() - 1); // Reserve space for country names
        for (size_t i = 1; i < headers.size(); i++) {
            // Extract country names from headers (column names)
            std::string country = headers[i].substr(0, headers[i].find("_temperature"));
            countries.push_back(country);
        }
        return countries;
    }

    // Compute the Average Temperature for a Given Vector of Temperatures
    double computeAverage(const std::vector<double>& temperatures) const {
        double sum = 0.0;
        for (double temperature : temperatures) {
            sum += temperature;
        }
        return sum / temperatures.size();
    }

    // Compute candlestick data for the specified time frame
    std::vector<CandlestickData> computeCandlestickData(const std::string& timeFrame) const {
        std::vector<CandlestickData> candlesticks;
        std::ifstream file(this->filePath); // Open the file
        std::string line;
        // Read the header line
        std::getline(file, line);
        std::istringstream headerStream(line);
        std::vector<std::string> headers;
        std::string header;
        while (std::getline(headerStream, header, ',')) {
            headers.push_back(header); // Store each header (column name)
        }

        // Find the index of the country column
        auto it = std::find(headers.begin(), headers.end(), country + "_temperature");
        if (it == headers.end()) {
            std::cerr << "Country not found in data" << std::endl;
            return candlesticks;
        }
        size_t countryIndex = std::distance(headers.begin(), it);
        std::map<std::string, std::vector<double>> data;
        while (std::getline(file, line)) {
            std::istringstream lineStream(line);
            std::string field;
            std::getline(lineStream, field, ','); // Read the timestamp
            std::string key = field.substr(0, 4); // Use the Year as the key for yearly data
            if (timeFrame == "monthly") {
                key = field.substr(0, 7); // Use the Year-Month as the key for monthly data
            } else if (timeFrame == "daily") {
                key = field.substr(0, 10); // Use the Year-Month-Day as the key for daily data
            }
            // Skip to the required country's temperature field
            for (size_t i = 0; i <= countryIndex; ++i) {
                std::getline(lineStream, field, ',');
            }
            double temperature = std::stod(field); // Convert the field to a double
            data[key].push_back(temperature); // Add the temperature to the map
        }
        std::string previousKey;
        double previousClose = 0.0;
        for (const auto& [key, temperatures] : data) {
            // Calculate the open, high, low, and close values for the candlestick
            double open = previousKey.empty() ? computeAverage(temperatures) : previousClose;
            double close = computeAverage(temperatures);
            double high = *std::max_element(temperatures.begin(), temperatures.end());
            double low = *std::min_element(temperatures.begin(), temperatures.end());
            CandlestickData candlestick(key, open, high, low, close);
            candlesticks.push_back(candlestick); // Add the candlestick data to the vector
            previousClose = close;
            previousKey = key;
        }
        return candlesticks;
    }

    // Filter candlestick data by date range
    std::vector<CandlestickData> filterDataByDateRange(const std::vector<CandlestickData>& candlesticks, const std::string& start, const std::string& end) const {
        std::vector<CandlestickData> filtered;
        filtered.reserve(candlesticks.size()); // Reserve space for the filtered data
        auto startIter = std::lower_bound(candlesticks.begin(), candlesticks.end(), start, [](const CandlestickData& candle, const std::string& date) {
            return candle.timeframe < date;
        });

        auto endIter = std::upper_bound(candlesticks.begin(), candlesticks.end(), end, [](const std::string& date, const CandlestickData& candle) {
            return date < candle.timeframe;
        });

        filtered.assign(startIter, endIter);
        return filtered;
    }

    // Method to filter candlestick data by temperature range
    std::vector<CandlestickData> filterDataByTemperatureRange(const std::vector<CandlestickData>& candlesticks, double minTemp, double maxTemp) const {
        std::vector<CandlestickData> filtered;
        for (const auto& candle : candlesticks) {
            // Add candlestick data to the filtered list if within the specified temperature range
            if (candle.low >= minTemp && candle.high <= maxTemp) {
                filtered.push_back(candle);
            }
        }
        return filtered;
    }

    // Compute the slope and intercept for predicting temperature using linear regression
    std::pair<double, double> computeLinearRegression(const std::vector<int>& years, const std::vector<double>& values) const {
        int n = years.size();
        double sumX = 0.0;
        double sumY = 0.0;
        double sumX2 = 0.0;
        double sumXY = 0.0;

        for (int i = 0; i < n; ++i) {
            sumX += years[i];
            sumY += values[i];
            sumX2 += years[i] * years[i];
            sumXY += years[i] * values[i];
        }

        double denominator = n * sumX2 - sumX * sumX;
        double slope = 0.0;
        double intercept = 0.0;

        if (denominator != 0.0) {
            slope = (n * sumXY - sumX * sumY) / denominator;
            intercept = (sumY - slope * sumX) / n;
        }

        return {slope, intercept};
    }

    // Predict future values using linear regression parameters
    std::vector<double> predictValues(const std::pair<double, double>& parameters, const std::vector<int>& futureYears) const {
        std::vector<double> predictions;
        predictions.reserve(futureYears.size()); // Reserve space for the predictions

        for (int year : futureYears) {
            // Use the linear regression parameters to predict values
            predictions.push_back(parameters.first * year + parameters.second);
        }

        return predictions;
    }

    // Prepare data for prediction by extracting years and corresponding values from the candlestick data
    std::pair<std::vector<int>, std::vector<double>> prepareDataForPrediction(const std::vector<CandlestickData>& candlesticks, const std::string& valueType) const {
        std::vector<int> years;
        std::vector<double> values;
        years.reserve(candlesticks.size()); // Reserve space for the years vector
        values.reserve(candlesticks.size()); // Reserve space for the values vector

        for (const auto& candle : candlesticks) {
            years.push_back(std::stoi(candle.timeframe)); // Extract the year from the timeframe

            double value;
            if (valueType == "open") {
                value = candle.open; // Extract the open values
            } else if (valueType == "close") {
                value = candle.close; // Extract the close values
            } else if (valueType == "high") {
                value = candle.high; // Extract the high values
            } else if (valueType == "low") {
                value = candle.low; // Extract the low values
            }
            values.push_back(value);
        }

        return {years, values};
    }

    // Predict candlestick data based on historical data and future years
    std::vector<CandlestickData> predictCandlestickData(const std::vector<CandlestickData>& historicalData, const std::vector<int>& futureYears) const {
        // Prepare data for each type of value (open, close, high, low) outside the loop
        auto [yearsOpen, valuesOpen] = prepareDataForPrediction(historicalData, "open");
        auto [yearsClose, valuesClose] = prepareDataForPrediction(historicalData, "close");
        auto [yearsHigh, valuesHigh] = prepareDataForPrediction(historicalData, "high");
        auto [yearsLow, valuesLow] = prepareDataForPrediction(historicalData, "low");

        // Compute linear regression parameters for each type of value outside the loop
        auto paramsOpen = computeLinearRegression(yearsOpen, valuesOpen);
        auto paramsClose = computeLinearRegression(yearsClose, valuesClose);
        auto paramsHigh = computeLinearRegression(yearsHigh, valuesHigh);
        auto paramsLow = computeLinearRegression(yearsLow, valuesLow);

        std::vector<CandlestickData> predictedCandlesticks;
        predictedCandlesticks.reserve(futureYears.size()); // Reserve space for the predicted candlesticks

        for (size_t i = 0; i < futureYears.size(); ++i) {
            // Predict future values for each type of value
            double open = paramsOpen.first * futureYears[i] + paramsOpen.second;
            double close = paramsClose.first * futureYears[i] + paramsClose.second;
            double high = paramsHigh.first * futureYears[i] + paramsHigh.second;
            double low = paramsLow.first * futureYears[i] + paramsLow.second;

            // Create candlestick data for each predicted year and add it to the vector
            predictedCandlesticks.emplace_back(std::to_string(futureYears[i]), open, high, low, close);
        }

        return predictedCandlesticks;
    }
};