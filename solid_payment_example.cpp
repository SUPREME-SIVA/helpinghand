#include <iostream>
#include <string>
#include <vector>
#include <memory>

// BAD DESIGN - Violates Open-Closed Principle
class PaymentProcessorBad {
public:
    void processPayment(const std::string& paymentType, double amount) {
        if (paymentType == "CreditCard") {
            processCreditCard(amount);
        } 
        else if (paymentType == "PayPal") {
            processPayPal(amount);
        }
        // Adding new payment method requires modifying existing code
        // else if (paymentType == "Crypto") { ... }
    }

private:
    void processCreditCard(double amount) {
        std::cout << "Processing credit card payment of $" << amount << std::endl;
    }

    void processPayPal(double amount) {
        std::cout << "Processing PayPal payment of $" << amount << std::endl;
    }
};

// GOOD DESIGN - Follows Open-Closed Principle
// Abstract base class for payment processing
class PaymentProcessor {
public:
    virtual void processPayment(double amount) = 0;
    virtual ~PaymentProcessor() = default;
};

// Concrete implementation for credit card
class CreditCardProcessor : public PaymentProcessor {
public:
    void processPayment(double amount) override {
        std::cout << "Processing credit card payment of $" << amount << std::endl;
        validateCard();
        processTransaction();
    }

private:
    void validateCard() {
        std::cout << "Validating credit card..." << std::endl;
    }

    void processTransaction() {
        std::cout << "Processing credit card transaction..." << std::endl;
    }
};

// Concrete implementation for PayPal
class PayPalProcessor : public PaymentProcessor {
public:
    void processPayment(double amount) override {
        std::cout << "Processing PayPal payment of $" << amount << std::endl;
        authenticatePayPal();
        processTransaction();
    }

private:
    void authenticatePayPal() {
        std::cout << "Authenticating PayPal account..." << std::endl;
    }

    void processTransaction() {
        std::cout << "Processing PayPal transaction..." << std::endl;
    }
};

// New payment method can be added without modifying existing code
class CryptoProcessor : public PaymentProcessor {
public:
    void processPayment(double amount) override {
        std::cout << "Processing cryptocurrency payment of $" << amount << std::endl;
        validateWallet();
        processTransaction();
    }

private:
    void validateWallet() {
        std::cout << "Validating crypto wallet..." << std::endl;
    }

    void processTransaction() {
        std::cout << "Processing crypto transaction..." << std::endl;
    }
};

// Payment handler that follows OCP
class PaymentHandler {
public:
    void addPaymentProcessor(std::shared_ptr<PaymentProcessor> processor) {
        processors.push_back(processor);
    }

    void processPayments(double amount) {
        for (const auto& processor : processors) {
            processor->processPayment(amount);
        }
    }

private:
    std::vector<std::shared_ptr<PaymentProcessor>> processors;
};

int main() {
    // Bad design example
    std::cout << "Bad Design Example:\n";
    PaymentProcessorBad badProcessor;
    badProcessor.processPayment("CreditCard", 100.0);
    badProcessor.processPayment("PayPal", 50.0);
    std::cout << "\n";

    // Good design example
    std::cout << "Good Design Example:\n";
    PaymentHandler paymentHandler;
    
    paymentHandler.addPaymentProcessor(std::make_shared<CreditCardProcessor>());
    paymentHandler.addPaymentProcessor(std::make_shared<PayPalProcessor>());
    paymentHandler.addPaymentProcessor(std::make_shared<CryptoProcessor>());
    
    paymentHandler.processPayments(75.0);
    
    return 0;
}
