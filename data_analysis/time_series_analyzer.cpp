/**
 * Time Series Data Analyzer
 * ========================
 * 
 * A system design for analyzing high-frequency data points with historical context.
 * This implementation handles a data source producing hundreds of data points every 500ms
 * and provides efficient access to both past and future data relative to any point.
 * 
 * Features:
 * - Thread-safe circular buffer for efficient storage and retrieval
 * - Configurable history size
 * - Time-based and index-based data access
 * - Statistical analysis of time windows
 * - Memory-efficient design for high-frequency data
 * 
 * Modern C++ Implementation (C++17)
 */

#include <iostream>
#include <vector>
#include <deque>
#include <chrono>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <algorithm>
#include <numeric>
#include <functional>
#include <optional>
#include <memory>
#include <unordered_map>
#include <string>
#include <cmath>
#include <random>
#include <iomanip>

// Forward declarations
class DataPoint;
class CircularBuffer;
class DataSource;
class DataAnalyzer;
class HistoricalAnalysis;

/**
 * DataPoint class to encapsulate a single data measurement
 */
class DataPoint {
public:
    using TimePoint = std::chrono::time_point<std::chrono::system_clock>;
    
    DataPoint(double value, TimePoint timestamp = std::chrono::system_clock::now())
        : value_(value), timestamp_(timestamp) {}
    
    // Default constructor needed for vector initialization
    DataPoint() : value_(0.0), timestamp_(std::chrono::system_clock::now()) {}
    
    double getValue() const { return value_; }
    TimePoint getTimestamp() const { return timestamp_; }
    
    // Time difference in milliseconds between this point and another
    int64_t getTimeDiffMs(const DataPoint& other) const {
        return std::chrono::duration_cast<std::chrono::milliseconds>(
            timestamp_ - other.getTimestamp()).count();
    }
    
    friend std::ostream& operator<<(std::ostream& os, const DataPoint& dp) {
        auto time_t = std::chrono::system_clock::to_time_t(dp.timestamp_);
        os << "Value: " << dp.value_ << ", Time: " 
           << std::put_time(std::localtime(&time_t), "%Y-%m-%d %H:%M:%S");
        return os;
    }
    
private:
    double value_;
    TimePoint timestamp_;
};

/**
 * Thread-safe circular buffer for efficient storage and retrieval of DataPoints
 */
class CircularBuffer {
public:
    explicit CircularBuffer(size_t capacity) 
        : capacity_(capacity), buffer_(capacity) {}
    
    void push(const DataPoint& data) {
        std::lock_guard<std::mutex> lock(mutex_);
        
        if (size_ < capacity_) {
            buffer_[tail_] = data;
            tail_ = (tail_ + 1) % capacity_;
            size_++;
        } else {
            buffer_[head_] = data;
            head_ = (head_ + 1) % capacity_;
            tail_ = (tail_ + 1) % capacity_;
        }
    }
    
    std::optional<DataPoint> get(size_t index) const {
        std::lock_guard<std::mutex> lock(mutex_);
        
        if (index >= size_) {
            return std::nullopt;
        }
        
        size_t actual_index = (head_ + index) % capacity_;
        return buffer_[actual_index];
    }
    
    // Get a range of data points [start_idx, end_idx)
    std::vector<DataPoint> getRange(size_t start_idx, size_t end_idx) const {
        std::lock_guard<std::mutex> lock(mutex_);
        
        if (start_idx >= size_ || start_idx >= end_idx) {
            return {};
        }
        
        end_idx = std::min(end_idx, size_);
        std::vector<DataPoint> result;
        result.reserve(end_idx - start_idx);
        
        for (size_t i = start_idx; i < end_idx; ++i) {
            size_t actual_index = (head_ + i) % capacity_;
            result.push_back(buffer_[actual_index]);
        }
        
        return result;
    }
    
    // Get data points within a specific time window
    std::vector<DataPoint> getTimeWindow(
        std::chrono::milliseconds before,
        std::chrono::milliseconds after,
        size_t reference_idx) const {
        
        std::lock_guard<std::mutex> lock(mutex_);
        
        if (reference_idx >= size_) {
            return {};
        }
        
        size_t actual_ref_idx = (head_ + reference_idx) % capacity_;
        const DataPoint& reference = buffer_[actual_ref_idx];
        
        std::vector<DataPoint> result;
        result.reserve(size_); // Pre-allocate for worst case
        
        for (size_t i = 0; i < size_; ++i) {
            size_t actual_index = (head_ + i) % capacity_;
            const DataPoint& dp = buffer_[actual_index];
            
            auto time_diff = std::chrono::duration_cast<std::chrono::milliseconds>(
                dp.getTimestamp() - reference.getTimestamp());
            
            if (time_diff >= -before && time_diff <= after) {
                result.push_back(dp);
            }
        }
        
        return result;
    }
    
