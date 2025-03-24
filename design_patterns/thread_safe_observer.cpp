/**
 * @file thread_safe_observer.cpp
 * @brief Implementation of the Observer design pattern using smart pointers
 * 
 * This file demonstrates a thread-safe implementation of the Observer pattern
 * with smart pointers for memory management. The Observer pattern defines a
 * one-to-many dependency between objects so that when one object changes state,
 * all its dependents are notified and updated automatically.
 * 
 * Thread safety: Uses mutex locks to ensure thread-safe observer management
 * Memory safety: Uses shared_ptr and weak_ptr for automatic memory management
 * 
 * @author HelpingHand
 * @date March 22, 2025
 */

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <memory>
#include <mutex>
#include <unordered_map>
#include <functional>

// Forward declarations
template <typename T>
class Observer;

template <typename T>
class Subject;

/**
 * @brief Observer template class that defines the interface for receiving updates
 * 
 * @tparam T The type of data that will be passed in updates
 */
template <typename T>
class Observer : public std::enable_shared_from_this<Observer<T>> {
public:
    virtual ~Observer() = default;
    
    /**
     * @brief Method called when the observed subject changes state
     * 
     * @param subject Pointer to the subject that triggered the update
     * @param data The updated data
     */
    virtual void onUpdate(std::shared_ptr<Subject<T>> subject, const T& data) = 0;
    
    /**
     * @brief Get the unique identifier for this observer
     * 
     * @return std::string The observer's ID
     */
    virtual std::string getId() const = 0;
};

/**
 * @brief Subject template class that maintains a list of observers
 * 
 * @tparam T The type of data that will be passed in updates
 */
template <typename T>
class Subject : public std::enable_shared_from_this<Subject<T>> {
private:
    // Store weak_ptr to observers to avoid circular references
    std::unordered_map<std::string, std::weak_ptr<Observer<T>>> observers;
    mutable std::mutex observerMutex;
    std::string name;

public:
    /**
     * @brief Construct a new Subject object
     * 
     * @param name Name of the subject for identification
     */
    explicit Subject(const std::string& name) : name(name) {}
    
    virtual ~Subject() {
        std::cout << "Subject '" << name << "' destroyed" << std::endl;
    }
    
    /**
     * @brief Get the name of this subject
     * 
     * @return std::string The subject's name
     */
    std::string getName() const {
        return name;
    }
    
    /**
     * @brief Register an observer to receive updates
     * 
     * @param observer The observer to register
     * @return bool True if registration was successful
     */
    bool registerObserver(std::shared_ptr<Observer<T>> observer) {
        if (!observer) {
            return false;
        }
        
        std::lock_guard<std::mutex> lock(observerMutex);
        
        std::string id = observer->getId();
        observers[id] = observer;
        
        std::cout << "Observer '" << id << "' registered with Subject '" << name << "'" << std::endl;
        return true;
    }
    
    /**
     * @brief Remove an observer from the notification list
     * 
     * @param observerId The ID of the observer to remove
     * @return bool True if removal was successful
     */
    bool removeObserver(const std::string& observerId) {
        std::lock_guard<std::mutex> lock(observerMutex);
        
        auto it = observers.find(observerId);
        if (it != observers.end()) {
            observers.erase(it);
            std::cout << "Observer '" << observerId << "' removed from Subject '" << name << "'" << std::endl;
            return true;
        }
        
        return false;
    }
    
    /**
     * @brief Notify all registered observers of a state change
     * 
     * @param data The updated data to send to observers
     */
    void notifyObservers(const T& data) {
        std::lock_guard<std::mutex> lock(observerMutex);
        
        // Create a temporary list of valid observers
        std::vector<std::shared_ptr<Observer<T>>> validObservers;
        
        // Clean up expired observers and collect valid ones
        for (auto it = observers.begin(); it != observers.end();) {
            if (auto observer = it->second.lock()) {
                validObservers.push_back(observer);
                ++it;
            } else {
                // Remove expired weak_ptr
                std::cout << "Removing expired observer '" << it->first << "' from Subject '" << name << "'" << std::endl;
                it = observers.erase(it);
            }
        }
        
        // Notify all valid observers
        for (auto& observer : validObservers) {
            observer->onUpdate(this->shared_from_this(), data);
        }
    }
    
    /**
     * @brief Get the current number of registered observers
     * 
     * @return size_t Number of observers
     */
    size_t getObserverCount() const {
        std::lock_guard<std::mutex> lock(observerMutex);
        return observers.size();
    }
};

/**
 * @brief Example data structure for weather information
 */
struct WeatherData {
    double temperature;
    double humidity;
    double pressure;
    std::string condition;
    
