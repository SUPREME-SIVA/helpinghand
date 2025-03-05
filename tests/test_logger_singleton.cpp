#include <iostream>
#include <fstream>
#include <string>
#include <mutex>
#include <stdexcept>
#include <chrono>
#include <cassert>
#include <sstream>
#include <filesystem>

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

// Logger class from logger_singleton.cpp
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

    bool isInitialized() const {
        return initialized;
    }

    void reset() {
        std::lock_guard<std::mutex> lock(mutex_);
        if (log_file.is_open()) {
            log_file.close();
        }
        initialized = false;
    }

    static void resetInstance() {
        std::lock_guard<std::mutex> lock(mutex_);
        delete instance;
        instance = nullptr;
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

// Test functions
void test_singleton_pattern() {
    std::cout << "Testing singleton pattern..." << std::endl;
    
    // Get two instances and verify they are the same
    Logger* logger1 = Logger::getInstance();
    Logger* logger2 = Logger::getInstance();
    
    assert(logger1 == logger2);
    assert(logger1 != nullptr);
    
    std::cout << "Singleton pattern test passed!" << std::endl;
}

void test_logger_initialization() {
    std::cout << "Testing logger initialization..." << std::endl;
    
    // Reset the logger instance for this test
    Logger::resetInstance();
    
    Logger* logger = Logger::getInstance();
    assert(!logger->isInitialized());
    
    // Initialize the logger
    std::string test_log_file = "test_log.txt";
    logger->init(test_log_file);
    assert(logger->isInitialized());
    
    // Clean up
    logger->reset();
    if (std::filesystem::exists(test_log_file)) {
        std::filesystem::remove(test_log_file);
    }
    
    std::cout << "Logger initialization test passed!" << std::endl;
}

void test_double_initialization() {
    std::cout << "Testing double initialization..." << std::endl;
    
    // Reset the logger instance for this test
    Logger::resetInstance();
    
    Logger* logger = Logger::getInstance();
    std::string test_log_file = "test_log.txt";
    logger->init(test_log_file);
    
    // Try to initialize again, should throw exception
    bool exception_thrown = false;
    try {
        logger->init("another_log.txt");
    } catch (const std::runtime_error& e) {
        exception_thrown = true;
        assert(std::string(e.what()) == "Logger already initialized!");
    }
    
    assert(exception_thrown);
    
    // Clean up
    logger->reset();
    if (std::filesystem::exists(test_log_file)) {
        std::filesystem::remove(test_log_file);
    }
    
    std::cout << "Double initialization test passed!" << std::endl;
}

void test_logging() {
    std::cout << "Testing logging functionality..." << std::endl;
    
    // Reset the logger instance for this test
    Logger::resetInstance();
    
    Logger* logger = Logger::getInstance();
    std::string test_log_file = "test_log.txt";
    logger->init(test_log_file);
    
    // Log some messages
    logger->log("Test message 1");
    logger->log("Test message 2");
    
    // Close the log file
    logger->reset();
    
    // Verify the log file exists and contains the messages
    std::ifstream log_reader(test_log_file);
    assert(log_reader.good());
    
    std::string line;
    int message_count = 0;
    while (std::getline(log_reader, line)) {
        if (line.find("Test message 1") != std::string::npos) {
            message_count++;
        }
        if (line.find("Test message 2") != std::string::npos) {
            message_count++;
        }
    }
    
    assert(message_count == 2);
    
    // Clean up
    log_reader.close();
    if (std::filesystem::exists(test_log_file)) {
        std::filesystem::remove(test_log_file);
    }
    
    std::cout << "Logging functionality test passed!" << std::endl;
}

void test_uninitialized_logging() {
    std::cout << "Testing uninitialized logging..." << std::endl;
    
    // Reset the logger instance for this test
    Logger::resetInstance();
    
    Logger* logger = Logger::getInstance();
    
    // Try to log without initializing, should throw exception
    bool exception_thrown = false;
    try {
        logger->log("This should fail");
    } catch (const std::runtime_error& e) {
        exception_thrown = true;
        assert(std::string(e.what()) == "Logger not initialized!");
    }
    
    assert(exception_thrown);
    
    std::cout << "Uninitialized logging test passed!" << std::endl;
}

int main() {
    std::cout << "Running logger_singleton tests..." << std::endl;
    
    test_singleton_pattern();
    test_logger_initialization();
    test_double_initialization();
    test_logging();
    test_uninitialized_logging();
    
    // Final cleanup
    Logger::resetInstance();
    
    std::cout << "All logger_singleton tests passed!" << std::endl;
    return 0;
}
