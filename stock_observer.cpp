#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <algorithm>

// Observer interface
class StockObserver {
public:
    virtual void update(const std::string& symbol, double price) = 0;
    virtual ~StockObserver() = default;
};

// Concrete Observers
class InvestorObserver : public StockObserver {
    std::string name;
    double buyThreshold;
    double sellThreshold;

public:
    InvestorObserver(std::string investor_name, double buy_at, double sell_at)
        : name(std::move(investor_name)), buyThreshold(buy_at), sellThreshold(sell_at) {}

    void update(const std::string& symbol, double price) override {
        if (price <= buyThreshold) {
            std::cout << name << ": Time to buy " << symbol << " at $" << price << std::endl;
        } else if (price >= sellThreshold) {
            std::cout << name << ": Time to sell " << symbol << " at $" << price << std::endl;
        } else {
            std::cout << name << ": Monitoring " << symbol << " at $" << price << std::endl;
        }
    }
};

class AlertObserver : public StockObserver {
    double alertThreshold;

public:
    explicit AlertObserver(double threshold) : alertThreshold(threshold) {}

    void update(const std::string& symbol, double price) override {
        if (price > alertThreshold) {
            std::cout << "ALERT: " << symbol << " has exceeded $" << alertThreshold 
                      << " (Current: $" << price << ")" << std::endl;
        }
    }
};

// Subject (Observable)
class StockMarket {
    std::vector<std::weak_ptr<StockObserver>> observers;
    std::string symbol;
    double price;

public:
    explicit StockMarket(std::string stock_symbol) 
        : symbol(std::move(stock_symbol)), price(0.0) {}

    void attach(const std::shared_ptr<StockObserver>& observer) {
        observers.push_back(observer);
        std::cout << "Added new observer for " << symbol << std::endl;
    }

    void setPrice(double new_price) {
        if (price != new_price) {
            price = new_price;
            notifyObservers();
        }
    }

private:
    void notifyObservers() {
        // Remove expired observers
        observers.erase(
            std::remove_if(observers.begin(), observers.end(),
                [](const std::weak_ptr<StockObserver>& obs) { return obs.expired(); }),
            observers.end()
        );

        // Notify active observers
        for (const auto& weak_obs : observers) {
            if (auto obs = weak_obs.lock()) {
                obs->update(symbol, price);
            }
        }
    }
};

int main() {
    // Create stock market
    StockMarket tesla("TSLA");

    // Create observers
    auto investor1 = std::make_shared<InvestorObserver>("John", 150.0, 200.0);
    auto investor2 = std::make_shared<InvestorObserver>("Alice", 160.0, 210.0);
    auto alertSystem = std::make_shared<AlertObserver>(195.0);

    // Register observers
    tesla.attach(investor1);
    tesla.attach(investor2);
    tesla.attach(alertSystem);

    // Simulate price changes
    std::cout << "\nSimulating price changes:\n";
    tesla.setPrice(155.0);
    std::cout << "\n";
    tesla.setPrice(180.0);
    std::cout << "\n";
    tesla.setPrice(198.0);

    // Let one investor go out of scope
    investor1.reset();
    
    std::cout << "\nAfter investor1 leaves:\n";
    tesla.setPrice(205.0);

    return 0;
}
