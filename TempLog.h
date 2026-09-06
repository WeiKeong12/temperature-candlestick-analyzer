#pragma once

#include <string>

class TempLog {
public:
    TempLog(const std::string& timestamp, const std::string& country, double temperature);

    std::string timestamp;
    std::string country;
    double temperature;
};