#include "Tempapp.h"  // Include the Tempapp class header file.
#include <iostream>  // Include the standard input/output stream library.
#include "TempD.h"  // Include the TempD header file (likely related to Temp data).
#include "TextBasedPlot.h"  // Include the header for the TextBasedPlot class used for plotting.

Tempapp::Tempapp() : TempD("./data.csv") {}  // Constructor initializes Tempapp and loads Temp data from a CSV file.

void Tempapp::init() {  // Initializes the Tempapp, sets up the country and starts the main loop.
    std::string selectedCountry;  // Declare a string to store the user's country selection.

    while (true) {  // Infinite loop to prompt the user until a valid country is selected.
        std::cout << "Available countries: " << std::endl;  // Display available countries.
        const auto& knownCountries = TempD.getKnownCountries();  // Get the list of known countries from TempD.
        for (const auto& country : knownCountries) {  // Loop through and display the list of countries.
            std::cout << country << " ";  // Print each country.
        }
        std::cout << "\nEnter the country: " << std::endl;  // Prompt for user input to select a country.
        std::cin >> selectedCountry;  // Read the user's input into the selectedCountry variable.

        // Validate if the selected country is in the known list.
        if (std::find(knownCountries.begin(), knownCountries.end(), selectedCountry) != knownCountries.end()) {
            TempD.setCountry(selectedCountry);  // Set the selected country in the TempD.
            break;  // Exit the loop if a valid country is selected.
        } else {
            std::cout << "Invalid country. Please select a valid country from the list.\n" << std::endl;  // Show an error if the country is invalid.
        }
    }

    while (true) {  // Main loop to keep showing the menu until the user chooses to exit.
        printMenu();  // Display the menu options.
        int option = getUserOption();  // Get the user's option selection.
        processUserOption(option);  // Process the selected option.
    }
}

void Tempapp::printMenu() {  // Displays the main menu with various options.
    std::cout << "1. Compute candlestick data\n"
              << "2. Create text-based plot\n"
              << "3. Filter and plot data\n"
              << "4. Predict and plot data\n"
              << "5. Change country\n"
              << "6. Exit\n"
              << "\n"
              << std::endl;
}

int Tempapp::getUserOption() {  // Gets the user's input for selecting a menu option.
    int option;  // Declare a variable to store the user's choice.
    while (true) {  // Keep looping until the user provides a valid option.
        std::cout << "Please choose an option from above: " << std::endl;  // Prompt the user to choose an option.
        if (std::cin >> option && option >= 1 && option <= 6) {  // Validate the input to ensure it's between 1 and 6.
            break;  // Exit the loop if the option is valid.
        }
        std::cout << "Invalid option. Please try again.\n" << std::endl;  // Show an error message if the option is invalid.
        std::cin.clear();  // Clear the input stream to handle invalid input.
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');  // Ignore the invalid input until the next newline.
    }
    return option;  // Return the valid option chosen by the user.
}

void Tempapp::processUserOption(int userOption) {  // Processes the user's selected menu option.
    switch (userOption) {  // Switch statement to handle each option.
        case 1:
            computeCandlestickData();  // Call function to compute candlestick data.
            break;
        case 2:
            makeTextPlot();  // Call function to create a text-based plot.
            break;
        case 3:
            filterPlotData();  // Call function to filter and plot data.
            break;
        case 4:
            predictPlotData();  // Call function to predict and plot data.
            break;
        case 5: {  // Case for changing the country.
            std::string country;  // Declare a string to store the new country.
            std::cout << "Countries: " << std::endl;  // Display available countries.
            const auto& knownCountries = TempD.getKnownCountries();  // Get the list of known countries.
            for (const auto& country : knownCountries) {  // Loop through and display the list of countries.
                std::cout << country << " ";  // Print each country.
            }
            std::cout << "\nEnter the country: " << std::endl;  // Prompt the user to enter a new country.
            std::cin >> country;  // Read the user's input for the country.
            TempD.setCountry(country);  // Set the new country in the TempD.
            break;
        }
        case 6:
            std::exit(0);  // Exit the program when option 6 is selected.
        default:
            std::cout << "Invalid option\n" << std::endl;  // Handle invalid option.
    }
}

void Tempapp::computeCandlestickData() {  // Computes the candlestick data and displays it.
    auto candlesticks = TempD.computeCandlestickData("yearly");  // Compute the yearly candlestick data.
    std::cout << "Candlestick Data\n" << std::endl;  // Display a message indicating candlestick data is about to be shown.
    Textbasedplotter plotter;  // Create a plotter object to display the data.
    plotter.displayCandlestickData(candlesticks);  // Display the computed candlestick data using the plotter.
}

void Tempapp::makeTextPlot() {  // Creates a text-based plot of the candlestick data.
    auto candlesticks = TempD.computeCandlestickData("yearly");  // Compute the yearly candlestick data.
    Textbasedplotter plotter;  // Create a plotter object to display the data.
    plotter.displayCandlestickChart(candlesticks);  // Display the candlestick data as a chart.
}

