#include <iostream>
#include <tuple>
#include <map>
#include <string>

// Example 1: Real-time Stock Market Data Processing
struct StockData {
    std::string symbol;
    double price;
    int volume;
    bool trending_up;
};

// Function that simulates fetching stock data
StockData getStockData() {
    return {"AAPL", 189.75, 1000000, true};
}

// Example 2: Weather Monitoring System
class WeatherStation {
public:
    struct WeatherData {
        double temperature;
        double humidity;
        double pressure;
    };

    std::map<std::string, WeatherData> weather_records;

    // Function to add weather data for a location
    void addWeatherData(const std::string& location, double temp, double humid, double press) {
        weather_records[location] = {temp, humid, press};
    }
};

int main() {
    std::cout << "Example 1: Stock Market Data Processing\n";
    std::cout << "---------------------------------------\n";

    // C++17 Structured Binding
    {
        std::cout << "\nC++17 Structured Binding approach:\n";
        auto [symbol, price, volume, trend] = getStockData();
        std::cout << "Stock: " << symbol << "\n"
                  << "Price: $" << price << "\n"
                  << "Volume: " << volume << "\n"
                  << "Trending Up: " << (trend ? "Yes" : "No") << "\n";
    }

    // C++11/14 approach
    {
        std::cout << "\nC++11/14 approach:\n";
        StockData data = getStockData();
        std::string symbol = data.symbol;
        double price = data.price;
        int volume = data.volume;
        bool trend = data.trending_up;
        std::cout << "Stock: " << symbol << "\n"
                  << "Price: $" << price << "\n"
                  << "Volume: " << volume << "\n"
                  << "Trending Up: " << (trend ? "Yes" : "No") << "\n";
    }

    std::cout << "\nExample 2: Weather Monitoring System\n";
    std::cout << "------------------------------------\n";

    WeatherStation station;
    station.addWeatherData("New York", 25.5, 65.0, 1013.2);
    station.addWeatherData("London", 18.3, 78.0, 1011.5);

    // C++17 Structured Binding with map iteration
    {
        std::cout << "\nC++17 Structured Binding approach:\n";
        for (const auto& [location, data] : station.weather_records) {
            const auto& [temp, humidity, pressure] = data;
            std::cout << "Location: " << location << "\n"
                      << "Temperature: " << temp << "°C\n"
                      << "Humidity: " << humidity << "%\n"
                      << "Pressure: " << pressure << " hPa\n\n";
        }
    }

    // C++11/14 approach with map iteration
    {
        std::cout << "C++11/14 approach:\n";
        for (const auto& pair : station.weather_records) {
            const std::string& location = pair.first;
            const WeatherStation::WeatherData& data = pair.second;
            std::cout << "Location: " << location << "\n"
                      << "Temperature: " << data.temperature << "°C\n"
                      << "Humidity: " << data.humidity << "%\n"
                      << "Pressure: " << data.pressure << " hPa\n\n";
        }
    }

    // Example with tuple (C++17)
    {
        std::cout << "\nTuple example with C++17:\n";
        std::tuple<std::string, double, int> stock_info{"GOOGL", 138.92, 500000};
        auto [name, current_price, trade_volume] = stock_info;
        std::cout << "Stock: " << name << ", Price: $" << current_price 
                  << ", Volume: " << trade_volume << "\n";
    }

    // Tuple example (C++11/14)
    {
        std::cout << "\nTuple example with C++11/14:\n";
        std::tuple<std::string, double, int> stock_info{"GOOGL", 138.92, 500000};
        std::string name = std::get<0>(stock_info);
        double current_price = std::get<1>(stock_info);
        int trade_volume = std::get<2>(stock_info);
        std::cout << "Stock: " << name << ", Price: $" << current_price 
                  << ", Volume: " << trade_volume << "\n";
    }

    return 0;
}
