#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <fstream>
#include <chrono>
#include <sstream>

// Subtle violation of Interface Segregation Principle
// This interface is too fat - clients might not need all methods
class LoggerBad {
public:
    virtual void logError(const std::string& message) = 0;
    virtual void logWarning(const std::string& message) = 0;
    virtual void logInfo(const std::string& message) = 0;
    virtual void logDebug(const std::string& message) = 0;
    virtual void setFormat(const std::string& format) = 0;
    virtual void setOutput(const std::string& output) = 0;
    virtual void rotate() = 0;
    virtual void compress() = 0;
    virtual ~LoggerBad() = default;
};

// Better approach: Interface Segregation with Role Interfaces
// 1. Basic Logging Interface
class ILogger {
public:
    virtual void log(const std::string& message, const std::string& level) = 0;
    virtual ~ILogger() = default;
};

// 2. Format Configuration Interface
class IFormattable {
public:
    virtual void setFormat(const std::string& format) = 0;
    virtual ~IFormattable() = default;
};

// 3. Output Configuration Interface
class IOutputConfigurable {
public:
    virtual void setOutput(const std::string& output) = 0;
    virtual ~IOutputConfigurable() = default;
};

// 4. Log Maintenance Interface
class ILogMaintenance {
public:
    virtual void rotate() = 0;
    virtual void compress() = 0;
    virtual ~ILogMaintenance() = default;
};

// Advanced application of Single Responsibility Principle
// Separate formatting logic into its own class
class LogFormatter {
public:
    static std::string format(const std::string& message, 
                            const std::string& level, 
                            const std::string& format) {
        std::stringstream ss;
        auto now = std::chrono::system_clock::now();
        auto time = std::chrono::system_clock::to_time_t(now);
        
        if (format == "json") {
            ss << "{\"timestamp\":\"" << std::ctime(&time) << "\","
               << "\"level\":\"" << level << "\","
               << "\"message\":\"" << message << "\"}\n";
        } else {
            ss << "[" << std::ctime(&time) << "] "
               << "[" << level << "] "
               << message << "\n";
        }
        return ss.str();
    }
};

// Advanced application of Open-Closed Principle with Template Method Pattern
class BaseLogger : public ILogger, public IFormattable {
public:
    void log(const std::string& message, const std::string& level) override final {
        // Template method pattern
        std::string formattedMessage = preProcess(message, level);
        writeLog(formattedMessage);
        postProcess();
    }

    void setFormat(const std::string& fmt) override {
        format = fmt;
    }

protected:
    virtual std::string preProcess(const std::string& message, const std::string& level) {
        return LogFormatter::format(message, level, format);
    }
    
    virtual void writeLog(const std::string& message) = 0;
    
    virtual void postProcess() {} // Hook method

    std::string format = "standard";
};

// Concrete implementation showing Liskov Substitution Principle
class FileLogger : public BaseLogger, public IOutputConfigurable, public ILogMaintenance {
public:
    explicit FileLogger(const std::string& filename) : outputFile(filename) {}

    void setOutput(const std::string& output) override {
        outputFile = output;
    }

    void rotate() override {
        // Implement log rotation
        std::cout << "Rotating logs for " << outputFile << std::endl;
    }

    void compress() override {
        // Implement log compression
        std::cout << "Compressing logs for " << outputFile << std::endl;
    }

protected:
    void writeLog(const std::string& message) override {
        std::ofstream file(outputFile, std::ios::app);
        if (file) {
            file << message;
        }
    }

    void postProcess() override {
        // Additional file-specific post-processing
        checkFileSize();
    }

private:
    void checkFileSize() {
        // Check if log rotation is needed
        std::ifstream file(outputFile, std::ios::ate);
        if (file.tellg() > 1024 * 1024) { // 1MB
            rotate();
        }
    }

    std::string outputFile;
};

// Dependency Inversion Principle with Factory Pattern
class LoggerFactory {
public:
    static std::unique_ptr<ILogger> createLogger(const std::string& type) {
        if (type == "file") {
            return std::make_unique<FileLogger>("app.log");
        }
        // Can easily add more logger types without modifying existing code
        throw std::runtime_error("Unknown logger type");
    }
};

// Advanced composition over inheritance example
class LoggingSystem {
public:
    explicit LoggingSystem(std::unique_ptr<ILogger> logger) 
        : logger(std::move(logger)) {}

    void logMessage(const std::string& message, const std::string& level) {
        if (shouldLog(level)) {
            logger->log(message, level);
            notifySubscribers(level);
        }
    }

    void addLogSubscriber(const std::function<void(const std::string&)>& subscriber) {
        subscribers.push_back(subscriber);
    }

private:
    bool shouldLog(const std::string& level) {
        // Implement logging logic based on configuration
        return true; // Simplified
    }

    void notifySubscribers(const std::string& level) {
        for (const auto& subscriber : subscribers) {
            subscriber(level);
        }
    }

    std::unique_ptr<ILogger> logger;
    std::vector<std::function<void(const std::string&)>> subscribers;
};

int main() {
    // Example usage of advanced SOLID principles
    try {
        auto logger = LoggerFactory::createLogger("file");
        LoggingSystem loggingSystem(std::move(logger));

        // Add a subscriber for monitoring critical logs
        loggingSystem.addLogSubscriber([](const std::string& level) {
            if (level == "ERROR") {
                std::cout << "Alert: Error detected!" << std::endl;
            }
        });

        // Log some messages
        loggingSystem.logMessage("Application started", "INFO");
        loggingSystem.logMessage("Invalid input detected", "ERROR");

        // Demonstrate specific logger capabilities
        if (auto* fileLogger = dynamic_cast<FileLogger*>(logger.get())) {
            fileLogger->setFormat("json");
            fileLogger->rotate();
            fileLogger->compress();
        }

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
