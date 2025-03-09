#include <iostream>
#include <filesystem>
#include <fstream>
#include <chrono>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <iomanip>
#include <sstream>

namespace fs = std::filesystem;

/**
 * @brief Simple File Backup System
 * 
 * This program demonstrates a basic file backup system using the C++17 filesystem
 * library. It copies files from a source directory to a backup directory,
 * preserving the directory structure and only copying files that have changed.
 * 
 * Interview Topics:
 * - Selective file copying based on file metadata
 * - Directory structure preservation
 * - Error handling in filesystem operations
 * - Backup strategies and versioning
 */

class BackupSystem {
public:
    BackupSystem(const fs::path& source_dir, const fs::path& backup_dir) 
        : source_dir_(source_dir), backup_dir_(backup_dir) {
        
        // Validate source directory
        if (!fs::exists(source_dir)) {
            throw std::runtime_error("Source directory does not exist: " + source_dir.string());
        }
        
        if (!fs::is_directory(source_dir)) {
            throw std::runtime_error("Source path is not a directory: " + source_dir.string());
        }
        
        // Create backup directory if it doesn't exist
        if (!fs::exists(backup_dir)) {
            std::cout << "Creating backup directory: " << backup_dir << "\n";
            fs::create_directories(backup_dir);
        } else if (!fs::is_directory(backup_dir)) {
            throw std::runtime_error("Backup path exists but is not a directory: " + backup_dir.string());
        }
    }
    
    // Run the backup process
    void perform_backup() {
        stats_ = BackupStats();
        auto start_time = std::chrono::steady_clock::now();
        
        std::cout << "Starting backup from " << source_dir_ << " to " << backup_dir_ << "\n";
        
        // Process the source directory recursively
        backup_directory(source_dir_);
        
        auto end_time = std::chrono::steady_clock::now();
        stats_.duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
        
        print_backup_report();
    }
    
private:
    fs::path source_dir_;
    fs::path backup_dir_;
    
    // Statistics for the backup process
    struct BackupStats {
        size_t files_scanned = 0;
        size_t files_copied = 0;
        size_t files_skipped = 0;
        size_t bytes_copied = 0;
        size_t errors = 0;
        std::chrono::milliseconds duration{0};
    };
    
    BackupStats stats_;
    
    // Backup a directory recursively
    void backup_directory(const fs::path& source_subdir) {
        try {
            // Calculate relative path from source root
            fs::path rel_path = source_subdir.lexically_relative(source_dir_);
            fs::path target_dir = backup_dir_ / rel_path;
            
            // Create target directory if it doesn't exist
            if (!fs::exists(target_dir) && !rel_path.empty()) {
                fs::create_directories(target_dir);
                std::cout << "Created directory: " << target_dir << "\n";
            }
            
            // Iterate through directory entries
            for (const auto& entry : fs::directory_iterator(source_subdir)) {
                try {
                    if (fs::is_directory(entry)) {
                        // Recursively backup subdirectory
                        backup_directory(entry.path());
                    } 
                    else if (fs::is_regular_file(entry)) {
                        // Backup file
                        backup_file(entry.path());
                    }
                    // Skip other types (symlinks, etc.)
                } catch (const std::exception& e) {
                    std::cerr << "Error processing " << entry.path() << ": " << e.what() << "\n";
                    stats_.errors++;
                }
            }
        } catch (const std::exception& e) {
            std::cerr << "Error accessing directory " << source_subdir << ": " << e.what() << "\n";
            stats_.errors++;
        }
    }
    
    // Backup a single file
    void backup_file(const fs::path& source_file) {
        stats_.files_scanned++;
        
        try {
            // Calculate the target path in the backup directory
            fs::path rel_path = source_file.lexically_relative(source_dir_);
            fs::path target_file = backup_dir_ / rel_path;
            
            // Check if we need to copy the file
            bool need_copy = false;
            
            if (!fs::exists(target_file)) {
                // Target doesn't exist, need to copy
                need_copy = true;
            } else {
                // Compare file sizes
                auto source_size = fs::file_size(source_file);
                auto target_size = fs::file_size(target_file);
                
                if (source_size != target_size) {
                    need_copy = true;
                } else {
                    // Compare last write times
                    auto source_time = fs::last_write_time(source_file);
                    auto target_time = fs::last_write_time(target_file);
                    
                    if (source_time > target_time) {
                        need_copy = true;
                    }
                }
            }
            
            // Copy file if needed
            if (need_copy) {
                // Create parent directories if needed
                fs::create_directories(target_file.parent_path());
                
                // Copy the file with overwrite option
                fs::copy_file(source_file, target_file, fs::copy_options::overwrite_existing);
                
                // Update statistics
                stats_.files_copied++;
                stats_.bytes_copied += fs::file_size(source_file);
                
                std::cout << "Copied: " << rel_path << "\n";
            } else {
                stats_.files_skipped++;
            }
        } catch (const std::exception& e) {
            std::cerr << "Error backing up " << source_file << ": " << e.what() << "\n";
            stats_.errors++;
        }
    }
    
