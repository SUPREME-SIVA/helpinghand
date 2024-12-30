#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <atomic>
#include <numeric>

/**
 * Calculates the sum of numbers in a range using parallel processing
 * @param first Starting number of the range (inclusive)
 * @param last Ending number of the range (exclusive)
 * @return The sum of all numbers in the range
 */
size_t parallel_sum(size_t first, size_t last) {
    // Determine optimal thread count based on hardware
    const size_t thread_count = std::thread::hardware_concurrency() > 0 ? 
                              std::thread::hardware_concurrency() : 4;
    
    // Calculate input size and chunk size for each thread
    const size_t input_size = last - first;
    const size_t chunk_size = input_size / thread_count;
    
    // Vector to store partial sums from each thread
    std::vector<size_t> partial_sums(thread_count, 0);
    
    // Vector to store thread objects
    std::vector<std::thread> processing_threads;
    processing_threads.reserve(thread_count);  // Prevent reallocation

    // Create and launch threads
    for (size_t thread_id = 0; thread_id < thread_count; thread_id++) {
        processing_threads.emplace_back(
            [first, thread_id, chunk_size, &partial_sums]() {
                // Calculate range for this thread
                size_t start = first + (thread_id * chunk_size);
                size_t end = first + ((thread_id + 1) * chunk_size);
                
                // Calculate partial sum for this chunk
                size_t local_sum = 0;
                for (size_t number = start; number < end; number++) {
                    local_sum += number;
                }
                
                // Store result in thread-specific slot
                partial_sums[thread_id] = local_sum;
            }
        );
    }

    // Handle any remaining elements in the last chunk
    size_t remaining_start = first + (thread_count * chunk_size);
    size_t remaining_sum = 0;
    for (size_t i = remaining_start; i < last; i++) {
        remaining_sum += i;
    }

    // Wait for all threads to complete
    for (auto& thread : processing_threads) {
        thread.join();
    }

    // Sum up all partial results
    size_t total_sum = remaining_sum;
    for (size_t partial_sum : partial_sums) {
        total_sum += partial_sum;
    }

    return total_sum;
}

// Function to calculate sum sequentially for verification
size_t sequential_sum(size_t first, size_t last) {
    size_t sum = 0;
    for (size_t i = first; i < last; i++) {
        sum += i;
    }
    return sum;
}

int main() {
    const size_t RANGE_END = 1000000;
    
    // Calculate and verify results
    size_t parallel_result = parallel_sum(0, RANGE_END);
    size_t sequential_result = sequential_sum(0, RANGE_END);
    
    std::cout << "Parallel sum result: " << parallel_result << "\n";
    std::cout << "Sequential sum result: " << sequential_result << "\n";
    std::cout << "Results match: " << (parallel_result == sequential_result ? "Yes" : "No") << "\n";
    
    return 0;
}
