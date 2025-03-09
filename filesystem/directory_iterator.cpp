#include <iostream>
#include <filesystem>
#include <string>
#include <vector>
#include <algorithm>
#include <chrono>
#include <iomanip>

namespace fs = std::filesystem;

/**
 * @brief Directory Iteration Example
 * 
 * This program demonstrates various techniques for directory traversal using the C++17 
 * std::filesystem library. It covers directory iteration, recursive directory iteration,
 * filtering entries, and gathering statistics about directory content.
 * 
 * Interview Topics:
 * - Directory iteration approaches and performance
 * - Recursive vs. non-recursive traversal
 * - Exception handling during traversal
 * - Filter strategies for file types and properties
 */

// Format file size with appropriate units
std::string format_file_size(uintmax_t size) {
    constexpr uintmax_t KB = 1024;
    constexpr uintmax_t MB = 1024 * KB;
    constexpr uintmax_t GB = 1024 * MB;
    
    if (size < KB) {
        return std::to_string(size) + " B";
    } else if (size < MB) {
        return std::to_string(size / KB) + "." + std::to_string((size % KB) * 10 / KB) + " KB";
    } else if (size < GB) {
        return std::to_string(size / MB) + "." + std::to_string((size % MB) * 10 / MB) + " MB";
    } else {
        return std::to_string(size / GB) + "." + std::to_string((size % GB) * 10 / GB) + " GB";
    }
}

// Print last modification time of a file
void print_last_modified(const fs::file_time_type& time) {
    auto sctp = std::chrono::time_point_cast<std::chrono::system_clock::duration>(
        time - fs::file_time_type::clock::now() + std::chrono::system_clock::now());
    
    std::time_t tt = std::chrono::system_clock::to_time_t(sctp);
    std::cout << std::put_time(std::localtime(&tt), "%Y-%m-%d %H:%M:%S");
}

// Basic directory iteration
void basic_directory_iteration(const fs::path& path) {
    std::cout << "\n=== Basic Directory Iteration ===\n";
    std::cout << "Contents of: " << path << "\n\n";
    
    try {
        // Check if path exists and is a directory
        if (!fs::exists(path)) {
            std::cout << "Path does not exist\n";
            return;
        }
        
        if (!fs::is_directory(path)) {
            std::cout << "Path is not a directory\n";
            return;
        }
        
        // Simple iteration using a range-based for loop
        std::cout << "Directory entries:\n";
        for (const auto& entry : fs::directory_iterator(path)) {
            std::cout << (fs::is_directory(entry) ? "D " : "F ") 
                      << entry.path().filename().string();
            
            if (fs::is_regular_file(entry)) {
                std::cout << " (" << format_file_size(fs::file_size(entry)) << ")";
            }
            
            std::cout << "\n";
        }
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Filesystem error: " << e.what() << "\n";
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
    }
}

// Recursive directory iteration with detailed information
void recursive_directory_iteration(const fs::path& path, int max_depth = -1) {
    std::cout << "\n=== Recursive Directory Iteration ===\n";
    std::cout << "Recursively listing: " << path << "\n\n";
    
    try {
        // Using recursive_directory_iterator
        fs::recursive_directory_iterator dir_iter(
            path, 
            fs::directory_options::skip_permission_denied
        );
        
        // Initialize counters for statistics
        std::size_t file_count = 0;
        std::size_t dir_count = 0;
        std::size_t symlink_count = 0;
        uintmax_t total_size = 0;
        
        // Table header
        std::cout << std::left << std::setw(10) << "Type"
                  << std::setw(10) << "Size"
                  << std::setw(20) << "Modified"
                  << std::setw(15) << "Depth" 
                  << "Path\n";
        std::cout << std::string(80, '-') << "\n";
        
        for (const auto& entry : dir_iter) {
            // Check depth limit if set
            if (max_depth >= 0 && dir_iter.depth() > max_depth) {
                dir_iter.pop(); // Go back up one level
                continue;
            }
            
            // Get entry type and increase appropriate counter
            std::string type;
            if (fs::is_directory(entry)) {
                type = "DIR";
                dir_count++;
            } else if (fs::is_regular_file(entry)) {
                type = "FILE";
                file_count++;
                total_size += fs::file_size(entry);
            } else if (fs::is_symlink(entry)) {
                type = "SYMLINK";
                symlink_count++;
            } else {
                type = "OTHER";
            }
            
            // Print entry details
            std::cout << std::left << std::setw(10) << type;
            
            // Size (only for regular files)
            if (fs::is_regular_file(entry)) {
                std::cout << std::setw(10) << format_file_size(fs::file_size(entry));
            } else {
                std::cout << std::setw(10) << "-";
            }
            
            // Last modified time
            try {
                std::cout << std::setw(20);
                print_last_modified(fs::last_write_time(entry));
            } catch (...) {
                std::cout << std::setw(20) << "N/A";
            }
            
            // Depth and path
            std::cout << std::setw(15) << dir_iter.depth()
                      << entry.path().filename().string() << "\n";
            
            // Skip permission denied errors by using the directory_options::skip_permission_denied flag
        }
        
        // Print summary
        std::cout << "\nSummary:\n";
        std::cout << "Directories: " << dir_count << "\n";
        std::cout << "Files: " << file_count << "\n";
        std::cout << "Symbolic links: " << symlink_count << "\n";
        std::cout << "Total size: " << format_file_size(total_size) << "\n";
        
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Filesystem error: " << e.what() << "\n";
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
    }
}