    size_t size() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return size_;
    }
    
    size_t capacity() const {
        return capacity_;
    }
    
    void clear() {
        std::lock_guard<std::mutex> lock(mutex_);
        head_ = 0;
        tail_ = 0;
        size_ = 0;
    }
    
private:
    size_t capacity_;
    mutable std::mutex mutex_;
    std::vector<DataPoint> buffer_;
    size_t head_ = 0;  // Index of the oldest element
    size_t tail_ = 0;  // Index where the next element will be inserted
    size_t size_ = 0;  // Current number of elements
};

/**
 * Simulates a high-frequency data source
 */
class DataSource {
public:
    using DataCallback = std::function<void(const DataPoint&)>;
    
    DataSource(int frequency_ms = 500, double min_value = 0.0, double max_value = 100.0)
        : frequency_ms_(frequency_ms), min_value_(min_value), max_value_(max_value),
          running_(false) {}
    
    ~DataSource() {
        stop();
    }
    
    void registerCallback(DataCallback callback) {
        callbacks_.push_back(std::move(callback));
    }
    
    void start() {
        if (running_) return;
        
        running_ = true;
        thread_ = std::thread([this]() {
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_real_distribution<double> dis(min_value_, max_value_);
            
            while (running_) {
                // Generate multiple data points per cycle
                for (int i = 0; i < data_points_per_cycle_; ++i) {
                    DataPoint dp(dis(gen));
                    
                    // Notify all registered callbacks
                    for (const auto& callback : callbacks_) {
                        callback(dp);
                    }
                    
                    // Small delay between data points in the same cycle
                    if (i < data_points_per_cycle_ - 1) {
                        std::this_thread::sleep_for(std::chrono::milliseconds(1));
                    }
                }
                
                // Wait for the next cycle
                std::this_thread::sleep_for(std::chrono::milliseconds(frequency_ms_));
            }
        });
    }
    
    void stop() {
        if (!running_) return;
        
        running_ = false;
        if (thread_.joinable()) {
            thread_.join();
        }
    }
    
    void setFrequency(int frequency_ms) {
        frequency_ms_ = frequency_ms;
    }
    
    void setDataPointsPerCycle(int count) {
        data_points_per_cycle_ = count;
    }
    
private:
    int frequency_ms_;
    double min_value_;
    double max_value_;
    int data_points_per_cycle_ = 100;  // Number of data points per cycle
    std::atomic<bool> running_;
    std::thread thread_;
    std::vector<DataCallback> callbacks_;
};

/**
 * Statistical analysis utilities for data points
 */
class DataAnalyzer {
public:
    static double calculateMean(const std::vector<DataPoint>& data) {
        if (data.empty()) return 0.0;
        
        double sum = 0.0;
        for (const auto& dp : data) {
            sum += dp.getValue();
        }
        return sum / data.size();
    }
    
    static double calculateStdDev(const std::vector<DataPoint>& data) {
        if (data.size() <= 1) return 0.0;
        
        double mean = calculateMean(data);
        double sum_squared_diff = 0.0;
        
        for (const auto& dp : data) {
            double diff = dp.getValue() - mean;
            sum_squared_diff += diff * diff;
        }
        
        return std::sqrt(sum_squared_diff / (data.size() - 1));
    }
    
    static double calculateMedian(const std::vector<DataPoint>& data) {
        if (data.empty()) return 0.0;
        
        std::vector<double> values;
        values.reserve(data.size());
        
        for (const auto& dp : data) {
            values.push_back(dp.getValue());
        }
        
        std::sort(values.begin(), values.end());
        
        if (values.size() % 2 == 0) {
            return (values[values.size()/2 - 1] + values[values.size()/2]) / 2.0;
        } else {
            return values[values.size()/2];
        }
    }
    
    static std::pair<double, double> calculateMinMax(const std::vector<DataPoint>& data) {
        if (data.empty()) return {0.0, 0.0};
        
        double min_val = data[0].getValue();
        double max_val = data[0].getValue();
        
        for (const auto& dp : data) {
            min_val = std::min(min_val, dp.getValue());
            max_val = std::max(max_val, dp.getValue());
        }
        
        return {min_val, max_val};
    }
    
    static double calculateSlope(const std::vector<DataPoint>& data) {
        if (data.size() <= 1) return 0.0;
        
        // Simple linear regression
        double sum_x = 0.0, sum_y = 0.0, sum_xy = 0.0, sum_xx = 0.0;
        double n = static_cast<double>(data.size());
        
        // Use time differences in milliseconds from the first point
        auto base_time = data[0].getTimestamp();
        
        for (const auto& dp : data) {
            double x = std::chrono::duration_cast<std::chrono::milliseconds>(
                dp.getTimestamp() - base_time).count();
            double y = dp.getValue();
            
            sum_x += x;
            sum_y += y;
            sum_xy += x * y;
            sum_xx += x * x;
        }
        
        // Calculate slope (change per millisecond)
        return (n * sum_xy - sum_x * sum_y) / (n * sum_xx - sum_x * sum_x);
    }
};

/**
 * Main class for historical data analysis
 */
