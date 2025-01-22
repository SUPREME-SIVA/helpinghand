#include <iostream>
#include <string>
#include <vector>
#include <memory>

// BAD DESIGN - Violates SOLID principles
class NotificationServiceBad {
public:
    void sendNotification(const std::string& message, const std::string& type) {
        if (type == "email") {
            std::cout << "Sending email: " << message << std::endl;
            // Email-specific logic here
        }
        else if (type == "sms") {
            std::cout << "Sending SMS: " << message << std::endl;
            // SMS-specific logic here
        }
        // Adding new notification type requires modifying this class
    }
};

// GOOD DESIGN - Follows SOLID principles

// Single Responsibility Principle: Each class has one responsibility
// Open-Closed Principle: Open for extension, closed for modification
// Interface Segregation: Clients aren't forced to depend on methods they don't use
// Dependency Inversion: High-level modules depend on abstractions

// Abstract base class for notification
class NotificationService {
public:
    virtual void sendNotification(const std::string& message) = 0;
    virtual ~NotificationService() = default;
};

// Concrete implementation for email notifications
class EmailNotification : public NotificationService {
public:
    void sendNotification(const std::string& message) override {
        connectSMTP();
        std::cout << "Sending email: " << message << std::endl;
    }

private:
    void connectSMTP() {
        std::cout << "Connecting to SMTP server..." << std::endl;
    }
};

// Concrete implementation for SMS notifications
class SMSNotification : public NotificationService {
public:
    void sendNotification(const std::string& message) override {
        connectSMSGateway();
        std::cout << "Sending SMS: " << message << std::endl;
    }

private:
    void connectSMSGateway() {
        std::cout << "Connecting to SMS gateway..." << std::endl;
    }
};

// New notification type can be added without modifying existing code
class PushNotification : public NotificationService {
public:
    void sendNotification(const std::string& message) override {
        initializePushService();
        std::cout << "Sending push notification: " << message << std::endl;
    }

private:
    void initializePushService() {
        std::cout << "Initializing push notification service..." << std::endl;
    }
};

// Notification manager that follows OCP
class NotificationManager {
public:
    void addNotificationService(std::shared_ptr<NotificationService> service) {
        services.push_back(service);
    }

    void notify(const std::string& message) {
        for (const auto& service : services) {
            service->sendNotification(message);
        }
    }

private:
    std::vector<std::shared_ptr<NotificationService>> services;
};

int main() {
    // Bad design example
    std::cout << "Bad Design Example:\n";
    NotificationServiceBad badService;
    badService.sendNotification("Hello World!", "email");
    badService.sendNotification("Hello World!", "sms");
    std::cout << "\n";

    // Good design example
    std::cout << "Good Design Example:\n";
    NotificationManager notificationManager;
    
    notificationManager.addNotificationService(std::make_shared<EmailNotification>());
    notificationManager.addNotificationService(std::make_shared<SMSNotification>());
    notificationManager.addNotificationService(std::make_shared<PushNotification>());
    
    notificationManager.notify("Hello World!");
    
    return 0;
}
