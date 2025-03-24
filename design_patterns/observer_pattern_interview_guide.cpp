/**
 * @file observer_pattern_interview_guide.cpp
 * @brief Implementation of the Observer design pattern with real-world examples
 * 
 * This file demonstrates the Observer pattern, which defines a one-to-many dependency
 * between objects so that when one object changes state, all its dependents are
 * notified and updated automatically.
 * 
 * Key components:
 * - Subject: Maintains a list of observers and notifies them of state changes
 * - Observer: Interface that defines the update method for concrete observers
 * - ConcreteSubject: Implements the Subject interface and maintains state
 * - ConcreteObserver: Implements the Observer interface to respond to updates
 * 
 * Thread safety: This implementation uses mutex locks to ensure thread-safe
 * observer registration, removal, and notification.
 * 
 * Memory safety: Uses smart pointers to manage observer lifetimes.
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
#include <chrono>
#include <thread>
#include <iomanip>
#include <sstream>
#include <functional>

// Forward declarations
class Observer;
class Subject;

/**
 * @brief Observer interface that all concrete observers must implement
 * 
 * This abstract class defines the update method that will be called by
 * the Subject when its state changes.
 */
class Observer {
public:
    virtual ~Observer() = default;
    
    /**
     * @brief Method called when the observed subject changes state
     * 
     * @param subject The subject that triggered the update
     * @param message Additional context about the state change
     */
    virtual void update(Subject* subject, const std::string& message) = 0;
    
    /**
     * @brief Get the name of this observer for identification
     * 
     * @return std::string The observer's name
     */
    virtual std::string getName() const = 0;
};

/**
 * @brief Subject interface that maintains a list of observers
 * 
 * This class manages observer registration and notification.
 */
class Subject {
private:
    std::vector<std::shared_ptr<Observer>> observers;
    mutable std::mutex observerMutex;

public:
    virtual ~Subject() = default;
    
    /**
     * @brief Register an observer to receive updates
     * 
     * @param observer The observer to register
     */
    void registerObserver(std::shared_ptr<Observer> observer) {
        std::lock_guard<std::mutex> lock(observerMutex);
        
        // Check if observer already exists
        auto it = std::find_if(observers.begin(), observers.end(), 
            [&observer](const std::shared_ptr<Observer>& o) {
                return o->getName() == observer->getName();
            });
            
        if (it == observers.end()) {
            observers.push_back(observer);
            std::cout << "Registered observer: " << observer->getName() << std::endl;
        }
    }
    
    /**
     * @brief Remove an observer from the notification list
     * 
     * @param observerName The name of the observer to remove
     */
    void removeObserver(const std::string& observerName) {
        std::lock_guard<std::mutex> lock(observerMutex);
        
        auto it = std::find_if(observers.begin(), observers.end(), 
            [&observerName](const std::shared_ptr<Observer>& o) {
                return o->getName() == observerName;
            });
            
        if (it != observers.end()) {
            std::cout << "Removed observer: " << (*it)->getName() << std::endl;
            observers.erase(it);
        }
    }
    