    friend std::ostream& operator<<(std::ostream& os, const WeatherData& data) {
        os << "Temperature: " << data.temperature << "°C, "
           << "Humidity: " << data.humidity << "%, "
           << "Pressure: " << data.pressure << " hPa, "
           << "Condition: " << data.condition;
        return os;
    }
};

/**
 * @brief Concrete weather station subject that monitors weather conditions
 */
class WeatherStation : public Subject<WeatherData> {
private:
    WeatherData currentWeather;
    mutable std::mutex weatherMutex;

public:
    /**
     * @brief Construct a new Weather Station object
     * 
     * @param location The location of the weather station
     */
    explicit WeatherStation(const std::string& location)
        : Subject<WeatherData>(location) {}
    
    /**
     * @brief Get the current weather data
     * 
     * @return WeatherData The current weather
     */
    WeatherData getCurrentWeather() const {
        std::lock_guard<std::mutex> lock(weatherMutex);
        return currentWeather;
    }
    
    /**
     * @brief Update the weather data and notify observers
     * 
     * @param newWeather The new weather data
     */
    void setWeatherData(const WeatherData& newWeather) {
        {
            std::lock_guard<std::mutex> lock(weatherMutex);
            currentWeather = newWeather;
        }
        
        std::cout << "Weather updated at station '" << getName() << "': " << newWeather << std::endl;
        notifyObservers(newWeather);
    }
};

/**
 * @brief Concrete observer that displays weather information
 */
class WeatherDisplay : public Observer<WeatherData> {
private:
    std::string displayId;
    std::string displayName;
    bool isActive;

public:
    /**
     * @brief Construct a new Weather Display object
     * 
     * @param id Unique identifier for this display
     * @param name Human-readable name for this display
     */
    WeatherDisplay(const std::string& id, const std::string& name)
        : displayId(id), displayName(name), isActive(true) {}
    
    ~WeatherDisplay() {
        std::cout << "WeatherDisplay '" << displayName << "' destroyed" << std::endl;
    }
    
    /**
     * @brief Get the unique identifier for this display
     * 
     * @return std::string The display's ID
     */
    std::string getId() const override {
        return displayId;
    }
    
    /**
     * @brief Set whether this display is active
     * 
     * @param active True if the display should be active
     */
    void setActive(bool active) {
        isActive = active;
        std::cout << "WeatherDisplay '" << displayName << "' is now " 
                  << (isActive ? "active" : "inactive") << std::endl;
    }
    
    /**
     * @brief Handle weather updates from the subject
     * 
     * @param subject The weather station that sent the update
     * @param data The updated weather data
     */
    void onUpdate(std::shared_ptr<Subject<WeatherData>> subject, const WeatherData& data) override {
        if (!isActive) {
            std::cout << "WeatherDisplay '" << displayName << "' received update but is inactive" << std::endl;
            return;
        }
        
        std::cout << "WeatherDisplay '" << displayName << "' received update from station '" 
                  << subject->getName() << "'" << std::endl;
        
        // Display the weather information
        std::cout << "  " << displayName << " shows: " << data << std::endl;
        
        // Provide additional information based on the weather
        if (data.temperature > 30.0) {
            std::cout << "  " << displayName << " alert: High temperature warning!" << std::endl;
        } else if (data.temperature < 0.0) {
            std::cout << "  " << displayName << " alert: Freezing temperature warning!" << std::endl;
        }
        
        if (data.condition == "Stormy") {
            std::cout << "  " << displayName << " alert: Storm warning in effect!" << std::endl;
        }
    }
};

/**
 * @brief Concrete observer that records weather history
 */
class WeatherRecorder : public Observer<WeatherData> {
private:
    std::string recorderId;
    std::vector<std::pair<std::string, WeatherData>> history;
    mutable std::mutex historyMutex;
    size_t maxHistorySize;

public:
    /**
     * @brief Construct a new Weather Recorder object
     * 
     * @param id Unique identifier for this recorder
     * @param maxHistory Maximum number of weather records to keep
     */
    WeatherRecorder(const std::string& id, size_t maxHistory = 10)
        : recorderId(id), maxHistorySize(maxHistory) {}
    
    ~WeatherRecorder() {
        std::cout << "WeatherRecorder '" << recorderId << "' destroyed" << std::endl;
    }
    
    /**
     * @brief Get the unique identifier for this recorder
     * 
     * @return std::string The recorder's ID
     */
    std::string getId() const override {
        return recorderId;
    }
    
