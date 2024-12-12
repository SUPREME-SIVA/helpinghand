#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <fstream>
#include <stdexcept>

// FileHandler class to manage file operations
class FileHandler {
private:
    std::string filename;
    std::unique_ptr<std::fstream> file;

public:
    FileHandler(const std::string& fname) : filename(fname) {
        std::cout << "Opening file: " << filename << std::endl;
        file = std::make_unique<std::fstream>();
        file->open(filename, std::ios::in | std::ios::out | std::ios::app);
        
        if (!file->is_open()) {
            throw std::runtime_error("Failed to open file: " + filename);
        }
    }

    ~FileHandler() {
        if (file && file->is_open()) {
            file->close();
            std::cout << "Closing file: " << filename << std::endl;
        }
    }

    // Write data to file
    void writeData(const std::string& data) {
        if (file && file->is_open()) {
            *file << data << std::endl;
            file->flush();
        }
    }

    // Read all lines from file
    std::vector<std::string> readAllLines() {
        std::vector<std::string> lines;
        if (file && file->is_open()) {
            file->seekg(0); // Go to start of file
            std::string line;
            while (std::getline(*file, line)) {
                lines.push_back(line);
            }
        }
        return lines;
    }
};

// LogManager class to manage multiple log files
class LogManager {
private:
    std::vector<std::unique_ptr<FileHandler>> logFiles;

public:
    // Add a new log file
    void addLogFile(const std::string& filename) {
        try {
            logFiles.push_back(std::make_unique<FileHandler>(filename));
            std::cout << "Added new log file: " << filename << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "Error adding log file: " << e.what() << std::endl;
        }
    }

    // Write to all log files
    void writeToAllLogs(const std::string& message) {
        for (const auto& log : logFiles) {
            log->writeData(message);
        }
    }

    // Read from a specific log file (by index)
    std::vector<std::string> readFromLog(size_t index) {
        if (index >= logFiles.size()) {
            throw std::out_of_range("Invalid log file index");
        }
        return logFiles[index]->readAllLines();
    }
};

// Example usage with error handling
int main() {
    try {
        // Create a log manager
        LogManager logManager;

        // Add multiple log files
        logManager.addLogFile("system.log");
        logManager.addLogFile("error.log");
        logManager.addLogFile("debug.log");

        // Write some sample logs
        std::cout << "\nWriting logs...\n";
        logManager.writeToAllLogs("System startup - " + std::string(__DATE__));
        logManager.writeToAllLogs("Initializing components...");
        logManager.writeToAllLogs("System ready");

        // Read and display logs from the first file
        std::cout << "\nReading from system.log:\n";
        try {
            auto lines = logManager.readFromLog(0);
            std::cout << "Log contents:\n";
            for (const auto& line : lines) {
                std::cout << line << std::endl;
            }
        } catch (const std::exception& e) {
            std::cerr << "Error reading log: " << e.what() << std::endl;
        }

    } catch (const std::exception& e) {
        std::cerr << "Program error: " << e.what() << std::endl;
        return 1;
    }

    std::cout << "\nProgram completed successfully\n";
    return 0;
}
