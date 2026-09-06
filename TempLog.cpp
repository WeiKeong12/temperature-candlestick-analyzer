#include "TempLog.h"

TempLog::TempLog(const std::string& timestamp, const std::string& country, const double temperature)
    : timestamp(timestamp), country(country), temperature(temperature) {}