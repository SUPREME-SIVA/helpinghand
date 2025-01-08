#include <iostream>
#include <fstream>
#include <string>
#include <mutex>
#include <stdexcept>
#include <chrono>

class Logger {
private:
    static Logger* instance;
    static std::mutex mutex_;
    std::ofstream log_file;
    bool initialized;

    // Private constructor to prevent direct instantiation
    Logger() : initialized(false) {}

    // Delete copy constructor and assignment operator
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

public:
    static Logger* getInstance() {
        std::lock_guard<std::mutex> lock(mutex_);
        if (instance == nullptr) {
            instance = new Logger();
        }
        return instance;
    }

    void init(const std::string& filename) {
        std::lock_guard<std::mutex> lock(mutex_);
        if (initialized) {
            throw std::runtime_error("Logger already initialized!");
        }
        log_file.open(filename, std::ios::app);
        initialized = true;
    }

    void log(const std::string& message) {
        std::lock_guard<std::mutex> lock(mutex_);
        if (!initialized) {
            throw std::runtime_error("Logger not initialized!");
        }
        auto now = std::chrono::system_clock::now();
        auto time = std::chrono::system_clock::to_time_t(now);
        log_file << std::ctime(&time) << message << std::endl;
    }

    ~Logger() {
        if (log_file.is_open()) {
            log_file.close();
        }
    }
};

// Initialize static members
Logger* Logger::instance = nullptr;
std::mutex Logger::mutex_;

int main() {
    try {
        // Get logger instance and initialize
        Logger* logger1 = Logger::getInstance();
        logger1->init("app.log");

        // Try to initialize again (should throw error)
        try {
            logger1->init("another.log");
        } catch (const std::runtime_error& e) {
            std::cout << "Expected error: " << e.what() << std::endl;
        }

        // Log some messages
        logger1->log("Application started");
        logger1->log("Processing data...");

        // Try to create another instance (will return same instance)
        Logger* logger2 = Logger::getInstance();
        logger2->log("Another log message");

        if (logger1 == logger2) {
            std::cout << "Both loggers are the same instance!" << std::endl;
        }

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
