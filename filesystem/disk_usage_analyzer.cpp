#include <iostream>
#include <filesystem>
#include <map>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <string>
#include <chrono>
#include <sstream>

namespace fs = std::filesystem;

/**
 * @brief Disk Usage Analyzer
 * 
 * This program demonstrates how to analyze disk space usage across a directory structure
 * using the C++17 filesystem library. It calculates directory sizes recursively,
 * identifies large files, and provides a breakdown of space usage by file extension.
 * 
 * Interview Topics:
 * - Recursive directory traversal
 * - File size aggregation and statistics
 * - Efficient memory usage for large directory structures
 * - Time complexity analysis of filesystem operations
 */

// Format file size to human-readable form
std::string format_size(uintmax_t size) {
    const char* units[] = {"B", "KB", "MB", "GB", "TB", "PB", "EB"};
    int unit_index = 0;
    double size_d = static_cast<double>(size);
    
    while (size_d >= 1024.0 && unit_index < 6) {
        size_d /= 1024.0;
        unit_index++;
    }
    
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2) << size_d << " " << units[unit_index];
    return oss.str();
}

// Structure to hold file extension statistics
struct ExtensionStat {
    std::string extension;
    uintmax_t total_size = 0;
    size_t file_count = 0;
    
    // Compare for sorting by size
    bool operator<(const ExtensionStat& other) const {
        return total_size > other.total_size; // Sort by descending size
    }
};

// Structure to hold information about a large file
struct LargeFile {
    fs::path path;
    uintmax_t size = 0;
    
    // Compare for sorting by size
    bool operator<(const LargeFile& other) const {
        return size > other.size; // Sort by descending size
    }
};

// Class to analyze disk usage
class DiskUsageAnalyzer {
public:
    DiskUsageAnalyzer(const fs::path& root_path) 
        : root_path_(root_path), total_size_(0), file_count_(0), dir_count_(0) {
        
        if (!fs::exists(root_path)) {
            throw std::runtime_error("Path does not exist: " + root_path.string());
        }
        
        if (!fs::is_directory(root_path)) {
            throw std::runtime_error("Path is not a directory: " + root_path.string());
        }
    }
    
    // Run the analysis
    void analyze() {
        auto start_time = std::chrono::steady_clock::now();
        
        // Reset statistics
        total_size_ = 0;
        file_count_ = 0;
        dir_count_ = 0;
        extension_stats_.clear();
        large_files_.clear();
        directory_sizes_.clear();
        
        // Analyze the directory tree
        directory_sizes_[root_path_] = analyze_directory(root_path_);
        
        // Sort extension stats by total size
        std::sort(extension_stats_.begin(), extension_stats_.end());
        
        // Sort large files by size
        std::sort(large_files_.begin(), large_files_.end());
        
        // Calculate elapsed time
        auto end_time = std::chrono::steady_clock::now();
        analyze_duration_ = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
    }
    
    // Print analysis results
    void print_results(int top_extensions = 10, int top_files = 20, int top_dirs = 10) const {
        std::cout << "\n=== Disk Usage Analysis Results ===\n\n";
        
        // Summary
        std::cout << "Root directory: " << root_path_ << "\n";
        std::cout << "Total size: " << format_size(total_size_) << "\n";
        std::cout << "Total files: " << file_count_ << "\n";
        std::cout << "Total directories: " << dir_count_ << "\n";
        std::cout << "Analysis time: " << analyze_duration_.count() << " ms\n\n";
        
        // Top file extensions by size
        print_top_extensions(top_extensions);
        
        // Top large files
        print_large_files(top_files);
        
        // Top directories by size
        print_top_directories(top_dirs);
    }
    
private:
    fs::path root_path_;                                // Root directory to analyze
    uintmax_t total_size_;                              // Total size of all files
    size_t file_count_;                                 // Total number of files
    size_t dir_count_;                                  // Total number of directories
    std::vector<ExtensionStat> extension_stats_;        // Statistics by file extension
    std::vector<LargeFile> large_files_;                // List of large files
    std::map<fs::path, uintmax_t> directory_sizes_;     // Sizes of directories
    std::chrono::milliseconds analyze_duration_{0};     // Time taken for analysis
    
    // Threshold for large files (100 MB)
    const uintmax_t large_file_threshold_ = 100 * 1024 * 1024;
    