    // Format file size to human-readable form
    std::string format_size(uintmax_t size) const {
        const char* units[] = {"B", "KB", "MB", "GB", "TB"};
        int unit_index = 0;
        double size_d = static_cast<double>(size);
        
        while (size_d >= 1024.0 && unit_index < 4) {
            size_d /= 1024.0;
            unit_index++;
        }
        
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(2) << size_d << " " << units[unit_index];
        return oss.str();
    }
    
    // Print backup report
    void print_backup_report() const {
        std::cout << "\n=== Backup Report ===\n";
        std::cout << "Source: " << source_dir_ << "\n";
        std::cout << "Destination: " << backup_dir_ << "\n";
        std::cout << "Duration: " << stats_.duration.count() << " ms\n";
        std::cout << "Files scanned: " << stats_.files_scanned << "\n";
        std::cout << "Files copied: " << stats_.files_copied << "\n";
        std::cout << "Files skipped (no changes): " << stats_.files_skipped << "\n";
        std::cout << "Data copied: " << format_size(stats_.bytes_copied) << "\n";
        std::cout << "Errors: " << stats_.errors << "\n";
        
        if (stats_.errors > 0) {
            std::cout << "WARNING: Some errors occurred during the backup process.\n";
            std::cout << "Please check the error messages above for details.\n";
        } else {
            std::cout << "Backup completed successfully.\n";
        }
    }
};

// A more advanced backup system with versioning capabilities
class VersionedBackupSystem : public BackupSystem {
public:
    VersionedBackupSystem(const fs::path& source_dir, const fs::path& backup_dir)
        : BackupSystem(source_dir, backup_dir) {
        // Create a timestamped version directory
        auto now = std::chrono::system_clock::now();
        auto time_t_now = std::chrono::system_clock::to_time_t(now);
        
        std::stringstream ss;
        ss << std::put_time(std::localtime(&time_t_now), "%Y%m%d_%H%M%S");
        version_dir_ = backup_dir / ss.str();
        
        std::cout << "Creating versioned backup in: " << version_dir_ << "\n";
        fs::create_directories(version_dir_);
    }
    
    // Override to use versioned directory
    void perform_backup() {
        // Replace backup_dir with version_dir in base class
        fs::path original_backup_dir = backup_dir_;
        backup_dir_ = version_dir_;
        
        // Call base class implementation
        BackupSystem::perform_backup();
        
        // Restore original backup dir
        backup_dir_ = original_backup_dir;
    }
    
private:
    fs::path version_dir_;  // Directory for this backup version
    fs::path backup_dir_;   // Base backup directory
};

