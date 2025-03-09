/**
 * @file array_sorting.cpp
 * @brief Templated implementation of various sorting algorithms
 *
 * This program demonstrates how to implement various sorting algorithms
 * in C++ using templates, without relying on any built-in sorting functions.
 * It provides a flexible way to sort arrays of different types.
 */

#include <iostream>
#include <chrono>
#include <string>
#include <utility>
#include <cassert>

/**
 * @brief Namespace containing various sorting algorithms
 */
namespace sorting {

/**
 * @brief Swap two elements using move semantics
 * 
 * @tparam T Type of elements to swap
 * @param a First element
 * @param b Second element
 */
template <typename T>
void swap(T& a, T& b) {
    T temp = std::move(a);
    a = std::move(b);
    b = std::move(temp);
}

/**
 * @brief Bubble sort implementation
 * 
 * Time Complexity: O(n²) in worst and average case
 * Space Complexity: O(1)
 * Stable: Yes
 * 
 * @tparam T Type of array elements
 * @param arr Array to sort
 * @param size Size of the array
 */
template <typename T>
void bubble_sort(T* arr, size_t size) {
    if (!arr || size <= 1) return;
    
    for (size_t i = 0; i < size - 1; ++i) {
        bool swapped = false;
        
        for (size_t j = 0; j < size - i - 1; ++j) {
            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
                swapped = true;
            }
        }
        
        // If no swaps were made in this pass, array is sorted
        if (!swapped) break;
    }
}

/**
 * @brief Selection sort implementation
 * 
 * Time Complexity: O(n²) in all cases
 * Space Complexity: O(1)
 * Stable: No
 * 
 * @tparam T Type of array elements
 * @param arr Array to sort
 * @param size Size of the array
 */
template <typename T>
void selection_sort(T* arr, size_t size) {
    if (!arr || size <= 1) return;
    
    for (size_t i = 0; i < size - 1; ++i) {
        size_t min_idx = i;
        
        // Find the minimum element in the unsorted part
        for (size_t j = i + 1; j < size; ++j) {
            if (arr[j] < arr[min_idx]) {
                min_idx = j;
            }
        }
        
        // Swap the found minimum element with the element at position i
        if (min_idx != i) {
            swap(arr[i], arr[min_idx]);
        }
    }
}

/**
 * @brief Insertion sort implementation
 * 
 * Time Complexity: O(n²) worst case, O(n) best case (already sorted)
 * Space Complexity: O(1)
 * Stable: Yes
 * 
 * @tparam T Type of array elements
 * @param arr Array to sort
 * @param size Size of the array
 */
template <typename T>
void insertion_sort(T* arr, size_t size) {
    if (!arr || size <= 1) return;
    
    for (size_t i = 1; i < size; ++i) {
        T key = std::move(arr[i]);
        int j = static_cast<int>(i) - 1;
        
        // Move elements greater than key to one position ahead
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = std::move(arr[j]);
            --j;
        }
        
        arr[j + 1] = std::move(key);
    }
}

/**
 * @brief Merge two sorted subarrays
 * 
 * @tparam T Type of array elements
 * @param arr Array containing both subarrays
 * @param left Start index of the first subarray
 * @param mid End index of the first subarray
 * @param right End index of the second subarray
 */
template <typename T>
void merge(T* arr, size_t left, size_t mid, size_t right) {
    // Calculate sizes of the two subarrays
    size_t n1 = mid - left + 1;
    size_t n2 = right - mid;
    
    // Create temporary arrays
    T* L = new T[n1];
    T* R = new T[n2];
    
    // Copy data to temporary arrays
    for (size_t i = 0; i < n1; ++i) {
        L[i] = std::move(arr[left + i]);
    }
    for (size_t j = 0; j < n2; ++j) {
        R[j] = std::move(arr[mid + 1 + j]);
    }
    
    // Merge the temporary arrays back into arr[left..right]
    size_t i = 0, j = 0, k = left;
    
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = std::move(L[i]);
            ++i;
        } else {
            arr[k] = std::move(R[j]);
            ++j;
        }
        ++k;
    }
    
    // Copy the remaining elements of L[], if any
    while (i < n1) {
        arr[k] = std::move(L[i]);
        ++i;
        ++k;
    }
    
    // Copy the remaining elements of R[], if any
    while (j < n2) {
        arr[k] = std::move(R[j]);
        ++j;
        ++k;
    }
    
    // Free temporary arrays
    delete[] L;
    delete[] R;
}