    // Analyze a directory recursively and return its total size
    uintmax_t analyze_directory(const fs::path& dir_path) {
        uintmax_t dir_size = 0;
        
        try {
            // Count this directory
            dir_count_++;
            
            // Iterate through all entries in the directory
            for (const auto& entry : fs::directory_iterator(dir_path)) {
                try {
                    if (fs::is_directory(entry)) {
                        // Recursively analyze subdirectory
                        uintmax_t subdir_size = analyze_directory(entry.path());
                        directory_sizes_[entry.path()] = subdir_size;
                        dir_size += subdir_size;
                    } 
                    else if (fs::is_regular_file(entry)) {
                        // Get file size
                        uintmax_t file_size = fs::file_size(entry);
                        
                        // Update directory size
                        dir_size += file_size;
                        
                        // Update total size and file count
                        total_size_ += file_size;
                        file_count_++;
                        
                        // Update extension statistics
                        update_extension_stats(entry.path(), file_size);
                        
                        // Check if this is a large file
                        if (file_size >= large_file_threshold_) {
                            large_files_.push_back({entry.path(), file_size});
                        }
                    }
                } catch (const fs::filesystem_error& e) {
                    std::cerr << "Error processing entry: " << entry.path() << "\n";
                    std::cerr << "  " << e.what() << "\n";
                }
            }
        } catch (const fs::filesystem_error& e) {
            std::cerr << "Error accessing directory: " << dir_path << "\n";
            std::cerr << "  " << e.what() << "\n";
        }
        
        return dir_size;
    }
    
    // Update extension statistics for a file
    void update_extension_stats(const fs::path& file_path, uintmax_t file_size) {
        std::string ext = file_path.extension().string();
        
        // Convert to lowercase for case-insensitive comparison
        std::transform(ext.begin(), ext.end(), ext.begin(), 
                      [](unsigned char c) { return std::tolower(c); });
        
        // If empty extension, use "[no extension]"
        if (ext.empty()) {
            ext = "[no extension]";
        }
        
        // Find or create extension stats
        auto it = std::find_if(extension_stats_.begin(), extension_stats_.end(),
                              [&ext](const ExtensionStat& stat) { return stat.extension == ext; });
        
        if (it != extension_stats_.end()) {
            // Update existing stats
            it->total_size += file_size;
            it->file_count++;
        } else {
            // Add new extension stats
            extension_stats_.push_back({ext, file_size, 1});
        }
    }
    
    // Print top file extensions by size
    void print_top_extensions(int count) const {
        std::cout << "=== Top File Extensions by Size ===\n\n";
        std::cout << std::left << std::setw(15) << "Extension" 
                  << std::setw(15) << "Total Size" 
                  << std::setw(10) << "Files" 
                  << std::setw(10) << "Avg Size" 
                  << "% of Total\n";
        std::cout << std::string(60, '-') << "\n";
        
        int printed = 0;
        for (const auto& stat : extension_stats_) {
            if (printed >= count) break;
            
            double percent = (static_cast<double>(stat.total_size) / total_size_) * 100.0;
            double avg_size = static_cast<double>(stat.total_size) / stat.file_count;
            
            std::cout << std::left << std::setw(15) << stat.extension 
                      << std::setw(15) << format_size(stat.total_size) 
                      << std::setw(10) << stat.file_count 
                      << std::setw(10) << format_size(static_cast<uintmax_t>(avg_size)) 
                      << std::fixed << std::setprecision(2) << percent << "%\n";
            
            printed++;
        }
        
        std::cout << "\n";
    }
    
    // Print top large files
    void print_large_files(int count) const {
        std::cout << "=== Largest Files ===\n\n";
        std::cout << std::left << std::setw(15) << "Size" 
                  << "Path\n";
        std::cout << std::string(80, '-') << "\n";
        
        int printed = 0;
        for (const auto& file : large_files_) {
            if (printed >= count) break;
            
            std::cout << std::left << std::setw(15) << format_size(file.size) 
                      << file.path.string() << "\n";
            
            printed++;
        }
        
        std::cout << "\n";
    }
    