// Find files matching specific criteria
void find_files_by_extension(const fs::path& path, const std::vector<std::string>& extensions, bool recursive = true) {
    std::cout << "\n=== Finding Files by Extension ===\n";
    std::cout << "Searching for files with extensions: ";
    for (const auto& ext : extensions) {
        std::cout << ext << " ";
    }
    std::cout << "\nIn directory: " << path << "\n\n";
    
    try {
        // Choose iterator type based on recursion flag
        if (recursive) {
            // Recursively iterate through all subdirectories
            fs::recursive_directory_iterator dir_iter(path, fs::directory_options::skip_permission_denied);
            
            std::cout << "Matching files:\n";
            for (const auto& entry : dir_iter) {
                if (fs::is_regular_file(entry)) {
                    std::string ext = entry.path().extension().string();
                    
                    // Convert extension to lowercase for case-insensitive comparison
                    std::transform(ext.begin(), ext.end(), ext.begin(), 
                                  [](unsigned char c) { return std::tolower(c); });
                    
                    // Check if this file's extension matches any in our list
                    if (std::find(extensions.begin(), extensions.end(), ext) != extensions.end()) {
                        std::cout << entry.path().string() << " (" 
                                  << format_file_size(fs::file_size(entry)) << ")\n";
                    }
                }
            }
        } else {
            // Only iterate through the top-level directory
            fs::directory_iterator dir_iter(path);
            
            std::cout << "Matching files:\n";
            for (const auto& entry : dir_iter) {
                if (fs::is_regular_file(entry)) {
                    std::string ext = entry.path().extension().string();
                    
                    // Convert extension to lowercase for case-insensitive comparison
                    std::transform(ext.begin(), ext.end(), ext.begin(), 
                                  [](unsigned char c) { return std::tolower(c); });
                    
                    // Check if this file's extension matches any in our list
                    if (std::find(extensions.begin(), extensions.end(), ext) != extensions.end()) {
                        std::cout << entry.path().filename().string() << " (" 
                                  << format_file_size(fs::file_size(entry)) << ")\n";
                    }
                }
            }
        }
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Filesystem error: " << e.what() << "\n";
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
    }
}

// List recently modified files
void find_recent_files(const fs::path& path, int days, bool recursive = true) {
    std::cout << "\n=== Finding Recently Modified Files ===\n";
    std::cout << "Searching for files modified in the last " << days << " days\n";
    std::cout << "In directory: " << path << "\n\n";
    
    try {
        // Calculate the cutoff time point
        auto now = fs::file_time_type::clock::now();
        auto cutoff = now - std::chrono::hours(24 * days);
        
        // Choose iterator type based on recursion flag
        if (recursive) {
            fs::recursive_directory_iterator dir_iter(path, fs::directory_options::skip_permission_denied);
            
            std::cout << "Recently modified files:\n";
            for (const auto& entry : dir_iter) {
                if (fs::is_regular_file(entry)) {
                    auto last_write_time = fs::last_write_time(entry);
                    
                    if (last_write_time >= cutoff) {
                        std::cout << entry.path().string() << "\n";
                        std::cout << "  Modified: ";
                        print_last_modified(last_write_time);
                        std::cout << "\n";
                    }
                }
            }
        } else {
            fs::directory_iterator dir_iter(path);
            
            std::cout << "Recently modified files:\n";
            for (const auto& entry : dir_iter) {
                if (fs::is_regular_file(entry)) {
                    auto last_write_time = fs::last_write_time(entry);
                    
                    if (last_write_time >= cutoff) {
                        std::cout << entry.path().filename().string() << "\n";
                        std::cout << "  Modified: ";
                        print_last_modified(last_write_time);
                        std::cout << "\n";
                    }
                }
            }
        }
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Filesystem error: " << e.what() << "\n";
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
    }
}

