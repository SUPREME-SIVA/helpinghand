#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <chrono>
#include <iomanip>

namespace fs = std::filesystem;

/**
 * @brief Basic Filesystem Operations Example
 * 
 * This program demonstrates fundamental filesystem operations using the C++17 std::filesystem library.
 * It covers path manipulation, file existence checks, file creation, copying, and metadata access.
 * 
 * Interview Topics:
 * - Path manipulation and normalization
 * - Error handling with std::filesystem::filesystem_error
 * - File permissions and attributes
 * - Time complexity considerations for filesystem operations
 */

// Helper function to print file information
void print_file_info(const fs::path& path) {
    try {
        std::cout << "\nFile: " << path << "\n";
        
        if (fs::exists(path)) {
            // Get file size
            auto size = fs::file_size(path);
            std::cout << "Size: " << size << " bytes\n";
            
            // Get last write time
            auto last_write = fs::last_write_time(path);
            auto last_write_time = std::chrono::time_point_cast<std::chrono::system_clock::duration>(
                last_write - fs::file_time_type::clock::now() + std::chrono::system_clock::now());
            
            std::time_t time = std::chrono::system_clock::to_time_t(last_write_time);
            std::cout << "Last modified: " << std::put_time(std::localtime(&time), "%F %T") << "\n";
            
            // Check file type
            std::cout << "Type: ";
            if (fs::is_regular_file(path)) std::cout << "Regular file\n";
            else if (fs::is_directory(path)) std::cout << "Directory\n";
            else if (fs::is_symlink(path)) std::cout << "Symbolic link\n";
            else std::cout << "Other\n";
            
            // Check permissions (simplified)
            std::cout << "Permissions: ";
            auto perms = fs::status(path).permissions();
            std::cout << ((perms & fs::perms::owner_read) != fs::perms::none ? "r" : "-");
            std::cout << ((perms & fs::perms::owner_write) != fs::perms::none ? "w" : "-");
            std::cout << ((perms & fs::perms::owner_exec) != fs::perms::none ? "x" : "-");
            std::cout << "\n";
        } else {
            std::cout << "File does not exist\n";
        }
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Filesystem error: " << e.what() << "\n";
        std::cerr << "Path 1: " << e.path1() << "\n";
        std::cerr << "Path 2: " << e.path2() << "\n";
        std::cerr << "Error code: " << e.code().value() << " - " << e.code().message() << "\n";
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
    }
}