    /**
     * @brief Notify all registered observers of a state change
     * 
     * @param message Additional context about the state change
     */
    void notifyObservers(const std::string& message) {
        std::lock_guard<std::mutex> lock(observerMutex);
        
        for (auto& observer : observers) {
            observer->update(this, message);
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

// ==================================================================================
// Example 1: Stock Market Price Monitoring System
// ==================================================================================

/**
 * @brief Represents a stock in a market
 * 
 * This is a concrete subject that maintains stock price and notifies
 * observers when the price changes.
 */
class Stock : public Subject {
private:
    std::string symbol;
    double price;
    mutable std::mutex priceMutex;

public:
    /**
     * @brief Construct a new Stock object
     * 
     * @param symbol The stock's ticker symbol
     * @param initialPrice The initial price of the stock
     */
    Stock(const std::string& symbol, double initialPrice)
        : symbol(symbol), price(initialPrice) {}
    
    /**
     * @brief Get the stock's ticker symbol
     * 
     * @return std::string The symbol
     */
    std::string getSymbol() const {
        return symbol;
    }
    
    /**
     * @brief Get the current stock price
     * 
     * @return double The price
     */
    double getPrice() const {
        std::lock_guard<std::mutex> lock(priceMutex);
        return price;
    }
    
    /**
     * @brief Update the stock price and notify observers
     * 
     * @param newPrice The new stock price
     */
    void updatePrice(double newPrice) {
        {
            std::lock_guard<std::mutex> lock(priceMutex);
            
            // Calculate price change percentage
            double changePercent = ((newPrice - price) / price) * 100.0;
            
            // Format the message with price change information
            std::ostringstream message;
            message << std::fixed << std::setprecision(2);
            message << "Price updated from $" << price << " to $" << newPrice;
            message << " (" << (changePercent >= 0 ? "+" : "") << changePercent << "%)";
            
            // Update the price
            price = newPrice;
            
            // Notify observers about the price change
            notifyObservers(message.str());
        }
    }
};

/**
 * @brief Investor who monitors stock price changes
 * 
 * This is a concrete observer that reacts to stock price updates.
 */
class Investor : public Observer {
private:
    std::string name;
    std::unordered_map<std::string, double> purchasePrices;
    std::mutex investorMutex;

public:
    /**
     * @brief Construct a new Investor object
     * 
     * @param name The investor's name
     */
    explicit Investor(const std::string& name) : name(name) {}
    
    /**
     * @brief Get the investor's name
     * 
     * @return std::string The name
     */
    std::string getName() const override {
        return name;
    }
    
    /**
     * @brief Record a stock purchase
     * 
     * @param stockSymbol The stock's ticker symbol
     * @param purchasePrice The price at which the stock was purchased
     */
    void buyStock(const std::string& stockSymbol, double purchasePrice) {
        std::lock_guard<std::mutex> lock(investorMutex);
        purchasePrices[stockSymbol] = purchasePrice;
        std::cout << name << " purchased " << stockSymbol << " at $" 
                  << std::fixed << std::setprecision(2) << purchasePrice << std::endl;
    }
    
    /**
     * @brief React to stock price updates
     * 
     * @param subject The stock that changed price
     * @param message Details about the price change
     */
    void update(Subject* subject, const std::string& message) override {
        Stock* stock = dynamic_cast<Stock*>(subject);
        if (!stock) {
            return;
        }
        
        std::string stockSymbol = stock->getSymbol();
        double currentPrice = stock->getPrice();
        
        std::lock_guard<std::mutex> lock(investorMutex);
        auto it = purchasePrices.find(stockSymbol);
        
        if (it != purchasePrices.end()) {
            double purchasePrice = it->second;
            double profitLoss = currentPrice - purchasePrice;
            double profitLossPercent = (profitLoss / purchasePrice) * 100.0;
            
            std::cout << name << " received update for " << stockSymbol << ": " << message << std::endl;
            std::cout << "  Current P&L: " << (profitLoss >= 0 ? "+" : "") 
                      << std::fixed << std::setprecision(2) << profitLoss 
                      << " (" << (profitLossPercent >= 0 ? "+" : "") 
                      << profitLossPercent << "%)" << std::endl;
                      
            // Make a decision based on the price change
            if (profitLossPercent > 10.0) {
                std::cout << "  Decision: Consider selling " << stockSymbol 
                          << " to lock in profits" << std::endl;
            } else if (profitLossPercent < -5.0) {
                std::cout << "  Decision: Monitor " << stockSymbol 
                          << " closely for further decline" << std::endl;
            }
        } else {
            std::cout << name << " received update for " << stockSymbol 
                      << " (not in portfolio): " << message << std::endl;
        }
    }
};

// ==================================================================================
// Example 2: Distributed System Health Monitoring
// ==================================================================================

/**
 * @brief Represents the status of a service in a distributed system
 */
enum class ServiceStatus {
    HEALTHY,
    DEGRADED,
    DOWN
};

/**
 * @brief Convert ServiceStatus enum to string representation
 * 
 * @param status The service status
 * @return std::string String representation
 */
std::string statusToString(ServiceStatus status) {
    switch (status) {
        case ServiceStatus::HEALTHY: return "HEALTHY";
        case ServiceStatus::DEGRADED: return "DEGRADED";
        case ServiceStatus::DOWN: return "DOWN";
        default: return "UNKNOWN";
    }
}

/**
 * @brief Represents a microservice in a distributed system
 * 
 * This is a concrete subject that maintains service health status
 * and notifies observers when the status changes.
 */
class Microservice : public Subject {
private:
    std::string serviceName;
    ServiceStatus status;
    std::string endpoint;
    mutable std::mutex statusMutex;

public:
    /**
     * @brief Construct a new Microservice object
     * 
     * @param name The service name
     * @param endpoint The service endpoint URL
     */
    Microservice(const std::string& name, const std::string& endpoint)
        : serviceName(name), status(ServiceStatus::HEALTHY), endpoint(endpoint) {}
    
    /**
     * @brief Get the service name
     * 
     * @return std::string The name
     */
    std::string getServiceName() const {
        return serviceName;
    }
    
    /**
     * @brief Get the service endpoint
     * 
     * @return std::string The endpoint URL
     */
    std::string getEndpoint() const {
        return endpoint;
    }
    
    /**
     * @brief Get the current service status
     * 
     * @return ServiceStatus The status
     */
    ServiceStatus getStatus() const {
        std::lock_guard<std::mutex> lock(statusMutex);
        return status;
    }
    
    /**
     * @brief Update the service status and notify observers
     * 
     * @param newStatus The new service status
     * @param details Additional details about the status change
     */
    void updateStatus(ServiceStatus newStatus, const std::string& details) {
        std::lock_guard<std::mutex> lock(statusMutex);
        
        if (newStatus != status) {
            // Format the message with status change information
            std::ostringstream message;
            message << "Status changed from " << statusToString(status)
                    << " to " << statusToString(newStatus);
            
            if (!details.empty()) {
                message << " - Details: " << details;
            }
            
            // Update the status
            status = newStatus;
            
            // Get current timestamp
            auto now = std::chrono::system_clock::now();
            auto now_time_t = std::chrono::system_clock::to_time_t(now);
            std::stringstream timestamp;
            timestamp << std::put_time(std::localtime(&now_time_t), "%Y-%m-%d %H:%M:%S");
            
            // Add timestamp to message
            message << " (at " << timestamp.str() << ")";
            
            // Notify observers about the status change
            notifyObservers(message.str());
        }
    }
};

/**
 * @brief System administrator who monitors service health
 * 
 * This is a concrete observer that reacts to service status updates.
 */
class SystemAdmin : public Observer {
private:
    std::string name;
    bool onCall;

public:
    /**
     * @brief Construct a new SystemAdmin object
     * 
     * @param name The admin's name
     * @param onCall Whether the admin is currently on call
     */
    SystemAdmin(const std::string& name, bool onCall = false)
        : name(name), onCall(onCall) {}
    
    /**
     * @brief Get the admin's name
     * 
     * @return std::string The name
     */
    std::string getName() const override {
        return name;
    }
    
    /**
     * @brief Set whether the admin is on call
     * 
     * @param isOnCall True if the admin is on call
     */
    void setOnCall(bool isOnCall) {
        onCall = isOnCall;
        std::cout << name << " is " << (onCall ? "now" : "no longer") << " on call" << std::endl;
    }
    
    /**
     * @brief React to service status updates
     * 
     * @param subject The service that changed status
     * @param message Details about the status change
     */
    void update(Subject* subject, const std::string& message) override {
        Microservice* service = dynamic_cast<Microservice*>(subject);
        if (!service) {
            return;
        }
        
        std::string serviceName = service->getServiceName();
        ServiceStatus status = service->getStatus();
        
        std::cout << name << " received update for " << serviceName << ": " << message << std::endl;
        
        // Take action based on service status and whether admin is on call
        if (onCall) {
            switch (status) {
                case ServiceStatus::DEGRADED:
                    std::cout << "  Action: " << name << " is investigating performance issues with "
                              << serviceName << " at " << service->getEndpoint() << std::endl;
                    break;
                    
                case ServiceStatus::DOWN:
                    std::cout << "  URGENT Action: " << name << " is initiating recovery procedures for "
                              << serviceName << " and notifying stakeholders" << std::endl;
                    break;
                    
                case ServiceStatus::HEALTHY:
                    std::cout << "  Note: " << name << " confirms " << serviceName 
                              << " is operating normally" << std::endl;
                    break;
            }
        } else {
            if (status == ServiceStatus::DOWN) {
                std::cout << "  Note: " << name << " is aware of " << serviceName 
                          << " outage but is not on call" << std::endl;
            }
        }
    }
};

/**
 * @brief Monitoring dashboard that displays service health
 * 
 * This is another concrete observer that displays status updates.
 */
class MonitoringDashboard : public Observer {
private:
    std::string dashboardName;
    std::unordered_map<std::string, ServiceStatus> serviceStatuses;
    std::mutex dashboardMutex;

public:
    /**
     * @brief Construct a new MonitoringDashboard object
     * 
     * @param name The dashboard name
     */
    explicit MonitoringDashboard(const std::string& name) : dashboardName(name) {}
    
    /**
     * @brief Get the dashboard name
     * 
     * @return std::string The name
     */
    std::string getName() const override {
        return dashboardName;
    }
    
    /**
     * @brief React to service status updates
     * 
     * @param subject The service that changed status
     * @param message Details about the status change
     */
    void update(Subject* subject, const std::string& message) override {
        Microservice* service = dynamic_cast<Microservice*>(subject);
        if (!service) {
            return;
        }
        
        std::string serviceName = service->getServiceName();
        ServiceStatus status = service->getStatus();
        
        {
            std::lock_guard<std::mutex> lock(dashboardMutex);
            serviceStatuses[serviceName] = status;
            
            std::cout << dashboardName << " updated status of " << serviceName 
                      << " to " << statusToString(status) << std::endl;
        }
        
        // Display the current system health summary
        displaySystemHealth();
    }
    
    /**
     * @brief Display the current health status of all services
     */
    void displaySystemHealth() {
        std::lock_guard<std::mutex> lock(dashboardMutex);
        
        int healthy = 0, degraded = 0, down = 0;
        
        for (const auto& pair : serviceStatuses) {
            switch (pair.second) {
                case ServiceStatus::HEALTHY: healthy++; break;
                case ServiceStatus::DEGRADED: degraded++; break;
                case ServiceStatus::DOWN: down++; break;
            }
        }
        
        std::cout << "\n=== " << dashboardName << " System Health Summary ===" << std::endl;
        std::cout << "Total Services: " << serviceStatuses.size() << std::endl;
        std::cout << "  HEALTHY:  " << healthy << std::endl;
        std::cout << "  DEGRADED: " << degraded << std::endl;
        std::cout << "  DOWN:     " << down << std::endl;
        
        // Calculate overall system health percentage
        if (!serviceStatuses.empty()) {
            double healthPercentage = (healthy * 100.0 + degraded * 50.0) / serviceStatuses.size();
            std::cout << "Overall System Health: " << std::fixed << std::setprecision(1) 
                      << healthPercentage << "%" << std::endl;
                      
            // Determine if any alerts should be triggered
            if (down > 0) {
                std::cout << "⚠️ ALERT: " << down << " critical services are DOWN!" << std::endl;
            } else if (degraded > serviceStatuses.size() / 3) {
                std::cout << "⚠️ WARNING: Multiple services are experiencing degraded performance" << std::endl;
            }
        }
        
        std::cout << "=========================================\n" << std::endl;
    }
};

// ==================================================================================
// Main function with demonstration of both examples
// ==================================================================================

/**
 * @brief Demonstrates the Observer pattern with both examples
 * 
 * @return int Exit code
 */
int main() {
    std::cout << "==================================================================" << std::endl;
    std::cout << "                 OBSERVER PATTERN INTERVIEW GUIDE                  " << std::endl;
    std::cout << "==================================================================" << std::endl;
    std::cout << "\nThe Observer pattern defines a one-to-many dependency between objects" << std::endl;
    std::cout << "so that when one object changes state, all its dependents are notified" << std::endl;
    std::cout << "and updated automatically.\n" << std::endl;
    
    std::cout << "Key components:" << std::endl;
    std::cout << "- Subject: Maintains a list of observers and notifies them of changes" << std::endl;
    std::cout << "- Observer: Interface that defines the update method" << std::endl;
    std::cout << "- ConcreteSubject: Implements the Subject interface" << std::endl;
    std::cout << "- ConcreteObserver: Implements the Observer interface\n" << std::endl;
    
    std::cout << "==================================================================" << std::endl;
    std::cout << "EXAMPLE 1: STOCK MARKET PRICE MONITORING SYSTEM" << std::endl;
    std::cout << "==================================================================" << std::endl;
    std::cout << "This example demonstrates how investors can monitor stock prices" << std::endl;
    std::cout << "and receive notifications when prices change.\n" << std::endl;
    
    // Create stock subjects
    auto appleStock = std::make_shared<Stock>("AAPL", 175.50);
    auto googleStock = std::make_shared<Stock>("GOOGL", 2150.75);
    auto amazonStock = std::make_shared<Stock>("AMZN", 3300.25);
    
    // Create investor observers
    auto investor1 = std::make_shared<Investor>("Alice");
    auto investor2 = std::make_shared<Investor>("Bob");
    auto investor3 = std::make_shared<Investor>("Charlie");
    
    // Register investors with stocks they're interested in
    appleStock->registerObserver(investor1);
    appleStock->registerObserver(investor2);
    
    googleStock->registerObserver(investor1);
    googleStock->registerObserver(investor3);
    
    amazonStock->registerObserver(investor2);
    amazonStock->registerObserver(investor3);
    
    // Investors buy stocks
    investor1->buyStock("AAPL", 170.25);
    investor1->buyStock("GOOGL", 2100.50);
    
    investor2->buyStock("AAPL", 165.75);
    investor2->buyStock("AMZN", 3250.00);
    
    investor3->buyStock("GOOGL", 2200.00);
    investor3->buyStock("AMZN", 3275.50);
    
    std::cout << "\n--- Simulating stock price changes ---\n" << std::endl;
    
    // Simulate price changes
    appleStock->updatePrice(180.75);  // Price increase
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    
    googleStock->updatePrice(2050.25);  // Price decrease
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    
    amazonStock->updatePrice(3400.50);  // Price increase
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    
    // Investor 2 stops watching Apple
    appleStock->removeObserver("Bob");
    
    // Another price change
    appleStock->updatePrice(185.25);  // Only Alice will be notified
    
    std::cout << "\n==================================================================" << std::endl;
    std::cout << "EXAMPLE 2: DISTRIBUTED SYSTEM HEALTH MONITORING" << std::endl;
    std::cout << "==================================================================" << std::endl;
    std::cout << "This example demonstrates how system administrators and dashboards" << std::endl;
    std::cout << "can monitor the health of microservices in a distributed system.\n" << std::endl;
    
    // Create microservice subjects
    auto authService = std::make_shared<Microservice>("Authentication Service", "https://auth.example.com");
    auto paymentService = std::make_shared<Microservice>("Payment Processing", "https://payments.example.com");
    auto databaseService = std::make_shared<Microservice>("Database Cluster", "db.example.com:5432");
    auto apiGateway = std::make_shared<Microservice>("API Gateway", "https://api.example.com");
    
    // Create system admin observers
    auto admin1 = std::make_shared<SystemAdmin>("Dave", true);  // On call
    auto admin2 = std::make_shared<SystemAdmin>("Eve", false);  // Not on call
    
    // Create monitoring dashboard
    auto dashboard = std::make_shared<MonitoringDashboard>("Main Operations Dashboard");
    
    // Register observers with services
    authService->registerObserver(admin1);
    authService->registerObserver(admin2);
    authService->registerObserver(dashboard);
    
    paymentService->registerObserver(admin1);
    paymentService->registerObserver(dashboard);
    
    databaseService->registerObserver(admin1);
    databaseService->registerObserver(admin2);
    databaseService->registerObserver(dashboard);
    
    apiGateway->registerObserver(admin1);
    apiGateway->registerObserver(admin2);
    apiGateway->registerObserver(dashboard);
    
    std::cout << "\n--- Simulating service status changes ---\n" << std::endl;
    
    // Simulate status changes
    paymentService->updateStatus(ServiceStatus::DEGRADED, "High latency detected");
    std::this_thread::sleep_for(std::chrono::milliseconds(800));
    
    databaseService->updateStatus(ServiceStatus::DOWN, "Connection timeout");
    std::this_thread::sleep_for(std::chrono::milliseconds(800));
    
    // Admin 2 goes on call
    admin2->setOnCall(true);
    
    // More status changes
    authService->updateStatus(ServiceStatus::DEGRADED, "Increased authentication failures");
    std::this_thread::sleep_for(std::chrono::milliseconds(800));
    
    // Service recovery
    databaseService->updateStatus(ServiceStatus::HEALTHY, "Failover completed successfully");
    std::this_thread::sleep_for(std::chrono::milliseconds(800));
    
    paymentService->updateStatus(ServiceStatus::HEALTHY, "Performance issues resolved");
    std::this_thread::sleep_for(std::chrono::milliseconds(800));
    
    authService->updateStatus(ServiceStatus::HEALTHY, "Normal operation resumed");
    
    std::cout << "\n==================================================================" << std::endl;
    std::cout << "                  INTERVIEW DISCUSSION POINTS                      " << std::endl;
    std::cout << "==================================================================" << std::endl;
    
    std::cout << "\n1. When to use the Observer pattern:" << std::endl;
    std::cout << "   - When changes to one object require changing others, and you don't" << std::endl;
    std::cout << "     know how many objects need to change" << std::endl;
    std::cout << "   - When an object should be able to notify other objects without" << std::endl;
    std::cout << "     making assumptions about who these objects are" << std::endl;
    std::cout << "   - When you need to maintain loose coupling between related objects" << std::endl;
    
    std::cout << "\n2. Real-world applications:" << std::endl;
    std::cout << "   - Event handling systems in user interfaces" << std::endl;
    std::cout << "   - Stock market monitoring applications" << std::endl;
    std::cout << "   - Distributed system health monitoring" << std::endl;
    std::cout << "   - Pub/sub messaging systems" << std::endl;
    std::cout << "   - Model-View-Controller (MVC) architectures" << std::endl;
    
    std::cout << "\n3. Benefits:" << std::endl;
    std::cout << "   - Loose coupling between subjects and observers" << std::endl;
    std::cout << "   - Support for broadcast communication" << std::endl;
    std::cout << "   - Dynamic relationships between objects at runtime" << std::endl;
    
    std::cout << "\n4. Drawbacks:" << std::endl;
    std::cout << "   - Unexpected updates if observers are unaware of each other" << std::endl;
    std::cout << "   - Memory leaks if observers aren't properly unregistered" << std::endl;
    std::cout << "   - Performance concerns with many observers or frequent updates" << std::endl;
    
    std::cout << "\n5. Implementation considerations:" << std::endl;
    std::cout << "   - Thread safety for concurrent applications" << std::endl;
    std::cout << "   - Memory management (using smart pointers)" << std::endl;
    std::cout << "   - Push vs. pull update models" << std::endl;
    std::cout << "   - Order of notification (if it matters)" << std::endl;
    
    return 0;
}
