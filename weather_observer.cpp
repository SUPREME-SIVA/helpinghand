#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <algorithm>
#include <ctime>

// Weather data structure
struct WeatherData {
    double temperature;
    double humidity;
    double pressure;
    std::time_t timestamp;

    WeatherData(double temp, double hum, double press)
        : temperature(temp), humidity(hum), pressure(press) {
        timestamp = std::time(nullptr);
    }
};

// Observer interface
class WeatherObserver {
public:
    virtual void update(const WeatherData& data) = 0;
    virtual ~WeatherObserver() = default;
};

// Concrete Observers
class DisplayObserver : public WeatherObserver {
    std::string location;

public:
    explicit DisplayObserver(std::string loc) : location(std::move(loc)) {}

    void update(const WeatherData& data) override {
        std::cout << "Weather Display at " << location << ":\n"
                  << "Temperature: " << data.temperature << "°C\n"
                  << "Humidity: " << data.humidity << "%\n"
                  << "Pressure: " << data.pressure << " hPa\n"
                  << "Time: " << std::ctime(&data.timestamp) << std::endl;
    }
};

class AlertObserver : public WeatherObserver {
    double tempThreshold;
    double humidityThreshold;

public:
    AlertObserver(double temp_threshold, double humidity_threshold)
        : tempThreshold(temp_threshold), humidityThreshold(humidity_threshold) {}

    void update(const WeatherData& data) override {
        if (data.temperature > tempThreshold) {
            std::cout << "ALERT: High temperature detected! " 
                      << data.temperature << "°C\n";
        }
        if (data.humidity > humidityThreshold) {
            std::cout << "ALERT: High humidity detected! " 
                      << data.humidity << "%\n";
        }
    }
};

class LoggerObserver : public WeatherObserver {
public:
    void update(const WeatherData& data) override {
        std::cout << "Logging weather data at " 
                  << std::ctime(&data.timestamp)
                  << "T: " << data.temperature << "°C, "
                  << "H: " << data.humidity << "%, "
                  << "P: " << data.pressure << " hPa\n";
    }
};

// Subject (Observable)
class WeatherStation {
    std::vector<std::shared_ptr<WeatherObserver>> observers;
    WeatherData current_data;
    std::string location;

public:
    WeatherStation(std::string loc, double temp, double humidity, double pressure)
        : current_data(temp, humidity, pressure), location(std::move(loc)) {}

    void attach(const std::shared_ptr<WeatherObserver>& observer) {
        observers.push_back(observer);
        std::cout << "New observer attached to " << location << " weather station\n";
    }

    void detach(const std::shared_ptr<WeatherObserver>& observer) {
        observers.erase(
            std::remove(observers.begin(), observers.end(), observer),
            observers.end()
        );
        std::cout << "Observer detached from " << location << " weather station\n";
    }

    void measurementsChanged(double temp, double humidity, double pressure) {
        current_data = WeatherData(temp, humidity, pressure);
        notifyObservers();
    }

private:
    void notifyObservers() {
        for (const auto& observer : observers) {
            observer->update(current_data);
        }
    }
};

int main() {
    // Create weather station
    WeatherStation station("New York", 25.0, 60.0, 1013.0);

    // Create observers
    auto display = std::make_shared<DisplayObserver>("Main Street");
    auto alert = std::make_shared<AlertObserver>(30.0, 80.0);
    auto logger = std::make_shared<LoggerObserver>();

    // Register observers
    station.attach(display);
    station.attach(alert);
    station.attach(logger);

    std::cout << "\nSimulating weather changes:\n";
    
    // Simulate weather changes
    std::cout << "\nNormal conditions:\n";
    station.measurementsChanged(28.0, 65.0, 1012.0);

    std::cout << "\nHot and humid conditions:\n";
    station.measurementsChanged(32.0, 85.0, 1010.0);

    // Detach one observer
    std::cout << "\nDetaching display observer:\n";
    station.detach(display);

    std::cout << "\nAfter detaching display:\n";
    station.measurementsChanged(27.0, 70.0, 1011.0);

    return 0;
}
