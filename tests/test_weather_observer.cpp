#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <algorithm>
#include <ctime>
#include <cassert>
#include <sstream>

// Redirect cout for testing
class CoutRedirect {
private:
    std::streambuf* old_buffer;
    std::ostringstream stream_buffer;

public:
    CoutRedirect() {
        old_buffer = std::cout.rdbuf();
        std::cout.rdbuf(stream_buffer.rdbuf());
    }

    ~CoutRedirect() {
        std::cout.rdbuf(old_buffer);
    }

    std::string get_output() {
        return stream_buffer.str();
    }
};

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

// Test observer that counts updates
class TestObserver : public WeatherObserver {
private:
    int update_count = 0;
    WeatherData last_data{0, 0, 0};
    
public:
    void update(const WeatherData& data) override {
        update_count++;
        last_data = data;
    }
    
    int getUpdateCount() const {
        return update_count;
    }
    
    WeatherData getLastData() const {
        return last_data;
    }
};

// Concrete Observers from weather_observer.cpp
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
    
    std::string getLocation() const {
        return location;
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
    
    double getTempThreshold() const {
        return tempThreshold;
    }
    
    double getHumidityThreshold() const {
        return humidityThreshold;
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
    
    int getObserverCount() const {
        return observers.size();
    }
    
    WeatherData getCurrentData() const {
        return current_data;
    }
    
    std::string getLocation() const {
        return location;
    }

private:
    void notifyObservers() {
        for (const auto& observer : observers) {
            observer->update(current_data);
        }
    }
};

// Test functions
void test_weather_data() {
    std::cout << "Testing WeatherData structure..." << std::endl;
    
    WeatherData data(25.5, 60.0, 1013.0);
    assert(data.temperature == 25.5);
    assert(data.humidity == 60.0);
    assert(data.pressure == 1013.0);
    assert(data.timestamp > 0);
    
    std::cout << "WeatherData test passed!" << std::endl;
}

void test_weather_station_creation() {
    std::cout << "Testing WeatherStation creation..." << std::endl;
    
    WeatherStation station("Test Location", 20.0, 50.0, 1010.0);
    assert(station.getLocation() == "Test Location");
    
    WeatherData data = station.getCurrentData();
    assert(data.temperature == 20.0);
    assert(data.humidity == 50.0);
    assert(data.pressure == 1010.0);
    
    assert(station.getObserverCount() == 0);
    
    std::cout << "WeatherStation creation test passed!" << std::endl;
}

void test_attach_detach_observers() {
    std::cout << "Testing attach/detach observers..." << std::endl;
    
    WeatherStation station("Test Location", 20.0, 50.0, 1010.0);
    
    auto observer1 = std::make_shared<TestObserver>();
    auto observer2 = std::make_shared<TestObserver>();
    
    station.attach(observer1);
    assert(station.getObserverCount() == 1);
    
    station.attach(observer2);
    assert(station.getObserverCount() == 2);
    
    station.detach(observer1);
    assert(station.getObserverCount() == 1);
    
    station.detach(observer2);
    assert(station.getObserverCount() == 0);
    
    std::cout << "Attach/detach observers test passed!" << std::endl;
}

void test_observer_notification() {
    std::cout << "Testing observer notification..." << std::endl;
    
    WeatherStation station("Test Location", 20.0, 50.0, 1010.0);
    
    auto observer = std::make_shared<TestObserver>();
    station.attach(observer);
    
    assert(observer->getUpdateCount() == 0);
    
    station.measurementsChanged(22.0, 55.0, 1012.0);
    assert(observer->getUpdateCount() == 1);
    
    WeatherData lastData = observer->getLastData();
    assert(lastData.temperature == 22.0);
    assert(lastData.humidity == 55.0);
    assert(lastData.pressure == 1012.0);
    
    station.measurementsChanged(23.0, 56.0, 1013.0);
    assert(observer->getUpdateCount() == 2);
    
    lastData = observer->getLastData();
    assert(lastData.temperature == 23.0);
    assert(lastData.humidity == 56.0);
    assert(lastData.pressure == 1013.0);
    
    std::cout << "Observer notification test passed!" << std::endl;
}

void test_display_observer() {
    std::cout << "Testing DisplayObserver..." << std::endl;
    
    CoutRedirect redirect;
    
    auto display = std::make_shared<DisplayObserver>("Test Display");
    assert(display->getLocation() == "Test Display");
    
    WeatherData data(25.0, 60.0, 1013.0);
    display->update(data);
    
    std::string output = redirect.get_output();
    assert(output.find("Weather Display at Test Display") != std::string::npos);
    assert(output.find("Temperature: 25") != std::string::npos);
    assert(output.find("Humidity: 60") != std::string::npos);
    assert(output.find("Pressure: 1013") != std::string::npos);
    
    std::cout << "DisplayObserver test passed!" << std::endl;
}

void test_alert_observer() {
    std::cout << "Testing AlertObserver..." << std::endl;
    
    CoutRedirect redirect;
    
    auto alert = std::make_shared<AlertObserver>(30.0, 80.0);
    assert(alert->getTempThreshold() == 30.0);
    assert(alert->getHumidityThreshold() == 80.0);
    
    // No alerts should be triggered
    WeatherData normalData(25.0, 60.0, 1013.0);
    alert->update(normalData);
    std::string output1 = redirect.get_output();
    assert(output1.find("ALERT") == std::string::npos);
    
    // Reset the buffer - create a new instance instead of assignment
    redirect.~CoutRedirect();
    new (&redirect) CoutRedirect();
    
    // Temperature alert should be triggered
    WeatherData highTempData(35.0, 60.0, 1013.0);
    alert->update(highTempData);
    std::string output2 = redirect.get_output();
    assert(output2.find("ALERT: High temperature detected") != std::string::npos);
    
    // Reset the buffer - create a new instance instead of assignment
    redirect.~CoutRedirect();
    new (&redirect) CoutRedirect();
    
    // Humidity alert should be triggered
    WeatherData highHumidityData(25.0, 85.0, 1013.0);
    alert->update(highHumidityData);
    std::string output3 = redirect.get_output();
    assert(output3.find("ALERT: High humidity detected") != std::string::npos);
    
    std::cout << "AlertObserver test passed!" << std::endl;
}

int main() {
    std::cout << "Running weather_observer tests..." << std::endl;
    
    test_weather_data();
    test_weather_station_creation();
    test_attach_detach_observers();
    test_observer_notification();
    test_display_observer();
    test_alert_observer();
    
    std::cout << "All weather_observer tests passed!" << std::endl;
    return 0;
}
