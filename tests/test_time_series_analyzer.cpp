/**
 * Unit tests for the Time Series Analyzer
 * 
 * This file contains comprehensive tests for the time series analyzer components,
 * including normal operation, edge cases, and boundary conditions.
 */

#include <iostream>
#include <cassert>
#include <vector>
#include <chrono>
#include <thread>
#include <random>
#include <iomanip>
#include <optional>
#include <memory>
#include <functional>
#include <atomic>
#include <mutex>

// Class definitions from time_series_analyzer.cpp
// Note: In a production environment, these would be in header files

class DataPoint {
public:
    using TimePoint = std::chrono::time_point<std::chrono::system_clock>;
    
    DataPoint(double value, TimePoint timestamp = std::chrono::system_clock::now())
        : value_(value), timestamp_(timestamp) {}
    
    DataPoint() : value_(0.0), timestamp_(std::chrono::system_clock::now()) {}
    
    double getValue() const { return value_; }
    TimePoint getTimestamp() const { return timestamp_; }
    
    int64_t getTimeDiffMs(const DataPoint& other) const {
        return std::chrono::duration_cast<std::chrono::milliseconds>(
            timestamp_ - other.getTimestamp()).count();
    }
    
private:
    double value_;
    TimePoint timestamp_;
};

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
        result.reserve(size_);
        
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
    size_t head_ = 0;
    size_t tail_ = 0;
    size_t size_ = 0;
};

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
};

// Test functions
void test_data_point() {
    std::cout << "Testing DataPoint..." << std::endl;
    
    // Test constructor and getters
    DataPoint dp1(42.0);
    assert(dp1.getValue() == 42.0);
    
    // Test custom timestamp
    auto now = std::chrono::system_clock::now();
    DataPoint dp2(99.9, now);
    assert(dp2.getValue() == 99.9);
    assert(dp2.getTimestamp() == now);
    
    // Test time difference calculation
    auto later = now + std::chrono::milliseconds(500);
    DataPoint dp3(55.5, later);
    assert(dp3.getTimeDiffMs(dp2) == 500);
    assert(dp2.getTimeDiffMs(dp3) == -500);
    
    std::cout << "DataPoint tests passed!" << std::endl;
}

void test_circular_buffer_basic() {
    std::cout << "Testing CircularBuffer basic operations..." << std::endl;
    
    CircularBuffer buffer(5);
    assert(buffer.capacity() == 5);
    assert(buffer.size() == 0);
    
    // Test push and get
    DataPoint dp1(1.0);
    buffer.push(dp1);
    assert(buffer.size() == 1);
    
    auto result = buffer.get(0);
    assert(result.has_value());
    assert(result.value().getValue() == 1.0);
    
    // Test out of bounds access
    result = buffer.get(1);
    assert(!result.has_value());
    
    std::cout << "CircularBuffer basic tests passed!" << std::endl;
}

void test_circular_buffer_wrapping() {
    std::cout << "Testing CircularBuffer wrapping behavior..." << std::endl;
    
    CircularBuffer buffer(3);
    
    // Fill the buffer
    buffer.push(DataPoint(1.0));
    buffer.push(DataPoint(2.0));
    buffer.push(DataPoint(3.0));
    assert(buffer.size() == 3);
    
    // Test values before wrapping
    assert(buffer.get(0).value().getValue() == 1.0);
    assert(buffer.get(1).value().getValue() == 2.0);
    assert(buffer.get(2).value().getValue() == 3.0);
    
    // Add more elements to cause wrapping
    buffer.push(DataPoint(4.0));
    assert(buffer.size() == 3);
    
    // Test values after wrapping
    assert(buffer.get(0).value().getValue() == 2.0);
    assert(buffer.get(1).value().getValue() == 3.0);
    assert(buffer.get(2).value().getValue() == 4.0);
    
    // Add two more elements
    buffer.push(DataPoint(5.0));
    buffer.push(DataPoint(6.0));
    
    // Test values after multiple wraps
    assert(buffer.get(0).value().getValue() == 4.0);
    assert(buffer.get(1).value().getValue() == 5.0);
    assert(buffer.get(2).value().getValue() == 6.0);
    
    std::cout << "CircularBuffer wrapping tests passed!" << std::endl;
}

void test_circular_buffer_range() {
    std::cout << "Testing CircularBuffer range operations..." << std::endl;
    
    CircularBuffer buffer(5);
    
    // Fill the buffer
    for (int i = 1; i <= 5; i++) {
        buffer.push(DataPoint(static_cast<double>(i)));
    }
    
    // Test getRange
    auto range = buffer.getRange(1, 4);
    assert(range.size() == 3);
    assert(range[0].getValue() == 2.0);
    assert(range[1].getValue() == 3.0);
    assert(range[2].getValue() == 4.0);
    
    // Test invalid ranges
    range = buffer.getRange(3, 2);  // start > end
    assert(range.empty());
    
    range = buffer.getRange(5, 7);  // start >= size
    assert(range.empty());
    
    // Test range that exceeds buffer size
    range = buffer.getRange(2, 10);
    assert(range.size() == 3);  // Should only return elements 2, 3, 4
    
    std::cout << "CircularBuffer range tests passed!" << std::endl;
}

