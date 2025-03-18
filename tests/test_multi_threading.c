/**
 * @file test_multi_threading.c
 * @brief Tests for the multi-threading implementations
 * 
 * This file contains tests for the multi-threading examples to ensure
 * they work correctly and demonstrate the intended concepts.
 * 
 * Compilation: gcc -o test_multi_threading test_multi_threading.c -pthread
 * Usage: ./test_multi_threading
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>
#include <assert.h>
#include <errno.h>  // For EBUSY and other error constants

// Shared counter for testing thread synchronization
int shared_counter = 0;

// Mutex for protecting the shared counter
pthread_mutex_t counter_mutex = PTHREAD_MUTEX_INITIALIZER;

/**
 * @brief Thread function that increments the shared counter with mutex protection
 * @param arg Number of increments to perform
 * @return NULL
 */
void* increment_with_mutex(void* arg) {
    int increments = *((int*)arg);
    
    for (int i = 0; i < increments; i++) {
        // Lock mutex before accessing shared resource
        pthread_mutex_lock(&counter_mutex);
        // Critical section (protected by mutex)
        shared_counter++;
        // Unlock mutex after accessing shared resource
        pthread_mutex_unlock(&counter_mutex);
    }
    
    return NULL;
}

/**
 * @brief Thread function that increments the shared counter without mutex protection
 * @param arg Number of increments to perform
 * @return NULL
 */
void* increment_without_mutex(void* arg) {
    int increments = *((int*)arg);
    
    for (int i = 0; i < increments; i++) {
        // No mutex protection, can lead to race conditions
        shared_counter++;
    }
    
    return NULL;
}

/**
 * @brief Tests basic thread creation and joining
 */
void test_thread_creation() {
    printf("Testing thread creation and joining...\n");
    
    pthread_t thread;
    int increments = 1000;
    
    // Reset counter
    shared_counter = 0;
    
    // Create thread
    int result = pthread_create(&thread, NULL, increment_with_mutex, &increments);
    assert(result == 0 && "Thread creation failed");
    
    // Join thread
    result = pthread_join(thread, NULL);
    assert(result == 0 && "Thread join failed");
    
    // Verify counter value
    assert(shared_counter == increments && "Counter value incorrect after thread execution");
    
    printf("Thread creation and joining test passed!\n");
}

/**
 * @brief Tests mutex synchronization with multiple threads
 */
void test_mutex_synchronization() {
    printf("Testing mutex synchronization with multiple threads...\n");
    
    const int NUM_THREADS = 5;
    pthread_t threads[NUM_THREADS];
    int increments = 1000;
    
    // Reset counter
    shared_counter = 0;
    
    // Create threads
    for (int i = 0; i < NUM_THREADS; i++) {
        int result = pthread_create(&threads[i], NULL, increment_with_mutex, &increments);
        assert(result == 0 && "Thread creation failed");
    }
    
    // Join threads
    for (int i = 0; i < NUM_THREADS; i++) {
        int result = pthread_join(threads[i], NULL);
        assert(result == 0 && "Thread join failed");
    }
    
    // Verify counter value (should be NUM_THREADS * increments)
    assert(shared_counter == NUM_THREADS * increments && 
           "Counter value incorrect after synchronized threads");
    
    printf("Mutex synchronization test passed!\n");
}

/**
 * @brief Tests race conditions without mutex protection
 * Note: This test may not always fail due to the non-deterministic nature of race conditions
 */
void test_race_conditions() {
    printf("Testing race conditions without mutex protection...\n");
    printf("Note: This test intentionally demonstrates race conditions and may not always fail\n");
    
    const int NUM_THREADS = 5;
    pthread_t threads[NUM_THREADS];
    int increments = 10000;
    
    // Reset counter
    shared_counter = 0;
    
    // Create threads without mutex protection
    for (int i = 0; i < NUM_THREADS; i++) {
        int result = pthread_create(&threads[i], NULL, increment_without_mutex, &increments);
        assert(result == 0 && "Thread creation failed");
    }
    
    // Join threads
    for (int i = 0; i < NUM_THREADS; i++) {
        int result = pthread_join(threads[i], NULL);
        assert(result == 0 && "Thread join failed");
    }
    
    // Check if counter value is less than expected (due to race conditions)
    // This is not a strict assertion since race conditions are non-deterministic
    if (shared_counter < NUM_THREADS * increments) {
        printf("Race condition detected: counter = %d, expected = %d\n", 
               shared_counter, NUM_THREADS * increments);
        printf("This demonstrates the need for proper synchronization!\n");
    } else {
        printf("No race condition detected in this run (counter = %d)\n", shared_counter);
        printf("Race conditions are non-deterministic and may not occur every time\n");
    }
}

/**
 * @brief Tests deadlock detection using trylock
 */
void test_deadlock_detection() {
    printf("Testing deadlock detection...\n");
    
    pthread_mutex_t mutex_a = PTHREAD_MUTEX_INITIALIZER;
    pthread_mutex_t mutex_b = PTHREAD_MUTEX_INITIALIZER;
    
    // Lock mutex A
    pthread_mutex_lock(&mutex_a);
    printf("Locked mutex A\n");
    
    // Simulate another thread holding mutex B and waiting for mutex A
    // by just locking mutex B here
    pthread_mutex_lock(&mutex_b);
    
    // Try to lock mutex B (would deadlock if we didn't use trylock)
    int result = pthread_mutex_trylock(&mutex_b);
    
    // If mutex B is already locked (as it should be), trylock will return EBUSY
    assert(result == EBUSY && "Expected mutex B to be locked");
    
    printf("Detected that mutex B is already locked - potential deadlock detected\n");
    
    // Unlock mutexes
    pthread_mutex_unlock(&mutex_a);
    pthread_mutex_unlock(&mutex_b);
    
    printf("Deadlock detection test passed!\n");
}

int main() {
    printf("=== Multi-Threading Tests ===\n\n");
    
    // Test basic thread creation and joining
    test_thread_creation();
    printf("\n");
    
    // Test mutex synchronization with multiple threads
    test_mutex_synchronization();
    printf("\n");
    
    // Test race conditions without mutex protection
    test_race_conditions();
    printf("\n");
    
    // Test deadlock detection
    test_deadlock_detection();
    printf("\n");
    
    printf("All tests completed successfully!\n");
    
    return EXIT_SUCCESS;
}