    // Print top directories by size
    void print_top_directories(int count) const {
        std::cout << "=== Largest Directories ===\n\n";
        std::cout << std::left << std::setw(15) << "Size" 
                  << "Path\n";
        std::cout << std::string(80, '-') << "\n";
        
        // Create a sorted list of directories by size
        std::vector<std::pair<fs::path, uintmax_t>> dirs;
        for (const auto& [path, size] : directory_sizes_) {
            dirs.push_back({path, size});
        }
        
        // Sort by size (descending)
        std::sort(dirs.begin(), dirs.end(), 
                 [](const auto& a, const auto& b) { return a.second > b.second; });
        
        // Print top directories
        int printed = 0;
        for (const auto& [path, size] : dirs) {
            if (printed >= count) break;
            
            std::cout << std::left << std::setw(15) << format_size(size) 
                      << path.string() << "\n";
            
            printed++;
        }
        
        std::cout << "\n";
    }
};

int main(int argc, char* argv[]) {
    std::cout << "=== C++17 Filesystem Disk Usage Analyzer ===\n";
    
    try {
        // Determine directory to analyze
        fs::path target_path;
        
        if (argc > 1) {
            target_path = argv[1];
        } else {
            // Default to current directory
            target_path = fs::current_path();
        }
        
        std::cout << "Analyzing disk usage for: " << target_path << "\n";
        std::cout << "This may take a while for large directories...\n";
        
        // Create analyzer and run analysis
        DiskUsageAnalyzer analyzer(target_path);
        analyzer.analyze();
        
        // Print results
        analyzer.print_results();
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
    
    return 0;
}

/**
 * === INTERVIEW GUIDE: Disk Usage Analyzer ===
 * 
 * Key Concepts to Understand:
 * 
 * 1. Recursive Directory Analysis:
 *    - Time Complexity: O(n) where n is the number of files and directories
 *    - Space Complexity: O(d + e + f) where:
 *      - d is the number of directories (for directory_sizes_ map)
 *      - e is the number of unique file extensions (for extension_stats_)
 *      - f is the number of large files (for large_files_ vector)
 * 
 * 2. Performance Considerations:
 *    - Directory traversal is I/O bound, not CPU bound
 *    - File system caching can significantly impact performance
 *    - For very large directory trees, memory usage can become a concern
 * 
 * 3. Common Interview Questions:
 * 
 *    Q: How would you optimize this program for extremely large directory structures?
 *    A: Several approaches:
 *       - Use memory-mapped files for intermediate storage
 *       - Implement a streaming approach rather than storing all results in memory
 *       - Use worker threads to parallelize directory scanning
 *       - Add a progress indicator for better user experience
 *       - Implement partial scanning with incremental results
 * 
 *    Q: How would you modify this program to handle symbolic links?
 *    A: Add options to:
 *       - Skip symbolic links entirely
 *       - Follow symbolic links but detect cycles
 *       - Count symbolic links only once even if they appear multiple times
 *       - Track and report symbolic links separately
 * 
 *    Q: What's the difference between calculating disk usage with file sizes vs. block sizes?
 *    A: File size is the logical size of the file's contents, while block size accounts
 *       for actual storage allocation (usually rounded up to block boundaries, typically
 *       4KB). For accurate disk usage, you should calculate based on allocated blocks,
 *       but C++ filesystem doesn't provide this directly - you'd need platform-specific
 *       code or approximations.
 * 
 *    Q: How would you implement this to run incrementally on very large directory structures?
 *    A: - Use a task queue to process directories in chunks
 *       - Save intermediate results to disk
 *       - Implement resumable analysis
 *       - Add priority to scan high-level directories first for quick feedback
 * 
 *    Q: How would you make this analysis concurrent to improve performance?
 *    A: - Use a thread pool to scan multiple directories in parallel
 *       - Use atomic counters for statistics
 *       - Use thread-safe containers for results
 *       - Implement work-stealing for load balancing
 * 
 * 4. Edge Cases to Consider:
 *    - Permission denied errors when accessing certain directories
 *    - Very large files that exceed standard integer ranges
 *    - File sizes changing during analysis
 *    - Files being added or removed during analysis
 *    - Extremely deep directory structures that may cause stack overflow
 * 
 * 5. Thread Safety Considerations:
 *    - The current implementation is not thread-safe
 *    - To make it thread-safe, you would need:
 *      - Mutex protection for shared data structures
 *      - Atomic counters for file_count_, dir_count_, etc.
 *      - Thread-safe containers or synchronized access
 * 
 * 6. Visual Representation Improvements:
 *    - Add a tree view of directory structure
 *    - Generate visualizations like treemaps or sunburst diagrams
 *    - Implement color-coding based on file types or sizes
 */
