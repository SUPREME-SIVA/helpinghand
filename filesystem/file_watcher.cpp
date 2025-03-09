#include <iostream>
#include <filesystem>
#include <chrono>
#include <thread>
#include <map>
#include <string>
#include <functional>
#include <atomic>
#include <mutex>
#include <condition_variable>
#include <unordered_set>

namespace fs = std::filesystem;

/**
 * @brief File Watcher Implementation
 * 
 * This program implements a simple file watching system that monitors a directory
 * for changes (file creation, modification, deletion) and triggers callbacks when
 * changes are detected. It uses the C++17 filesystem library and demonstrates
 * polling-based file monitoring.
 * 
 * Interview Topics:
 * - Real-time file system monitoring techniques
 * - Thread-safety in filesystem operations
 * - Event-based programming with callbacks
 * - Performance considerations for file system watchers
 */

class FileWatcher {
public:
    // Define callback function type
    using Callback = std::function<void(const fs::path&, const std::string&)>;
    
    /**
     * Constructor
     * 
     * @param path Directory to watch
     * @param interval_ms Polling interval in milliseconds
     */
    FileWatcher(const fs::path& path, std::chrono::milliseconds interval_ms = std::chrono::milliseconds(1000))
        : watch_path_(path), interval_(interval_ms), running_(false) {
        if (!fs::exists(path)) {
            throw std::runtime_error("Watch path does not exist: " + path.string());
        }
        
        if (!fs::is_directory(path)) {
            throw std::runtime_error("Watch path is not a directory: " + path.string());
        }
    }
    
    /**
     * Start the file watcher in a separate thread
     */
    void start() {
        std::unique_lock<std::mutex> lock(mutex_);
        if (running_) {
            return; // Already running
        }
        
        running_ = true;
        lock.unlock();
        
        // Start the watcher thread
        watcher_thread_ = std::thread(&FileWatcher::watch, this);
    }
    
    /**
     * Stop the file watcher
     */
    void stop() {
        {
            std::lock_guard<std::mutex> lock(mutex_);
            running_ = false;
        }
        
        // Notify the condition variable to wake up the thread
        cv_.notify_all();
        
        // Wait for the thread to finish
        if (watcher_thread_.joinable()) {
            watcher_thread_.join();
        }
    }
    
    /**
     * Add a callback function for a specific event type
     * 
     * @param event_type Event type: "created", "modified", "deleted"
     * @param callback Function to call when event occurs
     */
    void add_callback(const std::string& event_type, Callback callback) {
        std::lock_guard<std::mutex> lock(mutex_);
        callbacks_[event_type].push_back(callback);
    }
    
    /**
     * Add specific file extension to watch
     * 
     * @param extension File extension to watch (e.g., ".txt")
     */
    void add_extension_filter(const std::string& extension) {
        std::lock_guard<std::mutex> lock(mutex_);
        extension_filters_.insert(extension);
    }
    
    /**
     * Enable or disable recursive watching
     * 
     * @param recursive Whether to watch subdirectories
     */
    void set_recursive(bool recursive) {
        std::lock_guard<std::mutex> lock(mutex_);
        recursive_ = recursive;
    }
    
    /**
     * Destructor - ensures watcher is stopped
     */
    ~FileWatcher() {
        stop();
    }
    
private:
    fs::path watch_path_;                                // Directory to watch
    std::chrono::milliseconds interval_;                // Polling interval
    std::atomic<bool> running_;                         // Whether watcher is running
    std::thread watcher_thread_;                        // Thread for watching
    std::mutex mutex_;                                  // Mutex for thread safety
    std::condition_variable cv_;                        // Condition variable for stopping
    std::map<fs::path, fs::file_time_type> file_times_; // Last modified times
    std::map<std::string, std::vector<Callback>> callbacks_; // Callbacks for events
    std::unordered_set<std::string> extension_filters_; // Extensions to watch
    bool recursive_ = false;                           // Whether to watch subdirectories
    
    /**
     * Main watching function that runs in a separate thread
     */
    void watch() {
        // Initial scan to populate the file times map
        scan_files(true);
        
        while (running_) {
            {
                // Sleep for the interval, but allow early wake up for stopping
                std::unique_lock<std::mutex> lock(mutex_);
                if (cv_.wait_for(lock, interval_, [this] { return !running_; })) {
                    // If we woke up because running_ is false, exit the loop
                    break;
                }
            }
            
            // Check for changes
            scan_files(false);
        }
    }
    
