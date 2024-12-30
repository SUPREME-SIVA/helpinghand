#include <iostream>
#include <string>
#include <ctime>

class WeatherStation {
    double temperature;
    double humidity;

public:
    WeatherStation() : temperature(25.5), humidity(60.0) {}

    // C++17 style with switch initialization
    void checkConditionsModern() {
        std::cout << "\nC++17 approach:" << std::endl;
        switch (int condition = static_cast<int>(temperature * humidity / 100); condition) {
            case 0 ... 500:
                std::cout << "Comfortable weather (Condition: " << condition << ")" << std::endl;
                break;
            case 501 ... 1000:
                std::cout << "Humid weather (Condition: " << condition << ")" << std::endl;
                break;
            default:
                std::cout << "Extreme weather (Condition: " << condition << ")" << std::endl;
        }
        // condition is not accessible here - better scope control
    }

    // C++11/14 style
    void checkConditionsLegacy() {
        std::cout << "\nC++11/14 approach:" << std::endl;
        int condition = static_cast<int>(temperature * humidity / 100);  // Variable declared outside switch
        switch (condition) {
            case 0 ... 500:
                std::cout << "Comfortable weather (Condition: " << condition << ")" << std::endl;
                break;
            case 501 ... 1000:
                std::cout << "Humid weather (Condition: " << condition << ")" << std::endl;
                break;
            default:
                std::cout << "Extreme weather (Condition: " << condition << ")" << std::endl;
        }
        // condition is still accessible here - potential scope pollution
    }

    void setConditions(double temp, double hum) {
        temperature = temp;
        humidity = hum;
    }
};

int main() {
    WeatherStation station;
    
    // Test with different weather conditions
    std::cout << "Default conditions:" << std::endl;
    station.checkConditionsModern();
    station.checkConditionsLegacy();
    
    std::cout << "\nModified conditions:" << std::endl;
    station.setConditions(35.0, 90.0);
    station.checkConditionsModern();
    station.checkConditionsLegacy();
    
    return 0;
}