    /**
     * @brief Handle weather updates from the subject
     * 
     * @param subject The weather station that sent the update
     * @param data The updated weather data
     */
    void onUpdate(std::shared_ptr<Subject<WeatherData>> subject, const WeatherData& data) override {
        std::lock_guard<std::mutex> lock(historyMutex);
        
        // Add the new weather data to history
        history.emplace_back(subject->getName(), data);
        
        std::cout << "WeatherRecorder '" << recorderId << "' recorded data from station '" 
                  << subject->getName() << "'" << std::endl;
        
        // Trim history if it exceeds the maximum size
        if (history.size() > maxHistorySize) {
            history.erase(history.begin());
        }
    }
    
    /**
     * @brief Display the weather history
     */
    void displayHistory() const {
        std::lock_guard<std::mutex> lock(historyMutex);
        
        std::cout << "\n=== Weather History (Recorder '" << recorderId << "') ===" << std::endl;
        
        if (history.empty()) {
            std::cout << "No weather data recorded yet." << std::endl;
        } else {
            for (size_t i = 0; i < history.size(); ++i) {
                std::cout << "Record " << (i + 1) << " from station '" << history[i].first 
                          << "': " << history[i].second << std::endl;
            }
        }
        
        std::cout << "===================================" << std::endl;
    }
};

/**
 * @brief Demonstrates the Observer pattern with smart pointers
 * 
 * @return int Exit code
 */
int main() {
    std::cout << "=== Observer Pattern with Smart Pointers ===" << std::endl;
    
    // Create subjects (weather stations)
    auto cityStation = std::make_shared<WeatherStation>("City Center");
    auto airportStation = std::make_shared<WeatherStation>("Airport");
    
    // Create observers
    auto mobileApp = std::make_shared<WeatherDisplay>("mobile_app", "Mobile Weather App");
    auto webDisplay = std::make_shared<WeatherDisplay>("web_display", "Website Weather Widget");
    auto weatherRecorder = std::make_shared<WeatherRecorder>("main_recorder");
    
    // Register observers with subjects
    cityStation->registerObserver(mobileApp);
    cityStation->registerObserver(webDisplay);
    cityStation->registerObserver(weatherRecorder);
    
    airportStation->registerObserver(mobileApp);
    airportStation->registerObserver(weatherRecorder);
    
    // Update weather at city station
    WeatherData cityWeather{25.5, 65.0, 1013.2, "Sunny"};
    cityStation->setWeatherData(cityWeather);
    
    std::cout << "\n--- Updating airport weather ---" << std::endl;
    
    // Update weather at airport station
    WeatherData airportWeather{23.8, 70.5, 1012.8, "Partly Cloudy"};
    airportStation->setWeatherData(airportWeather);
    
    // Deactivate the web display
    std::cout << "\n--- Deactivating web display ---" << std::endl;
    webDisplay->setActive(false);
    
    // Update city weather again
    std::cout << "\n--- Updating city weather again ---" << std::endl;
    WeatherData updatedCityWeather{32.1, 55.0, 1010.5, "Hot"};
    cityStation->setWeatherData(updatedCityWeather);
    
    // Display weather history
    weatherRecorder->displayHistory();
    
    // Remove mobile app from city station
    std::cout << "\n--- Removing mobile app from city station ---" << std::endl;
    cityStation->removeObserver("mobile_app");
    
    // Update city weather one more time
    std::cout << "\n--- Final weather update ---" << std::endl;
    WeatherData stormyWeather{18.5, 85.0, 998.5, "Stormy"};
    cityStation->setWeatherData(stormyWeather);
    
    // Check observer counts
    std::cout << "\nCity Station observer count: " << cityStation->getObserverCount() << std::endl;
    std::cout << "Airport Station observer count: " << airportStation->getObserverCount() << std::endl;
    
    // Demonstrate automatic cleanup of expired observers
    std::cout << "\n--- Demonstrating automatic cleanup ---" << std::endl;
    {
        // Create a temporary observer in a new scope
        auto tempObserver = std::make_shared<WeatherDisplay>("temp_display", "Temporary Display");
        airportStation->registerObserver(tempObserver);
        
        // Update weather to notify the temporary observer
        WeatherData tempWeather{22.0, 68.0, 1011.0, "Clear"};
        airportStation->setWeatherData(tempWeather);
        
        std::cout << "Airport Station observer count: " << airportStation->getObserverCount() << std::endl;
        
        // tempObserver will be destroyed when we exit this scope
    }
    
    // Update weather again to trigger cleanup of expired observers
    std::cout << "\n--- Update after temporary observer is gone ---" << std::endl;
    WeatherData finalWeather{21.5, 70.0, 1012.0, "Clear Night"};
    airportStation->setWeatherData(finalWeather);
    
    std::cout << "Airport Station observer count: " << airportStation->getObserverCount() << std::endl;
    
    return 0;
}