/**
 * @brief Merge sort implementation (recursive)
 * 
 * Time Complexity: O(n log n) in all cases
 * Space Complexity: O(n)
 * Stable: Yes
 * 
 * @tparam T Type of array elements
 * @param arr Array to sort
 * @param left Start index
 * @param right End index
 */
template <typename T>
void merge_sort_impl(T* arr, size_t left, size_t right) {
    if (left < right) {
        // Find the middle point
        size_t mid = left + (right - left) / 2;
        
        // Sort first and second halves
        merge_sort_impl(arr, left, mid);
        merge_sort_impl(arr, mid + 1, right);
        
        // Merge the sorted halves
        merge(arr, left, mid, right);
    }
}

/**
 * @brief Merge sort wrapper function
 * 
 * @tparam T Type of array elements
 * @param arr Array to sort
 * @param size Size of the array
 */
template <typename T>
void merge_sort(T* arr, size_t size) {
    if (!arr || size <= 1) return;
    
    merge_sort_impl(arr, 0, size - 1);
}

/**
 * @brief Partition the array for quicksort
 * 
 * @tparam T Type of array elements
 * @param arr Array to partition
 * @param low Starting index
 * @param high Ending index
 * @return Partition index
 */
template <typename T>
size_t partition(T* arr, size_t low, size_t high) {
    // Choose the rightmost element as pivot
    T pivot = arr[high];
    
    // Index of smaller element
    size_t i = low;
    
    for (size_t j = low; j < high; ++j) {
        // If current element is smaller than the pivot
        if (arr[j] < pivot) {
            swap(arr[i], arr[j]);
            ++i;
        }
    }
    
    // Place the pivot in its correct position
    swap(arr[i], arr[high]);
    return i;
}

/**
 * @brief Quicksort implementation (recursive)
 * 
 * Time Complexity: O(n²) worst case, O(n log n) average case
 * Space Complexity: O(log n) for recursion stack
 * Stable: No
 * 
 * @tparam T Type of array elements
 * @param arr Array to sort
 * @param low Starting index
 * @param high Ending index
 */
template <typename T>
void quick_sort_impl(T* arr, size_t low, size_t high) {
    if (low < high) {
        // Partition the array
        size_t pi = partition(arr, low, high);
        
        // Sort elements before and after partition
        if (pi > 0) {
            quick_sort_impl(arr, low, pi - 1);
        }
        quick_sort_impl(arr, pi + 1, high);
    }
}

/**
 * @brief Quicksort wrapper function
 * 
 * @tparam T Type of array elements
 * @param arr Array to sort
 * @param size Size of the array
 */
template <typename T>
void quick_sort(T* arr, size_t size) {
    if (!arr || size <= 1) return;
    
    quick_sort_impl(arr, 0, size - 1);
}

/**
 * @brief Heapify a subtree rooted with node i
 * 
 * @tparam T Type of array elements
 * @param arr Array to heapify
 * @param size Size of the heap
 * @param i Root index
 */
template <typename T>
void heapify(T* arr, size_t size, size_t i) {
    size_t largest = i;        // Initialize largest as root
    size_t left = 2 * i + 1;   // Left child
    size_t right = 2 * i + 2;  // Right child
    
    // If left child is larger than root
    if (left < size && arr[left] > arr[largest]) {
        largest = left;
    }
    
    // If right child is larger than largest so far
    if (right < size && arr[right] > arr[largest]) {
        largest = right;
    }
    
    // If largest is not root
    if (largest != i) {
        swap(arr[i], arr[largest]);
        
        // Recursively heapify the affected sub-tree
        heapify(arr, size, largest);
    }
}

