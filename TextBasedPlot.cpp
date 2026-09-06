#include "TextBasedPlot.h"


//TASK 1
// Function to display the data of each candlestick
std::string Textbasedplotter::displayCandlestickData(const std::vector<CandlestickData>& data) {
    // Iterate through each candlestick data in the vector
    for (const auto& candle : data) {
        // Print the candlestick data: timeframe, open, close, high, and low values
        std::cout << candle.timeframe             << "\n"
                  << "================="          << "\n"
                  << "Open  : " << candle.open    << "\n"
                  << "Close : " << candle.close   << "\n"
                  << "High  : " << candle.high    << "\n"
                  << "Low   : " << candle.low     << "\n" << std::endl;
    }
    return "";
}

//TASK 2
// Function to find the maximum value in the candlestick data
double Textbasedplotter::getMaxValue(const std::vector<CandlestickData>& candlesticks) {
    double maxValue = -std::numeric_limits<double>::infinity(); // Initialize maxValue to negative infinity
    // Iterate through each candlestick data in the vector
    for (const auto& candle : candlesticks) {
        // Update maxValue if the current candlestick’s high value is greater
        if (candle.high > maxValue) {
            maxValue = candle.high;
        }
    }
    return maxValue; // Return the maximum value found
}

// Function to find the minimum value in the candlestick data
double Textbasedplotter::getMinValue(const std::vector<CandlestickData>& candlesticks) {
    double minValue = std::numeric_limits<double>::infinity(); // Initialize minValue to positive infinity
    // Iterate through each candlestick data in the vector
    for (const auto& candle : candlesticks) {
        // Update minValue if the current candlestick’s low value is less
        if (candle.low < minValue) {
            minValue = candle.low;
        }
    }
    return minValue; // Return the minimum value found
}

// Function to get the closest value rounded to the nearest multiple of 1
int roundToNearestMultipleOf1(double value) {
    return static_cast<int>(std::round(value / 1) * 1); // Round the value to the nearest multiple of 1 and convert it to an integer
}

// Function to plot the candlestick data on a text-based grid
void Textbasedplotter::displayCandlestickChart(const std::vector<CandlestickData>& candlesticks) {
    // Define the parameters of the grid
    const int GRID_MIN      = -30; // Minimum value for the grid
    const int INTERVAL      = 1; // Interval between grid lines
    const int gridMin       = GRID_MIN;
    const int gridMax       = 41; // Maximum value for the grid
    const int gridHeight    = (-GRID_MIN + gridMax) / INTERVAL; // Calculate the height of the grid
    const int gridWidth     = candlesticks.size() * 2; // Calculate the width of the grid based on the size of the candlestick data

    // Initialize the grid with spaces
    std::vector<std::vector<char>> grid(gridHeight, std::vector<char>(gridWidth, ' '));

    // Initialize a calculation grid to store numeric values for plotting
    std::vector<std::vector<int>> calculation_grid(gridHeight, std::vector<int>(gridWidth, 0));

    // Populate the calculation grid with numbers from gridMin to gridMax at each interval
    for (int i = 0; i < gridWidth; i += 2) {
        int currentGridMin = gridMin;
        for (int j = 0; j < gridHeight; j++) {
            calculation_grid[j][i] = currentGridMin;
            currentGridMin += INTERVAL;
        }
    }

    // Iterate through the candlestick data to plot the open, close, high, and low values
    for (int i = 0; i < candlesticks.size(); i++) {
        int open    = roundToNearestMultipleOf1(candlesticks[i].open);
        int close   = roundToNearestMultipleOf1(candlesticks[i].close);
        int high    = roundToNearestMultipleOf1(candlesticks[i].high);
        int low     = roundToNearestMultipleOf1(candlesticks[i].low);

        // Initialize indices for plotting
        int openIndex = 0;
        int closeIndex = 0;
        int highIndex = 0;
        int lowIndex = 0;

        // Find the indices for the open, close, high, and low values on the grid
        for (int j = 0; j < gridHeight; j++) {
            if (calculation_grid[j][i * 2] == open) {
                openIndex = j;
            }
            if (calculation_grid[j][i * 2] == close) {
                closeIndex = j;
            }
            if (calculation_grid[j][i * 2] == high) {
                highIndex = j;
            }
            if (calculation_grid[j][i * 2] == low) {
                lowIndex = j;
            }
        }

        // Plot the vertical line for the high and low values
        if (lowIndex < highIndex) {
            for (int j = lowIndex; j <= highIndex; j++) {
                grid[j][i * 2] = '|';
            }
        } else {
            for (int j = highIndex; j <= lowIndex; j++) {
                grid[j][i * 2] = '|';
            }
        }

        // Plot the open and close values with the '-' character
        grid[closeIndex][i * 2] = '-';
        grid[openIndex][i * 2] = '-';
    }

    // Print the grid with y-axis labels
    int currentGridMin = gridMin;
    for (int i = 0; i < gridHeight; ++i) {
        std::cout << std::setw(4) << currentGridMin << "|\t"<< std::endl;
        for (int j = 0; j < gridWidth; ++j) {
            std::cout << std::setw(3) << grid[i][j]<< std::endl;
        }
        currentGridMin += INTERVAL;
        std::cout << std::endl;
    }

    // Draw a horizontal line at the bottom of the grid
    for (int i = 0; i < gridWidth; ++i) {
        std::cout << "---" << std::endl;
    }

    // Print the x-axis labels
    std::cout  << "\n YR " << "|"<< std::endl;
    for (int i = 0; i < candlesticks.size(); ++i) {
        std::cout << std::setw(6) << candlesticks[i].timeframe;
    }

    //spacing betwwen the data
    std::cout << "\n\n"<< std::endl;
}