int main(int argc, char* argv[]) {
    std::cout << "=== C++17 Filesystem Directory Iterator Example ===\n";
    
    // Use the provided directory or default to current directory
    fs::path target_path;
    
    if (argc > 1) {
        target_path = argv[1];
    } else {
        target_path = fs::current_path();
    }
    
    // Ensure the path exists
    if (!fs::exists(target_path)) {
        std::cerr << "Error: Path does not exist: " << target_path << "\n";
        return 1;
    }
    
    // Basic directory listing
    basic_directory_iteration(target_path);
    
    // Recursive directory listing with depth limit of 2
    recursive_directory_iteration(target_path, 2);
    
    // Find files by extension (.cpp, .h, .txt)
    find_files_by_extension(target_path, {".cpp", ".h", ".txt"}, true);
    
    // Find recently modified files (last 7 days)
    find_recent_files(target_path, 7, true);
    
    return 0;
}

/**
 * === INTERVIEW GUIDE: Directory Traversal with C++17 Filesystem ===
 * 
 * Key Concepts to Understand:
 * 
 * 1. Iterator Types:
 *    - directory_iterator: Non-recursive, one level only
 *    - recursive_directory_iterator: Traverses subdirectories
 *    - Both support range-based for loops
 * 
 * 2. Performance Considerations:
 *    - Time Complexity: O(n) where n is the number of entries
 *    - Space Complexity: O(d) where d is the maximum directory depth (for recursive)
 *    - Filtering during iteration vs. collecting and filtering later
 * 
 * 3. Error Handling:
 *    - Using directory_options::skip_permission_denied flag
 *    - Try-catch blocks for specific operations
 *    - Error recovery and continuing iteration after errors
 * 
 * 4. Common Interview Questions:
 * 
 *    Q: What is the difference between directory_iterator and recursive_directory_iterator?
 *    A: directory_iterator only lists entries in the specified directory, while
 *       recursive_directory_iterator traverses into subdirectories. The recursive
 *       iterator also provides depth information and depth control methods like pop().
 * 
 *    Q: How would you handle permission errors during directory traversal?
 *    A: You can use the directory_options::skip_permission_denied flag when creating
 *       the iterator, or wrap each operation in try-catch blocks to handle errors at
 *       a more granular level.
 * 
 *    Q: How would you efficiently find all files larger than a certain size in a directory tree?
 *    A: Use recursive_directory_iterator to traverse the tree, check if each entry is
 *       a regular file with is_regular_file(), then check its size with file_size().
 *       This gives O(n) time complexity where n is the number of entries.
 * 
 *    Q: How can you limit the depth of recursion when using recursive_directory_iterator?
 *    A: You can either check the depth() method during iteration and use pop() to skip
 *       deeper levels, or use the recursion_pending() and disable_recursion_pending()
 *       methods to control recursion on a per-directory basis.
 * 
 *    Q: How would you implement a simple file indexing system to quickly find files by name?
 *    A: Build an index (e.g., a hash map) by traversing the directory tree once with
 *       recursive_directory_iterator, mapping filenames to their full paths. This gives
 *       O(1) lookup time after the initial O(n) build time.
 * 
 * 5. Thread Safety:
 *    - Multiple iterators on different paths are thread-safe
 *    - Modifying a directory while iterating through it can lead to undefined behavior
 *    - Consider using a mutex or other synchronization when multiple threads might 
 *      modify the directory structure
 * 
 * 6. Common Pitfalls:
 *    - Not handling permission errors when traversing system directories
 *    - Performance issues when recursively traversing very large directory trees
 *    - Assuming directory contents won't change during iteration
 *    - Not handling symlink cycles in recursive traversal
 *    - Forgetting that directory_iterator doesn't keep directories ordered
 */