void Tempapp::filterPlotData() {  // Filters and plots data based on user input.
    int option;  // Declare a variable to store the user's filtering option.
    do {
        std::cout << "Filtering Data Options:\n1. By Year Range\n2. By Temperature Range\nChoose an option: " << std::endl;
        std::cin >> option;  // Get the user's filtering option.

        if (option != 1 && option != 2) {  // Validate input to ensure it's either 1 or 2.
            std::cout << "Invalid option. Please choose 1 or 2.\n\n" << std::endl;
        }
    } while (option != 1 && option != 2);  // Keep prompting until a valid option is chosen.

    // Case for filtering by year range.
    if (option == 1) {
        std::string start, end;  // Declare variables to store the start and end years.
        bool validYears = false;  // Flag to track if the years are valid.
        while (!validYears) {  // Loop until the user provides valid years.
            std::cout << "Enter the start year (YYYY): " << std::endl;
            std::cin >> start;  // Get the start year.
            std::cout << "Enter the end year (YYYY): " << std::endl;
            std::cin >> end;  // Get the end year.

            // Validate year format and range.
            if (start.length() == 4 && end.length() == 4 &&
                std::all_of(start.begin(), start.end(), ::isdigit) &&
                std::all_of(end.begin(), end.end(), ::isdigit) &&
                std::stoi(start) >= 1980 && std::stoi(end) <= 2019) {
                validYears = true;  // Exit the loop if valid years are provided.
            } else {
                std::cout << "Invalid year format. Please enter a valid year (YYYY) between 1980 and 2019.\n" << std::endl;
            }
        }

        auto candlesticks = TempD.computeCandlestickData("yearly");  // Compute the yearly candlestick data.
        auto filteredCandlesticks = TempD.filterDataByDateRange(candlesticks, start, end);  // Filter by date range.

        Textbasedplotter plotter;  // Create a plotter to display the filtered data.
        plotter.displayCandlestickChart(filteredCandlesticks);  // Display the filtered candlestick chart.
    }
    // Case for filtering by temperature range.
    else if (option == 2) {
        double minTemp, maxTemp;  // Declare variables for the minimum and maximum temperatures.
        bool validTemps = false;  // Flag to track if the temperature range is valid.

        while (!validTemps) {  // Loop until the user provides valid temperature values.
            std::cout << "Enter minimum temperature: " << std::endl;
            std::cin >> minTemp;  // Get the minimum temperature.
            std::cout << "Enter maximum temperature: " << std::endl;
            std::cin >> maxTemp;  // Get the maximum temperature.

            // Validate the temperature range.
            if (minTemp >= -30 && maxTemp <= 40 && minTemp <= maxTemp) {
                validTemps = true;  // Exit the loop if valid temperatures are provided.
            } else {
                std::cout << "Invalid temperature range. Please enter a temperature between -30 and 40.\n" << std::endl;
            }
        }

        auto candlesticks = TempD.computeCandlestickData("yearly");  // Compute the yearly candlestick data.
        auto filteredCandlesticks = TempD.filterDataByTemperatureRange(candlesticks, minTemp, maxTemp);  // Filter by temperature range.

        Textbasedplotter plotter;  // Create a plotter to display the filtered data.
        plotter.displayCandlestickChart(filteredCandlesticks);  // Display the filtered candlestick chart.
    } else {
        std::cout << "Invalid option\n" << std::endl;  // Handle invalid option.
    }
}

void Tempapp::predictPlotData() {  // Predicts and plots data for future years.
    const auto& knownCountries = TempD.getKnownCountries();  // Get the list of known countries.
    std::string selectedCountry;  // Declare a variable to store the selected country.

    while (true) {  // Keep prompting until a valid country is selected.
        std::cout << "Countries: " << std::endl;
        for (const auto& country : knownCountries) {  // Loop through and display available countries.
            std::cout << country << " ";  // Print each country.
        }
        std::cout << "\nEnter the country you want to predict data for: " << std::endl;
        std::cin >> selectedCountry;  // Get the user's country input.

        // Validate the selected country.
        if (std::find(knownCountries.begin(), knownCountries.end(), selectedCountry) != knownCountries.end()) {
            TempD.setCountry(selectedCountry);  // Set the selected country in the TempD.
            break;
        } else {
            std::cout << "Invalid country. Please select a valid country from the list.\n" << std::endl;
        }
    }

    int startYear, endYear;  // Declare variables for the start and end years for prediction.
    while (true) {  // Keep prompting until the user provides a valid year range.
        std::cout << "Enter the start year to predict: " << std::endl;
        std::cin >> startYear;  // Get the start year.
        std::cout << "Enter the end year to predict: " << std::endl;
        std::cin >> endYear;  // Get the end year.

        // Validate the year range.
        if (startYear > 2020 && endYear > 2020 && endYear >= startYear) {
            break;  // Exit the loop if the year range is valid.
        } else {
            std::cout << "Invalid year range. Please enter years greater than 2020, and ensure the end year is not earlier than the start year.\n" << std::endl;
        }
    }

    std::vector<int> futureYears;  // Declare a vector to store the future years.
    for (int year = startYear; year <= endYear; ++year) {  // Loop through the years in the prediction range.
        futureYears.push_back(year);  // Add each year to the vector.
    }

    auto candlesticks = TempD.computeCandlestickData("yearly");  // Compute the yearly candlestick data.
    auto predictions = TempD.predictCandlestickData(candlesticks, futureYears);  // Generate predictions for the future years.

    if (predictions.empty()) {  // Check if there are no predictions available.
        std::cout << "No predictions could be generated for the specified year range.\n" << std::endl;
        return;
    }

    Textbasedplotter plotter;  // Create a plotter to display the predicted data.
    plotter.displayCandlestickChart(predictions);  // Display the predicted candlestick chart.
    plotter.displayCandlestickData(predictions);  // Display the predicted candlestick data.
}