int main() {
    std::cout << "=== C++17 Filesystem Basic Operations ===\n\n";
    
    try {
        // 1. Path manipulation
        std::cout << "=== Path Manipulation ===\n";
        
        fs::path current_path = fs::current_path();
        std::cout << "Current path: " << current_path << "\n";
        
        // Creating a path and normalizing it
        fs::path example_path = current_path / "examples" / "test" / ".." / "example.txt";
        std::cout << "Path with ..: " << example_path << "\n";
        
        // Canonical form resolves symlinks and removes . and ..
        try {
            std::cout << "Canonical path: " << fs::canonical(current_path) << "\n";
        } catch (const fs::filesystem_error& e) {
            std::cout << "Cannot get canonical path: " << e.what() << "\n";
        }
        
        // Path components
        fs::path file_path = "data/config/settings.json";
        std::cout << "Path: " << file_path << "\n";
        std::cout << "Root name: " << file_path.root_name() << "\n";
        std::cout << "Root directory: " << file_path.root_directory() << "\n";
        std::cout << "Root path: " << file_path.root_path() << "\n";
        std::cout << "Relative path: " << file_path.relative_path() << "\n";
        std::cout << "Parent path: " << file_path.parent_path() << "\n";
        std::cout << "Filename: " << file_path.filename() << "\n";
        std::cout << "Stem: " << file_path.stem() << "\n";
        std::cout << "Extension: " << file_path.extension() << "\n";
        
        // 2. Creating a test directory structure
        std::cout << "\n=== Creating Test Directory Structure ===\n";
        
        fs::path test_dir = current_path / "filesystem_test";
        
        // Remove previous test directory if it exists
        if (fs::exists(test_dir)) {
            std::cout << "Removing existing test directory: " << test_dir << "\n";
            fs::remove_all(test_dir);
        }
        
        // Create directories
        std::cout << "Creating directory: " << test_dir << "\n";
        fs::create_directory(test_dir);
        
        fs::path nested_dir = test_dir / "nested";
        std::cout << "Creating nested directory: " << nested_dir << "\n";
        fs::create_directories(nested_dir);  // create_directories handles multiple levels
        
        // 3. Creating and writing to a file
        std::cout << "\n=== File Creation and Writing ===\n";
        
        fs::path test_file = test_dir / "example.txt";
        std::cout << "Creating file: " << test_file << "\n";
        
        std::ofstream file(test_file);
        if (file.is_open()) {
            file << "This is a test file created with C++17 Filesystem library.\n";
            file << "It demonstrates basic file operations.\n";
            file.close();
            std::cout << "File written successfully\n";
        } else {
            std::cerr << "Failed to open file for writing\n";
        }
        
        // Display file information
        print_file_info(test_file);
        
        // 4. Copying a file
        std::cout << "\n=== File Copying ===\n";
        
        fs::path copy_target = test_dir / "example_copy.txt";
        std::cout << "Copying " << test_file << " to " << copy_target << "\n";
        
        fs::copy_file(test_file, copy_target, fs::copy_options::overwrite_existing);
        std::cout << "File copied successfully\n";
        
        // 5. Renaming a file
        std::cout << "\n=== File Renaming ===\n";
        
        fs::path renamed_file = test_dir / "renamed_example.txt";
        std::cout << "Renaming " << copy_target << " to " << renamed_file << "\n";
        
        fs::rename(copy_target, renamed_file);
        std::cout << "File renamed successfully\n";
        
        // 6. File existence and properties
        std::cout << "\n=== File Properties ===\n";
        
        // Check if the renamed file exists
        if (fs::exists(renamed_file)) {
            std::cout << renamed_file << " exists\n";
            
            // Get file size
            std::cout << "Size: " << fs::file_size(renamed_file) << " bytes\n";
            
            // Check if it's a regular file
            if (fs::is_regular_file(renamed_file)) {
                std::cout << "It is a regular file\n";
            }
        }
        
        // 7. Creating a symbolic link (platform-dependent)
        std::cout << "\n=== Symbolic Links ===\n";
        
        fs::path symlink_path = test_dir / "example_link.txt";
        
        try {
            if (fs::exists(symlink_path)) {
                fs::remove(symlink_path);
            }
            
            std::cout << "Creating symbolic link from " << test_file << " to " << symlink_path << "\n";
            fs::create_symlink(test_file, symlink_path);
            
            if (fs::is_symlink(symlink_path)) {
                std::cout << "Created symbolic link successfully\n";
                std::cout << "Link target: " << fs::read_symlink(symlink_path) << "\n";
            }
        } catch (const fs::filesystem_error& e) {
            std::cout << "Symlink operation failed: " << e.what() << "\n";
            std::cout << "Note: Creating symbolic links may require elevated permissions on some systems\n";
        }
        
        // 8. Cleaning up (uncomment to remove test files)
        /*
        std::cout << "\n=== Cleanup ===\n";
        std::cout << "Removing test directory and all contents\n";
        std::size_t removed = fs::remove_all(test_dir);
        std::cout << "Removed " << removed << " files/directories\n";
        */
        
        std::cout << "\nTest directory is at: " << test_dir << "\n";
        std::cout << "Examine the files there or run with the cleanup code uncommented to remove them.\n";
        
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Filesystem error: " << e.what() << "\n";
        std::cerr << "Path 1: " << e.path1() << "\n";
        std::cerr << "Path 2: " << e.path2() << "\n";
        std::cerr << "Error code: " << e.code().value() << " - " << e.code().message() << "\n";
        return 1;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
    
    return 0;
}

/**
 * === INTERVIEW GUIDE: C++17 Filesystem Basic Operations ===
 * 
 * Key Concepts to Understand:
 * 
 * 1. Path Handling:
 *    - C++17 introduced std::filesystem::path for OS-independent path manipulation
 *    - Path normalization (e.g., handling . and .. entries)
 *    - Converting between absolute and relative paths
 * 
 * 2. Error Handling:
 *    - The filesystem library uses exceptions (std::filesystem::filesystem_error)
 *    - Each exception provides access to error codes and affected paths
 *    - Alternative error handling using error_code parameters
 * 
 * 3. Time Complexity:
 *    - Most basic operations (exists, is_directory) are O(1)
 *    - Operations like file_size, last_write_time are typically O(1)
 *    - Directory operations like create_directory are typically O(1)
 *    - remove_all is O(n) where n is the number of entries
 *    - canonical path resolution can be O(n) where n is path complexity
 * 
 * 4. Common Interview Questions:
 * 
 *    Q: How does C++17's filesystem library improve upon previous approaches?
 *    A: It provides platform-independent code, standardized error handling,
 *       rich path manipulation, and comprehensive file system operations,
 *       eliminating the need for platform-specific code or external libraries.
 * 
 *    Q: How do you handle the case when a file operation fails?
 *    A: You can either use try-catch blocks to catch std::filesystem::filesystem_error
 *       exceptions, or use the versions of functions that take an std::error_code
 *       parameter that gets set when an error occurs.
 * 
 *    Q: How would you recursively calculate the size of all files in a directory?
 *    A: Use std::filesystem::recursive_directory_iterator to traverse the directory
 *       tree and sum the file_size of each regular file encountered.
 * 
 *    Q: What's the difference between absolute, relative, and canonical paths?
 *    A: - Absolute path: complete path from root directory
 *       - Relative path: path relative to current working directory
 *       - Canonical path: absolute path with all symbolic links resolved and
 *         all . and .. components normalized
 * 
 *    Q: How do you safely remove a directory and all its contents?
 *    A: Use std::filesystem::remove_all, which returns the number of items removed.
 *       It's atomic in the sense that it either completes fully or reports an error,
 *       but individual file removals can fail while others succeed.
 * 
 * 5. Thread Safety:
 *    - The filesystem library itself is thread-safe for different paths
 *    - Operations on the same path from different threads may lead to race conditions
 *    - Consider using mutexes for concurrent access to the same paths
 * 
 * 6. Common Pitfalls:
 *    - Not checking if a file exists before attempting operations
 *    - Not handling exceptions or error codes
 *    - Assuming path separators are the same across all platforms
 *    - Not considering permission issues, especially for operations like
 *      creating symbolic links or removing files
 *    - Not handling Unicode paths properly
 */