/**
 * @brief Heap sort implementation
 * 
 * Time Complexity: O(n log n) in all cases
 * Space Complexity: O(1)
 * Stable: No
 * 
 * @tparam T Type of array elements
 * @param arr Array to sort
 * @param size Size of the array
 */
template <typename T>
void heap_sort(T* arr, size_t size) {
    if (!arr || size <= 1) return;
    
    // Build max heap
    for (int i = static_cast<int>(size / 2) - 1; i >= 0; --i) {
        heapify(arr, size, i);
    }
    
    // Extract elements from heap one by one
    for (size_t i = size - 1; i > 0; --i) {
        // Move current root to end
        swap(arr[0], arr[i]);
        
        // Call heapify on the reduced heap
        heapify(arr, i, 0);
    }
}

} // namespace sorting

/**
 * @brief Print the contents of an array
 * 
 * @tparam T Type of array elements
 * @param arr Array to print
 * @param size Size of the array
 * @param name Name of the array (for display)
 */
template <typename T>
void print_array(const T* arr, size_t size, const std::string& name) {
    std::cout << name << ": [";
    for (size_t i = 0; i < size; ++i) {
        std::cout << arr[i];
        if (i < size - 1) {
            std::cout << ", ";
        }
    }
    std::cout << "]" << std::endl;
}

/**
 * @brief Copy an array
 * 
 * @tparam T Type of array elements
 * @param src Source array
 * @param dest Destination array
 * @param size Size of the arrays
 */
template <typename T>
void copy_array(const T* src, T* dest, size_t size) {
    for (size_t i = 0; i < size; ++i) {
        dest[i] = src[i];
    }
}

/**
 * @brief Check if an array is sorted
 * 
 * @tparam T Type of array elements
 * @param arr Array to check
 * @param size Size of the array
 * @return true if sorted, false otherwise
 */
template <typename T>
bool is_sorted(const T* arr, size_t size) {
    if (!arr || size <= 1) return true;
    
    for (size_t i = 1; i < size; ++i) {
        if (arr[i] < arr[i - 1]) {
            return false;
        }
    }
    
    return true;
}

/**
 * @brief Benchmark a sorting algorithm
 * 
 * @tparam T Type of array elements
 * @tparam SortFunc Type of sorting function
 * @param arr Array to sort
 * @param size Size of the array
 * @param sort_func Sorting function to benchmark
 * @param name Name of the sorting algorithm (for display)
 */
template <typename T, typename SortFunc>
void benchmark_sort(const T* arr, size_t size, SortFunc sort_func, const std::string& name) {
    // Create a copy of the array to sort
    T* arr_copy = new T[size];
    copy_array(arr, arr_copy, size);
    
    // Measure sorting time
    auto start = std::chrono::high_resolution_clock::now();
    sort_func(arr_copy, size);
    auto end = std::chrono::high_resolution_clock::now();
    
    // Calculate duration
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    // Verify that the array is sorted
    bool sorted = is_sorted(arr_copy, size);
    
    // Print results
    std::cout << name << ":\n";
    std::cout << "  Time: " << duration.count() << " μs\n";
    std::cout << "  Sorted: " << (sorted ? "Yes" : "No") << "\n";
    
    if (size <= 20) {
        print_array(arr_copy, size, "  Result");
    }
    
    std::cout << std::endl;
    
    // Clean up
    delete[] arr_copy;
}

/**
 * @brief Sort two arrays of different types using a specified algorithm
 * 
 * @tparam T Type of first array elements
 * @tparam U Type of second array elements
 * @tparam SortFunc Type of sorting function
 * @param arr1 First array to sort
 * @param size1 Size of the first array
 * @param arr2 Second array to sort
 * @param size2 Size of the second array
 * @param sort_func Sorting function to use
 * @param name Name of the sorting algorithm (for display)
 */