int main(int argc, char* argv[]) {
    std::cout << "=== C++17 Filesystem Backup System Example ===\n\n";
    
    try {
        // Setup source and backup directories
        fs::path source_dir;
        fs::path backup_dir;
        bool use_versioning = false;
        
        // Parse command line arguments
        if (argc >= 3) {
            source_dir = argv[1];
            backup_dir = argv[2];
            
            if (argc >= 4 && std::string(argv[3]) == "--versioned") {
                use_versioning = true;
            }
        } else {
            // Default: Create a test directory structure and backup
            source_dir = fs::current_path() / "backup_test_source";
            backup_dir = fs::current_path() / "backup_test_dest";
            
            // Clean up previous test directories if they exist
            if (fs::exists(source_dir)) {
                fs::remove_all(source_dir);
            }
            if (fs::exists(backup_dir)) {
                fs::remove_all(backup_dir);
            }
            
            // Create source directory structure
            fs::create_directories(source_dir);
            fs::create_directories(source_dir / "docs");
            fs::create_directories(source_dir / "images");
            fs::create_directories(source_dir / "data" / "subfolder");
            
            // Create some test files
            create_test_file(source_dir / "readme.txt", "This is a readme file.");
            create_test_file(source_dir / "docs" / "document.txt", "This is a document.");
            create_test_file(source_dir / "images" / "image.jpg", std::string(1024, 'X')); // 1KB file
            create_test_file(source_dir / "data" / "data.csv", "id,name,value\n1,item1,100\n2,item2,200");
            create_test_file(source_dir / "data" / "subfolder" / "subdata.txt", "Nested file content");
            
            std::cout << "Created test directory structure at: " << source_dir << "\n\n";
        }
        
        // Create and run the appropriate backup system
        if (use_versioning) {
            VersionedBackupSystem backup(source_dir, backup_dir);
            backup.perform_backup();
        } else {
            BackupSystem backup(source_dir, backup_dir);
            backup.perform_backup();
        }
        
        std::cout << "\nBackup operation completed.\n";
        
        // If this was a test run, simulate a file change and run again
        if (argc < 3) {
            std::cout << "\nModifying a file to demonstrate incremental backup...\n";
            
            // Modify a file
            create_test_file(source_dir / "readme.txt", "This is a modified readme file with new content.");
            
            // Add a new file
            create_test_file(source_dir / "newfile.txt", "This is a newly added file.");
            
            std::cout << "\nRunning second backup to demonstrate only changed files are copied...\n";
            
            // Run backup again
            BackupSystem backup(source_dir, backup_dir);
            backup.perform_backup();
            
            std::cout << "\nTest completed. Backup directories are at:\n";
            std::cout << "Source: " << source_dir << "\n";
            std::cout << "Backup: " << backup_dir << "\n";
        }
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
    
    return 0;
}

// Helper function to create a test file with content
void create_test_file(const fs::path& path, const std::string& content) {
    std::ofstream file(path);
    if (file) {
        file << content;
    } else {
        std::cerr << "Error: Could not create test file: " << path << "\n";
    }
}

/**
 * === INTERVIEW GUIDE: File Backup System Implementation ===
 * 
 * Key Concepts to Understand:
 * 
 * 1. Backup Strategies:
 *    - Full Backup: Copy all files regardless of status
 *    - Incremental Backup: Copy only files that changed since last backup
 *    - Differential Backup: Copy files changed since last full backup
 *    - Our implementation uses an incremental approach based on file timestamps
 * 
 * 2. Performance Considerations:
 *    - Time Complexity: O(n) where n is the number of files
 *    - Space Complexity: O(n) for storing file metadata during backup
 *    - I/O is typically the bottleneck, not CPU or memory
 * 
 * 3. Common Interview Questions:
 * 
 *    Q: How would you improve the backup system to handle very large files?
 *    A: Several approaches:
 *       - Implement chunked copying with resumable transfers
 *       - Use memory-mapped files for efficient copying
 *       - Implement delta compression to only transfer changed parts
 *       - Add progress reporting for large file transfers
 * 
 *    Q: How would you implement a proper versioning system?
 *    A: Options include:
 *       - Timestamp-based directories for each backup (as shown in VersionedBackupSystem)
 *       - Hard linking unchanged files between versions to save space
 *       - Implementing a rotation system to manage backup retention
 *       - Adding metadata to track version history
 * 
 *    Q: What are the trade-offs between different backup strategies?
 *    A: - Full backup: Simple but uses more space and time
 *       - Incremental: Space and time efficient but complex restoration
 *       - Differential: Balanced approach with simpler restoration than incremental
 *       - Each strategy has different RPO (Recovery Point Objective) implications
 * 
 *    Q: How would you detect file renames to avoid unnecessary copies?
 *    A: - Compare file contents using hashing (e.g., SHA-256)
 *       - Maintain a database of file identifiers independent of path
 *       - Use OS-specific features like file IDs where available
 *       - Implement similarity detection for partial renames/moves
 * 
 *    Q: How would you improve error handling and recovery in the backup process?
 *    A: - Implement a transaction log to track backup progress
 *       - Add the ability to resume failed backups
 *       - Verify file integrity after copying (checksum comparison)
 *       - Add detailed error classification and reporting
 *       - Implement retry logic for transient errors
 * 
 * 4. Edge Cases to Consider:
 *    - Permission issues when accessing files
 *    - Files changing during the backup process
 *    - File system limitations (path length, special characters)
 *    - Very large directory structures with millions of files
 *    - Network interruptions for remote backups
 * 
 * 5. Thread Safety and Parallelism:
 *    - The current implementation is single-threaded
 *    - To improve performance, you could:
 *      - Use a thread pool to copy multiple files in parallel
 *      - Prioritize small files for quick wins
 *      - Use asynchronous I/O operations
 * 
 * 6. Advanced Features to Consider:
 *    - Data deduplication
 *    - Compression
 *    - Encryption
 *    - Backup verification
 *    - Retention policies
 *    - Disaster recovery testing
 */
