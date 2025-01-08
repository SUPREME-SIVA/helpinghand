#include <iostream>
#include <memory>
#include <string>
#include <stdexcept>

// Abstract Payment Processor
class PaymentProcessor {
public:
    virtual void processPayment(double amount) = 0;
    virtual std::string getProcessorName() const = 0;
    virtual ~PaymentProcessor() = default;
};

// Concrete Payment Processors
class CreditCardProcessor : public PaymentProcessor {
public:
    void processPayment(double amount) override {
        std::cout << "Processing credit card payment of $" << amount << std::endl;
        std::cout << "Validating card details...\n";
        std::cout << "Payment completed via Credit Card\n";
    }

    std::string getProcessorName() const override {
        return "Credit Card";
    }
};

class PayPalProcessor : public PaymentProcessor {
public:
    void processPayment(double amount) override {
        std::cout << "Processing PayPal payment of $" << amount << std::endl;
        std::cout << "Redirecting to PayPal...\n";
        std::cout << "Payment completed via PayPal\n";
    }

    std::string getProcessorName() const override {
        return "PayPal";
    }
};

class CryptoProcessor : public PaymentProcessor {
public:
    void processPayment(double amount) override {
        std::cout << "Processing cryptocurrency payment of $" << amount << std::endl;
        std::cout << "Generating wallet address...\n";
        std::cout << "Payment completed via Cryptocurrency\n";
    }

    std::string getProcessorName() const override {
        return "Cryptocurrency";
    }
};

// Payment Processor Factory
class PaymentProcessorFactory {
public:
    static std::unique_ptr<PaymentProcessor> createProcessor(const std::string& type) {
        if (type == "credit_card") {
            return std::make_unique<CreditCardProcessor>();
        } else if (type == "paypal") {
            return std::make_unique<PayPalProcessor>();
        } else if (type == "crypto") {
            return std::make_unique<CryptoProcessor>();
        }
        throw std::runtime_error("Unknown payment processor type: " + type);
    }
};

int main() {
    try {
        // Process payments using different processors
        auto creditCardProcessor = PaymentProcessorFactory::createProcessor("credit_card");
        auto paypalProcessor = PaymentProcessorFactory::createProcessor("paypal");
        auto cryptoProcessor = PaymentProcessorFactory::createProcessor("crypto");

        std::cout << "Using " << creditCardProcessor->getProcessorName() << ":\n";
        creditCardProcessor->processPayment(100.50);

        std::cout << "\nUsing " << paypalProcessor->getProcessorName() << ":\n";
        paypalProcessor->processPayment(75.25);

        std::cout << "\nUsing " << cryptoProcessor->getProcessorName() << ":\n";
        cryptoProcessor->processPayment(500.00);

        // Try to create an invalid processor
        try {
            auto invalidProcessor = PaymentProcessorFactory::createProcessor("invalid");
        } catch (const std::runtime_error& e) {
            std::cout << "\nExpected error: " << e.what() << std::endl;
        }

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
