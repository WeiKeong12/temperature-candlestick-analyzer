# Temperature Candlestick Analyzer

A C++ console application that analyzes historical temperature data and visualizes it as text-based candlestick charts. Supports filtering by date range or temperature range, and predicting future temperatures using linear regression.

## Features

- Loads historical daily temperature data from a CSV file, with one column per country
- Computes candlestick data (open, high, low, close) aggregated by year, month, or day
- Renders candlestick charts directly in the terminal using ASCII characters
- Filters candlestick data by year range or by temperature range
- Predicts future temperature candlesticks using linear regression
- Supports switching between countries at runtime

## How to Use

Run the compiled program and follow the on-screen prompts:

1. Select a country from the list of available countries.
2. Choose an option from the main menu:
   - Compute candlestick data
   - Create a text-based plot
   - Filter and plot data (by year range or temperature range)
   - Predict and plot future data
   - Change country
   - Exit

## Built With

- C++ (standard library only — no external dependencies)

## Building

Compile all source files together, for example with g++:

```
g++ -std=c++17 -o temp-candlestick main.cpp Tempapp.cpp TempD.cpp TempLog.cpp CandlestickData.cpp TextBasedPlot.cpp fileReader.cpp
```

Then run the resulting executable:

```
./temp-candlestick
```

Note: `data.csv` must be located in the same directory as the executable, since the program loads it using a relative path.

## Project Structure

```
├── main.cpp              # Entry point, launches the application
├── Tempapp.cpp / .h       # Main application loop, menu, and user interaction
├── TempD.cpp / .h         # Core data processing: candlestick computation, filtering, prediction
├── TempLog.cpp / .h       # Represents a single temperature log entry
├── CandlestickData.cpp / .h   # Represents a single candlestick (open, high, low, close)
├── TextBasedPlot.cpp / .h     # Renders candlestick data as an ASCII chart
├── fileReader.cpp / .h    # Generic CSV file reading utility
└── data.csv               # Historical temperature dataset, one column per country
```

## Notes

This project was built as an exercise in working with C++ classes, file I/O, and building simple data visualizations without external plotting libraries.

## License

This project is licensed under the MIT License.
