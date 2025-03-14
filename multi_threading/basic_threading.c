/**
 * @file basic_threading.c
 * @brief Demonstrates basic threading concepts in C using POSIX threads (pthreads)
 * 
 * This example shows:
 * 1. Creating threads
 * 2. Passing arguments to threads
 * 3. Joining threads
 * 4. Thread synchronization with mutex
 * 
 * Compilation: gcc -o basic_threading basic_threading.c -lpthread
 * Usage: ./basic_threading
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_THREADS 5
#define ITERATIONS 1000000

// Global counter (shared resource)
long counter = 0;

// Mutex for protecting the counter
pthread_mutex_t counter_mutex = PTHREAD_MUTEX_INITIALIZER;

/**
 * @brief Thread function that increments a counter without synchronization
 * @param arg Thread ID passed as void pointer
 * @return NULL
 */
void* increment_without_mutex(void* arg) {
    int thread_id = *((int*)arg);
    
    printf("Thread %d starting...\n", thread_id);
    
    // Increment counter without synchronization (can cause race conditions)
    for (int i = 0; i < ITERATIONS; i++) {
        counter++;  // UNSAFE: Race condition possible here
    }
    
    printf("Thread %d completed\n", thread_id);
    return NULL;
}

/**
 * @brief Thread function that increments a counter with mutex synchronization
 * @param arg Thread ID passed as void pointer
 * @return NULL
 */
void* increment_with_mutex(void* arg) {
    int thread_id = *((int*)arg);
    
    printf("Thread %d starting...\n", thread_id);
    
    // Increment counter with synchronization
    for (int i = 0; i < ITERATIONS; i++) {
        // Lock mutex before accessing shared resource
        pthread_mutex_lock(&counter_mutex);
        
        // Critical section (protected by mutex)
        counter++;
        
        // Unlock mutex after accessing shared resource
        pthread_mutex_unlock(&counter_mutex);
    }
    
    printf("Thread %d completed\n", thread_id);
    return NULL;
}

/**
 * @brief Demonstrates thread creation and execution without synchronization
 */
void demonstrate_without_mutex() {
    pthread_t threads[NUM_THREADS];
    int thread_ids[NUM_THREADS];
    
    printf("\n=== Demonstration WITHOUT Mutex (Race Condition) ===\n");
    
    // Reset counter
    counter = 0;
    
    // Create threads
    for (int i = 0; i < NUM_THREADS; i++) {
        thread_ids[i] = i;
        if (pthread_create(&threads[i], NULL, increment_without_mutex, &thread_ids[i]) != 0) {
            perror("pthread_create");
            exit(EXIT_FAILURE);
        }
    }
    
    // Wait for all threads to complete
    for (int i = 0; i < NUM_THREADS; i++) {
        if (pthread_join(threads[i], NULL) != 0) {
            perror("pthread_join");
            exit(EXIT_FAILURE);
        }
    }
    
    // Print final counter value
    printf("Final counter value (expected: %ld): %ld\n", 
           (long)NUM_THREADS * ITERATIONS, counter);
    
    // Check if race condition occurred
    if (counter != (long)NUM_THREADS * ITERATIONS) {
        printf("Race condition detected! Counter value is incorrect.\n");
    }
}

/**
 * @brief Demonstrates thread creation and execution with mutex synchronization
 */
void demonstrate_with_mutex() {
    pthread_t threads[NUM_THREADS];
    int thread_ids[NUM_THREADS];
    
    printf("\n=== Demonstration WITH Mutex (Thread-Safe) ===\n");
    
    // Reset counter
    counter = 0;
    
    // Create threads
    for (int i = 0; i < NUM_THREADS; i++) {
        thread_ids[i] = i;
        if (pthread_create(&threads[i], NULL, increment_with_mutex, &thread_ids[i]) != 0) {
            perror("pthread_create");
            exit(EXIT_FAILURE);
        }
    }
    
    // Wait for all threads to complete
    for (int i = 0; i < NUM_THREADS; i++) {
        if (pthread_join(threads[i], NULL) != 0) {
            perror("pthread_join");
            exit(EXIT_FAILURE);
        }
    }
    
    // Print final counter value
    printf("Final counter value (expected: %ld): %ld\n", 
           (long)NUM_THREADS * ITERATIONS, counter);
    
    // Check if counter is correct
    if (counter == (long)NUM_THREADS * ITERATIONS) {
        printf("Success! Counter value is correct.\n");
    }
}

int main() {
    printf("=== Basic Threading Example in C ===\n");
    printf("Number of threads: %d\n", NUM_THREADS);
    printf("Iterations per thread: %d\n", ITERATIONS);
    
    // Demonstrate thread creation without mutex (race condition)
    demonstrate_without_mutex();
    
    // Demonstrate thread creation with mutex (thread-safe)
    demonstrate_with_mutex();
    
    // Destroy mutex
    pthread_mutex_destroy(&counter_mutex);
    
    return EXIT_SUCCESS;
}
