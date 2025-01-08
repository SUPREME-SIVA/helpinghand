#include <iostream>
#include <unordered_map>
#include <string>
#include <mutex>
#include <memory>
#include <stdexcept>

class DatabaseConnection {
private:
    static std::unique_ptr<DatabaseConnection> instance;
    static std::mutex mutex_;
    std::unordered_map<std::string, std::string> data;
    bool connected;

    // Private constructor
    DatabaseConnection() : connected(false) {}

    // Delete copy constructor and assignment operator
    DatabaseConnection(const DatabaseConnection&) = delete;
    DatabaseConnection& operator=(const DatabaseConnection&) = delete;

public:
    static DatabaseConnection& getInstance() {
        std::lock_guard<std::mutex> lock(mutex_);
        if (!instance) {
            instance.reset(new DatabaseConnection());
        }
        return *instance;
    }

    void connect(const std::string& connectionString) {
        std::lock_guard<std::mutex> lock(mutex_);
        if (connected) {
            throw std::runtime_error("Database already connected!");
        }
        // Simulate database connection
        std::cout << "Connecting to database: " << connectionString << std::endl;
        connected = true;
    }

    void insert(const std::string& key, const std::string& value) {
        std::lock_guard<std::mutex> lock(mutex_);
        if (!connected) {
            throw std::runtime_error("Database not connected!");
        }
        data[key] = value;
        std::cout << "Inserted: " << key << " = " << value << std::endl;
    }

    std::string query(const std::string& key) {
        std::lock_guard<std::mutex> lock(mutex_);
        if (!connected) {
            throw std::runtime_error("Database not connected!");
        }
        auto it = data.find(key);
        if (it != data.end()) {
            return it->second;
        }
        throw std::runtime_error("Key not found: " + key);
    }
};

// Initialize static members
std::unique_ptr<DatabaseConnection> DatabaseConnection::instance;
std::mutex DatabaseConnection::mutex_;

int main() {
    try {
        // Get database instance and connect
        DatabaseConnection& db1 = DatabaseConnection::getInstance();
        db1.connect("mysql://localhost:3306/mydb");

        // Try to connect again (should throw error)
        try {
            db1.connect("mysql://localhost:3306/anotherdb");
        } catch (const std::runtime_error& e) {
            std::cout << "Expected error: " << e.what() << std::endl;
        }

        // Insert some data
        db1.insert("user1", "John Doe");
        db1.insert("user2", "Jane Smith");

        // Get another instance (will be the same instance)
        DatabaseConnection& db2 = DatabaseConnection::getInstance();
        
        // Query data using second instance
        std::cout << "Query result: " << db2.query("user1") << std::endl;

        // Verify it's the same instance
        if (&db1 == &db2) {
            std::cout << "Both database connections are the same instance!" << std::endl;
        }

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