    /**
     * Scan the directory for files and check for changes
     * 
     * @param initial Whether this is the initial scan
     */
    void scan_files(bool initial) {
        try {
            // Copy of tracked files for comparison
            std::map<fs::path, fs::file_time_type> current_file_times;
            
            // Determine the iterator type based on recursive flag
            if (recursive_) {
                scan_recursive(current_file_times);
            } else {
                scan_non_recursive(current_file_times);
            }
            
            if (!initial) {
                // Check for created or modified files
                for (const auto& [path, time] : current_file_times) {
                    // Check if this is a new file
                    if (file_times_.find(path) == file_times_.end()) {
                        notify("created", path);
                    }
                    // Check if existing file was modified
                    else if (file_times_[path] != time) {
                        notify("modified", path);
                    }
                }
                
                // Check for deleted files
                for (const auto& [path, time] : file_times_) {
                    if (current_file_times.find(path) == current_file_times.end()) {
                        notify("deleted", path);
                    }
                }
            }
            
            // Update the file times map
            std::lock_guard<std::mutex> lock(mutex_);
            file_times_ = std::move(current_file_times);
            
        } catch (const std::exception& e) {
            std::cerr << "Error during directory scan: " << e.what() << "\n";
        }
    }
    
    /**
     * Scan directory non-recursively
     */
    void scan_non_recursive(std::map<fs::path, fs::file_time_type>& current_file_times) {
        for (const auto& entry : fs::directory_iterator(watch_path_)) {
            if (fs::is_regular_file(entry) && should_watch_file(entry.path())) {
                current_file_times[entry.path()] = fs::last_write_time(entry);
            }
        }
    }
    
    /**
     * Scan directory recursively
     */
    void scan_recursive(std::map<fs::path, fs::file_time_type>& current_file_times) {
        for (const auto& entry : fs::recursive_directory_iterator(
                watch_path_, 
                fs::directory_options::skip_permission_denied)) {
            if (fs::is_regular_file(entry) && should_watch_file(entry.path())) {
                current_file_times[entry.path()] = fs::last_write_time(entry);
            }
        }
    }
    
    /**
     * Check if we should watch this file based on extension filters
     */
    bool should_watch_file(const fs::path& path) {
        std::lock_guard<std::mutex> lock(mutex_);
        
        // If no filters are set, watch all files
        if (extension_filters_.empty()) {
            return true;
        }
        
        // Check if the file extension is in our filter list
        std::string ext = path.extension().string();
        return extension_filters_.find(ext) != extension_filters_.end();
    }
    
    /**
     * Notify callbacks about an event
     */
    void notify(const std::string& event_type, const fs::path& path) {
        std::lock_guard<std::mutex> lock(mutex_);
        
        // Find callbacks for this event type
        auto it = callbacks_.find(event_type);
        if (it != callbacks_.end()) {
            // Call each callback
            for (const auto& callback : it->second) {
                callback(path, event_type);
            }
        }
    }
};