void test_circular_buffer_time_window() {
    std::cout << "Testing CircularBuffer time window operations..." << std::endl;
    
    CircularBuffer buffer(5);
    
    // Create data points with specific timestamps
    auto base_time = std::chrono::system_clock::now();
    buffer.push(DataPoint(1.0, base_time - std::chrono::milliseconds(200)));
    buffer.push(DataPoint(2.0, base_time - std::chrono::milliseconds(100)));
    buffer.push(DataPoint(3.0, base_time));
    buffer.push(DataPoint(4.0, base_time + std::chrono::milliseconds(100)));
    buffer.push(DataPoint(5.0, base_time + std::chrono::milliseconds(200)));
    
    // Test time window around the middle point
    auto window = buffer.getTimeWindow(
        std::chrono::milliseconds(150),  // 150ms before
        std::chrono::milliseconds(150),  // 150ms after
        2  // reference index (value 3.0)
    );
    
    assert(window.size() == 3);  // Should include points 2, 3, 4
    assert(window[0].getValue() == 2.0);
    assert(window[1].getValue() == 3.0);
    assert(window[2].getValue() == 4.0);
    
    // Test smaller window
    window = buffer.getTimeWindow(
        std::chrono::milliseconds(50),  // 50ms before
        std::chrono::milliseconds(50),  // 50ms after
        2  // reference index (value 3.0)
    );
    
    assert(window.size() == 1);  // Should only include point 3
    assert(window[0].getValue() == 3.0);
    
    // Test invalid reference index
    window = buffer.getTimeWindow(
        std::chrono::milliseconds(100),
        std::chrono::milliseconds(100),
        10  // out of bounds
    );
    assert(window.empty());
    
    std::cout << "CircularBuffer time window tests passed!" << std::endl;
}

void test_data_analyzer() {
    std::cout << "Testing DataAnalyzer..." << std::endl;
    
    // Create test data
    std::vector<DataPoint> data = {
        DataPoint(2.0),
        DataPoint(4.0),
        DataPoint(4.0),
        DataPoint(4.0),
        DataPoint(5.0),
        DataPoint(5.0),
        DataPoint(7.0),
        DataPoint(9.0)
    };
    
    // Test mean calculation
    double mean = DataAnalyzer::calculateMean(data);
    assert(std::abs(mean - 5.0) < 0.001);
    
    // Test median calculation
    double median = DataAnalyzer::calculateMedian(data);
    assert(std::abs(median - 4.5) < 0.001);
    
    // Test standard deviation calculation
    double stddev = DataAnalyzer::calculateStdDev(data);
    assert(std::abs(stddev - 2.138) < 0.01);
    
    // Test edge cases
    std::vector<DataPoint> empty_data;
    assert(DataAnalyzer::calculateMean(empty_data) == 0.0);
    assert(DataAnalyzer::calculateMedian(empty_data) == 0.0);
    assert(DataAnalyzer::calculateStdDev(empty_data) == 0.0);
    
    std::vector<DataPoint> single_data = { DataPoint(42.0) };
    assert(DataAnalyzer::calculateMean(single_data) == 42.0);
    assert(DataAnalyzer::calculateMedian(single_data) == 42.0);
    assert(DataAnalyzer::calculateStdDev(single_data) == 0.0);
    
    std::cout << "DataAnalyzer tests passed!" << std::endl;
}

void test_thread_safety() {
    std::cout << "Testing thread safety..." << std::endl;
    
    CircularBuffer buffer(1000);
    std::atomic<bool> running(true);
    std::atomic<int> push_count(0);
    std::atomic<int> get_count(0);
    std::atomic<int> errors(0);
    
    // Producer thread
    std::thread producer([&]() {
        for (int i = 0; i < 1000 && running; i++) {
            buffer.push(DataPoint(static_cast<double>(i)));
            push_count++;
            std::this_thread::sleep_for(std::chrono::microseconds(10));
        }
    });
    
    // Consumer threads
    std::vector<std::thread> consumers;
    for (int t = 0; t < 3; t++) {
        consumers.emplace_back([&, t]() {
            for (int i = 0; i < 300 && running; i++) {
                size_t idx = i % buffer.size();
                if (idx < buffer.size()) {
                    auto result = buffer.get(idx);
                    if (result.has_value()) {
                        get_count++;
                    }
                }
                std::this_thread::sleep_for(std::chrono::microseconds(50));
            }
        });
    }
    
    // Range query thread
    std::thread range_thread([&]() {
        for (int i = 0; i < 100 && running; i++) {
            size_t size = buffer.size();
            if (size > 2) {
                size_t start = size / 3;
                size_t end = 2 * size / 3;
                auto range = buffer.getRange(start, end);
                if (range.size() != end - start) {
                    errors++;
                }
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(5));
        }
    });
    
    // Let threads run for a short time
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    running = false;
    
    // Join all threads
    producer.join();
    for (auto& t : consumers) {
        t.join();
    }
    range_thread.join();
    
    std::cout << "Thread safety test completed:" << std::endl;
    std::cout << "  Push operations: " << push_count << std::endl;
    std::cout << "  Get operations: " << get_count << std::endl;
    std::cout << "  Range errors: " << errors << std::endl;
    std::cout << "  Final buffer size: " << buffer.size() << std::endl;
    
    // No assertions here as exact counts depend on timing, but we should have no errors
    assert(errors == 0);
    
    std::cout << "Thread safety tests passed!" << std::endl;
}

int main() {
    std::cout << "=== Time Series Analyzer Tests ===" << std::endl;
    
    // Run all tests
    test_data_point();
    test_circular_buffer_basic();
    test_circular_buffer_wrapping();
    test_circular_buffer_range();
    test_circular_buffer_time_window();
    test_data_analyzer();
    test_thread_safety();
    
    std::cout << "\nAll tests passed successfully!" << std::endl;
    return 0;
}