class HistoricalAnalysis {
public:
    HistoricalAnalysis(size_t buffer_capacity = 10000)
        : buffer_(buffer_capacity) {
        
        // Register callback with data source
        data_source_.registerCallback([this](const DataPoint& dp) {
            this->onNewDataPoint(dp);
        });
    }
    
    void start() {
        data_source_.setDataPointsPerCycle(100);  // 100 data points every cycle
        data_source_.start();
    }
    
    void stop() {
        data_source_.stop();
    }
    
    // Analyze data around a specific index
    void analyzeAroundIndex(size_t index, size_t points_before, size_t points_after) {
        size_t start_idx = (index >= points_before) ? (index - points_before) : 0;
        size_t end_idx = std::min(index + points_after + 1, buffer_.size());
        
        auto data = buffer_.getRange(start_idx, end_idx);
        printAnalysis(data, "Index-based");
    }
    
    // Analyze data around a specific time window
    void analyzeTimeWindow(size_t reference_idx, 
                          std::chrono::milliseconds before,
                          std::chrono::milliseconds after) {
        auto data = buffer_.getTimeWindow(before, after, reference_idx);
        printAnalysis(data, "Time-based");
    }
    
    // Get the most recent data point index
    size_t getCurrentIndex() const {
        return buffer_.size() - 1;
    }
    
    // Print buffer statistics
    void printBufferStats() const {
        std::cout << "Buffer capacity: " << buffer_.capacity() << std::endl;
        std::cout << "Buffer current size: " << buffer_.size() << std::endl;
    }
    
private:
    void onNewDataPoint(const DataPoint& dp) {
        buffer_.push(dp);
    }
    
    void printAnalysis(const std::vector<DataPoint>& data, const std::string& analysis_type) {
        if (data.empty()) {
            std::cout << "No data available for analysis." << std::endl;
            return;
        }
        
        std::cout << "\n=== " << analysis_type << " Analysis ===\n";
        std::cout << "Data points: " << data.size() << std::endl;
        
        auto [min_val, max_val] = DataAnalyzer::calculateMinMax(data);
        std::cout << "Min value: " << min_val << std::endl;
        std::cout << "Max value: " << max_val << std::endl;
        std::cout << "Mean: " << DataAnalyzer::calculateMean(data) << std::endl;
        std::cout << "Median: " << DataAnalyzer::calculateMedian(data) << std::endl;
        std::cout << "Standard Deviation: " << DataAnalyzer::calculateStdDev(data) << std::endl;
        std::cout << "Slope (change/ms): " << DataAnalyzer::calculateSlope(data) << std::endl;
        
        // Print first and last few points
        size_t preview_count = std::min(size_t(3), data.size());
        std::cout << "\nFirst " << preview_count << " points:" << std::endl;
        for (size_t i = 0; i < preview_count; ++i) {
            std::cout << "  " << data[i] << std::endl;
        }
        
        if (data.size() > 2 * preview_count) {
            std::cout << "  ..." << std::endl;
        }
        
        if (data.size() > preview_count) {
            std::cout << "Last " << preview_count << " points:" << std::endl;
            for (size_t i = data.size() - preview_count; i < data.size(); ++i) {
                std::cout << "  " << data[i] << std::endl;
            }
        }
        
        std::cout << "==========================\n" << std::endl;
    }
    
    CircularBuffer buffer_;
    DataSource data_source_;
};

/**
 * Main function demonstrating the system
 */
int main() {
    std::cout << "=== High-Frequency Data Analysis System ===" << std::endl;
    
    // Create analyzer with a buffer capacity of 10000 data points
    HistoricalAnalysis analyzer(10000);
    
    std::cout << "Starting data collection..." << std::endl;
    analyzer.start();
    
    // Wait for some data to accumulate
    std::this_thread::sleep_for(std::chrono::seconds(3));
    
    // Get current index for reference
    size_t current_idx = analyzer.getCurrentIndex();
    std::cout << "Current data point index: " << current_idx << std::endl;
    
    // Analyze data around the current index
    analyzer.analyzeAroundIndex(current_idx, 50, 0);  // 50 points before, 0 after
    
    // Wait for more data to accumulate
    std::this_thread::sleep_for(std::chrono::seconds(2));
    
    // Now we have data after our reference point
    size_t new_current_idx = analyzer.getCurrentIndex();
    std::cout << "New current data point index: " << new_current_idx << std::endl;
    
    // Analyze data around the original reference point, now with data before and after
    analyzer.analyzeAroundIndex(current_idx, 50, 50);
    
    // Analyze using time windows
    analyzer.analyzeTimeWindow(current_idx, 
                              std::chrono::milliseconds(1000),  // 1 second before
                              std::chrono::milliseconds(1000)); // 1 second after
    
    // Print buffer statistics
    analyzer.printBufferStats();
    
    // Stop data collection
    analyzer.stop();
    
    std::cout << "Data collection stopped." << std::endl;
    return 0;
}