// Example usage
int main() {
    std::cout << "=== C++17 Filesystem File Watcher Example ===\n\n";
    
    try {
        // Create a test directory to watch
        fs::path test_dir = fs::current_path() / "watch_test";
        
        // Clean up previous test directory if it exists
        if (fs::exists(test_dir)) {
            fs::remove_all(test_dir);
        }
        
        // Create the test directory
        fs::create_directory(test_dir);
        std::cout << "Created test directory: " << test_dir << "\n";
        
        // Create a file watcher for the test directory
        FileWatcher watcher(test_dir, std::chrono::milliseconds(500));
        
        // Add callbacks for different events
        watcher.add_callback("created", [](const fs::path& path, const std::string& event) {
            std::cout << "File created: " << path.filename() << "\n";
        });
        
        watcher.add_callback("modified", [](const fs::path& path, const std::string& event) {
            std::cout << "File modified: " << path.filename() << "\n";
        });
        
        watcher.add_callback("deleted", [](const fs::path& path, const std::string& event) {
            std::cout << "File deleted: " << path.filename() << "\n";
        });
        
        // Start the watcher
        watcher.start();
        
        std::cout << "\nFile watcher started. Monitoring directory: " << test_dir << "\n";
        std::cout << "Performing file operations to demonstrate watcher functionality...\n\n";
        
        // Create a new file
        fs::path test_file1 = test_dir / "test1.txt";
        {
            std::ofstream file(test_file1);
            file << "Hello, world!";
        }
        std::cout << "Created file: " << test_file1.filename() << "\n";
        
        // Sleep to allow watcher to detect the change
        std::this_thread::sleep_for(std::chrono::seconds(1));
        
        // Modify the file
        {
            std::ofstream file(test_file1, std::ios::app);
            file << "\nAppended content.";
        }
        std::cout << "Modified file: " << test_file1.filename() << "\n";
        
        // Sleep to allow watcher to detect the change
        std::this_thread::sleep_for(std::chrono::seconds(1));
        
        // Create a subdirectory
        fs::path sub_dir = test_dir / "subdir";
        fs::create_directory(sub_dir);
        std::cout << "Created subdirectory: " << sub_dir.filename() << "\n";
        
        // Create a file in the subdirectory
        fs::path test_file2 = sub_dir / "test2.txt";
        {
            std::ofstream file(test_file2);
            file << "File in subdirectory";
        }
        std::cout << "Created file in subdirectory: " << test_file2.filename() << "\n";
        
        // Sleep to allow watcher to detect the change
        std::this_thread::sleep_for(std::chrono::seconds(1));
        
        // Enable recursive watching now and create one more file in the subdirectory
        watcher.set_recursive(true);
        std::cout << "\nEnabled recursive watching\n";
        
        fs::path test_file3 = sub_dir / "test3.txt";
        {
            std::ofstream file(test_file3);
            file << "Another file in subdirectory";
        }
        std::cout << "Created another file in subdirectory: " << test_file3.filename() << "\n";
        
        // Sleep to allow watcher to detect the change
        std::this_thread::sleep_for(std::chrono::seconds(1));
        
        // Delete a file
        fs::remove(test_file1);
        std::cout << "Deleted file: " << test_file1.filename() << "\n";
        
        // Sleep to allow watcher to detect the change
        std::this_thread::sleep_for(std::chrono::seconds(1));
        
        // Add file extension filter and create files with different extensions
        watcher.add_extension_filter(".log");
        std::cout << "\nAdded extension filter for .log files\n";
        
        fs::path log_file = test_dir / "test.log";
        fs::path txt_file = test_dir / "another.txt";
        
        {
            std::ofstream file(log_file);
            file << "This is a log file";
        }
        std::cout << "Created .log file: " << log_file.filename() << "\n";
        
        {
            std::ofstream file(txt_file);
            file << "This is a txt file";
        }
        std::cout << "Created .txt file: " << txt_file.filename() << " (should not trigger callback)\n";
        
        // Sleep to allow watcher to detect the change
        std::this_thread::sleep_for(std::chrono::seconds(2));
        
        // Stop the watcher
        std::cout << "\nStopping file watcher...\n";
        watcher.stop();
        
        std::cout << "\nFile watcher stopped. Press Enter to clean up test directory and exit...\n";
        std::cin.get();
        
        // Clean up test directory
        fs::remove_all(test_dir);
        std::cout << "Removed test directory: " << test_dir << "\n";
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
    
    return 0;
}

/**
 * === INTERVIEW GUIDE: File Watcher Implementation ===
 * 
 * Key Concepts to Understand:
 * 
 * 1. File Watching Approaches:
 *    - Polling: Periodically check file attributes for changes
 *    - Event-based: Use OS-specific APIs (inotify on Linux, ReadDirectoryChangesW on Windows)
 *    - Our implementation uses polling for cross-platform compatibility
 * 
 * 2. Performance Considerations:
 *    - Time Complexity: O(n) where n is the number of watched files
 *    - Space Complexity: O(n) to store file data
 *    - Polling interval trade-off: shorter intervals detect changes faster but use more CPU
 *    - Platform-specific APIs can be more efficient but less portable
 * 
 * 3. Thread Safety:
 *    - Mutex protection for shared data
 *    - Atomic flag for thread control
 *    - Condition variable for signaling
 * 
 * 4. Common Interview Questions:
 * 
 *    Q: What are the pros and cons of polling-based vs. event-based file watching?
 *    A: Polling:
 *       - Pros: Simple, portable across platforms
 *       - Cons: CPU intensive, may miss changes between polls, latency in detecting changes
 *       Event-based:
 *       - Pros: Immediate notification, lower CPU usage, won't miss events
 *       - Cons: Platform-specific, more complex, can be harder to debug
 * 
 *    Q: How would you optimize this file watcher for a very large directory?
 *    A: - Implement a more efficient hashing scheme instead of storing all file details
 *       - Use platform-specific APIs where available
 *       - Add a more sophisticated filtering system to only watch relevant files
 *       - Consider a hybrid approach: rare full scans with more frequent targeted checks
 * 
 *    Q: How does your implementation handle the case when a file is renamed?
 *    A: In the current implementation, a rename would trigger both a "deleted" event
 *       for the old path and a "created" event for the new path. To properly detect
 *       renames, you would need to compare file contents or inodes (on Unix systems)
 *       or use platform-specific APIs that directly report rename events.
 * 
 *    Q: How would you modify the watcher to handle file content changes?
 *    A: In addition to checking last_write_time, you could:
 *       - Calculate and store a hash of file contents, then compare on each scan
 *       - Store file sizes and only read/hash content when size or timestamp changes
 *       - For text files, implement a diff algorithm to detect specific changes
 * 
 *    Q: How would you handle race conditions when multiple files are changed simultaneously?
 *    A: - Ensure all file operations are thread-safe with proper mutex protection
 *       - Queue events and process them sequentially
 *       - Use a thread pool to process multiple events in parallel but in a controlled manner
 * 
 * 5. Design Patterns Used:
 *    - Observer Pattern: Callbacks are registered for events
 *    - Producer-Consumer: The watcher thread produces events consumed by callbacks
 *    - Strategy Pattern: Different scanning strategies (recursive vs. non-recursive)
 * 
 * 6. Common Pitfalls:
 *    - Not handling permissions errors
 *    - Memory leaks in callback management
 *    - Resource exhaustion from watching too many files
 *    - Race conditions from inadequate thread synchronization
 *    - Missing file system changes that occur faster than polling interval
 */