template <typename T, typename U>
void sort_two_arrays(const T* arr1, size_t size1, const U* arr2, size_t size2, 
                     const std::string& name) {
    std::cout << "=== Sorting with " << name << " ===" << std::endl;
    
    // Sort and benchmark the first array
    std::cout << "First array:" << std::endl;
    
    // Use the appropriate sorting function for type T
    if (name == "Bubble Sort") {
        benchmark_sort(arr1, size1, sorting::bubble_sort<T>, name);
    } else if (name == "Selection Sort") {
        benchmark_sort(arr1, size1, sorting::selection_sort<T>, name);
    } else if (name == "Insertion Sort") {
        benchmark_sort(arr1, size1, sorting::insertion_sort<T>, name);
    } else if (name == "Merge Sort") {
        benchmark_sort(arr1, size1, sorting::merge_sort<T>, name);
    } else if (name == "Quick Sort") {
        benchmark_sort(arr1, size1, sorting::quick_sort<T>, name);
    } else if (name == "Heap Sort") {
        benchmark_sort(arr1, size1, sorting::heap_sort<T>, name);
    }
    
    // Sort and benchmark the second array
    std::cout << "Second array:" << std::endl;
    
    // Use the appropriate sorting function for type U
    if (name == "Bubble Sort") {
        benchmark_sort(arr2, size2, sorting::bubble_sort<U>, name);
    } else if (name == "Selection Sort") {
        benchmark_sort(arr2, size2, sorting::selection_sort<U>, name);
    } else if (name == "Insertion Sort") {
        benchmark_sort(arr2, size2, sorting::insertion_sort<U>, name);
    } else if (name == "Merge Sort") {
        benchmark_sort(arr2, size2, sorting::merge_sort<U>, name);
    } else if (name == "Quick Sort") {
        benchmark_sort(arr2, size2, sorting::quick_sort<U>, name);
    } else if (name == "Heap Sort") {
        benchmark_sort(arr2, size2, sorting::heap_sort<U>, name);
    }
    
    std::cout << "===========================" << std::endl << std::endl;
}

/**
 * @brief The entry point of the program
 */
int main() {
    std::cout << "=== Templated Array Sorting Algorithms ===\n\n";
    
    // Define test arrays of different types
    int int_arr[] = {9, 2, 7, 1, 6, 8, 5, 3, 4, 0};
    size_t int_size = sizeof(int_arr) / sizeof(int_arr[0]);
    
    double double_arr[] = {3.14, 1.41, 2.72, 0.577, 1.618, 2.0, 1.0, 0.0, 9.9, 4.669};
    size_t double_size = sizeof(double_arr) / sizeof(double_arr[0]);
    
    // Display original arrays
    print_array(int_arr, int_size, "Original int array");
    print_array(double_arr, double_size, "Original double array");
    std::cout << std::endl;
    
    // Sort using different algorithms
    sort_two_arrays(int_arr, int_size, double_arr, double_size, "Bubble Sort");
    sort_two_arrays(int_arr, int_size, double_arr, double_size, "Selection Sort");
    sort_two_arrays(int_arr, int_size, double_arr, double_size, "Insertion Sort");
    sort_two_arrays(int_arr, int_size, double_arr, double_size, "Merge Sort");
    sort_two_arrays(int_arr, int_size, double_arr, double_size, "Quick Sort");
    sort_two_arrays(int_arr, int_size, double_arr, double_size, "Heap Sort");
    
    // Test with larger arrays
    const size_t large_size = 10000;
    int* large_int_arr = new int[large_size];
    double* large_double_arr = new double[large_size];
    
    // Fill arrays with random values
    srand(static_cast<unsigned int>(time(nullptr)));
    for (size_t i = 0; i < large_size; ++i) {
        large_int_arr[i] = rand() % 10000;
        large_double_arr[i] = static_cast<double>(rand()) / RAND_MAX * 1000.0;
    }
    
    std::cout << "=== Benchmarking with large arrays (size: " << large_size << ") ===\n\n";
    
    // Benchmark with large arrays
    sort_two_arrays(large_int_arr, large_size, large_double_arr, large_size, "Merge Sort");
    sort_two_arrays(large_int_arr, large_size, large_double_arr, large_size, "Quick Sort");
    sort_two_arrays(large_int_arr, large_size, large_double_arr, large_size, "Heap Sort");
    
    // Clean up
    delete[] large_int_arr;
    delete[] large_double_arr;
    
    return 0;
}
