/**
 * @file deadlock_example.c
 * @brief Demonstrates deadlock scenarios and prevention techniques in C
 * 
 * This example shows:
 * 1. How deadlocks occur when multiple threads acquire locks in different orders
 * 2. How to prevent deadlocks by acquiring locks in a consistent order
 * 3. How to use timeouts to detect and recover from potential deadlocks
 * 
 * Compilation: gcc -o deadlock_example deadlock_example.c -lpthread
 * Usage: ./deadlock_example
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>
#include <time.h>
#include <errno.h>

// Mutex locks for resources
pthread_mutex_t resource_a_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t resource_b_mutex = PTHREAD_MUTEX_INITIALIZER;

// Flag to enable/disable deadlock prevention
bool prevent_deadlock = false;

/**
 * @brief Thread function that demonstrates a deadlock scenario
 * 
 * Thread 1 acquires resource A, then tries to acquire resource B
 * 
 * @param arg Thread arguments (unused)
 * @return NULL
 */
void* thread_1_function(void* arg) {
    (void)arg; // Suppress unused parameter warning
    
    printf("Thread 1: Starting\n");
    printf("Thread 1: Trying to acquire Resource A\n");
    
    // Acquire resource A
    pthread_mutex_lock(&resource_a_mutex);
    printf("Thread 1: Acquired Resource A\n");
    
    // Simulate some work
    printf("Thread 1: Working with Resource A\n");
    sleep(1);
    
    printf("Thread 1: Trying to acquire Resource B\n");
    
    // If deadlock prevention is enabled, release resource A before acquiring B
    if (prevent_deadlock) {
        printf("Thread 1: Releasing Resource A to prevent deadlock\n");
        pthread_mutex_unlock(&resource_a_mutex);
        
        printf("Thread 1: Trying to acquire both resources in order (A then B)\n");
        pthread_mutex_lock(&resource_a_mutex);
        printf("Thread 1: Acquired Resource A\n");
        pthread_mutex_lock(&resource_b_mutex);
        printf("Thread 1: Acquired Resource B\n");
    } else {
        // This can lead to deadlock if Thread 2 has acquired Resource B
        // and is waiting for Resource A
        pthread_mutex_lock(&resource_b_mutex);
        printf("Thread 1: Acquired Resource B\n");
    }
    
    // Work with both resources
    printf("Thread 1: Working with both resources\n");
    sleep(1);
    
    // Release resources
    pthread_mutex_unlock(&resource_b_mutex);
    printf("Thread 1: Released Resource B\n");
    
    pthread_mutex_unlock(&resource_a_mutex);
    printf("Thread 1: Released Resource A\n");
    
    printf("Thread 1: Finished\n");
    return NULL;
}

/**
 * @brief Thread function that demonstrates a deadlock scenario
 * 
 * Thread 2 acquires resource B, then tries to acquire resource A
 * 
 * @param arg Thread arguments (unused)
 * @return NULL
 */
void* thread_2_function(void* arg) {
    (void)arg; // Suppress unused parameter warning
    
    printf("Thread 2: Starting\n");
    printf("Thread 2: Trying to acquire Resource B\n");
    
    // Acquire resource B
    pthread_mutex_lock(&resource_b_mutex);
    printf("Thread 2: Acquired Resource B\n");
    
    // Simulate some work
    printf("Thread 2: Working with Resource B\n");
    sleep(1);
    
    printf("Thread 2: Trying to acquire Resource A\n");
    
    // If deadlock prevention is enabled, release resource B before acquiring A
    if (prevent_deadlock) {
        printf("Thread 2: Releasing Resource B to prevent deadlock\n");
        pthread_mutex_unlock(&resource_b_mutex);
        
        printf("Thread 2: Trying to acquire both resources in order (A then B)\n");
        pthread_mutex_lock(&resource_a_mutex);
        printf("Thread 2: Acquired Resource A\n");
        pthread_mutex_lock(&resource_b_mutex);
        printf("Thread 2: Acquired Resource B\n");
    } else {
        // This can lead to deadlock if Thread 1 has acquired Resource A
        // and is waiting for Resource B
        pthread_mutex_lock(&resource_a_mutex);
        printf("Thread 2: Acquired Resource A\n");
    }
    
    // Work with both resources
    printf("Thread 2: Working with both resources\n");
    sleep(1);
    
    // Release resources
    pthread_mutex_unlock(&resource_a_mutex);
    printf("Thread 2: Released Resource A\n");
    
    pthread_mutex_unlock(&resource_b_mutex);
    printf("Thread 2: Released Resource B\n");
    
    printf("Thread 2: Finished\n");
    return NULL;
}

/**
 * @brief Demonstrates a deadlock scenario
 */
void demonstrate_deadlock() {
    pthread_t thread1, thread2;
    
    printf("\n=== Demonstrating Deadlock Scenario ===\n");
    printf("In this scenario, Thread 1 acquires Resource A and then tries to acquire Resource B.\n");
    printf("Meanwhile, Thread 2 acquires Resource B and then tries to acquire Resource A.\n");
    printf("This creates a circular wait condition, leading to deadlock.\n\n");
    
    // Create threads
    if (pthread_create(&thread1, NULL, thread_1_function, NULL) != 0) {
        perror("pthread_create");
        exit(EXIT_FAILURE);
    }
    
    // Small delay to ensure Thread 1 starts first
    usleep(100000);  // 100ms
    
    if (pthread_create(&thread2, NULL, thread_2_function, NULL) != 0) {
        perror("pthread_create");
        exit(EXIT_FAILURE);
    }
    
    // Wait for threads to complete (they may never complete due to deadlock)
    printf("Main: Waiting for threads to complete (they may deadlock)...\n");
    printf("If the program hangs, press Ctrl+C to terminate.\n");
    
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    
    printf("Main: Both threads completed successfully (no deadlock occurred).\n");
}

/**
 * @brief Demonstrates deadlock prevention by acquiring locks in a consistent order
 */
void demonstrate_deadlock_prevention() {
    pthread_t thread1, thread2;
    
    printf("\n=== Demonstrating Deadlock Prevention ===\n");
    printf("In this scenario, both threads acquire resources in the same order (A then B).\n");
    printf("This prevents the circular wait condition and avoids deadlock.\n\n");
    
    // Enable deadlock prevention
    prevent_deadlock = true;
    
    // Create threads
    if (pthread_create(&thread1, NULL, thread_1_function, NULL) != 0) {
        perror("pthread_create");
        exit(EXIT_FAILURE);
    }
    
    // Small delay to ensure Thread 1 starts first
    usleep(100000);  // 100ms
    
    if (pthread_create(&thread2, NULL, thread_2_function, NULL) != 0) {
        perror("pthread_create");
        exit(EXIT_FAILURE);
    }
    
    // Wait for threads to complete
    printf("Main: Waiting for threads to complete...\n");
    
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    
    printf("Main: Both threads completed successfully.\n");
    
    // Disable deadlock prevention for future demonstrations
    prevent_deadlock = false;
}

/**
 * @brief Thread function that demonstrates deadlock detection using trylock
 * 
 * @param arg Thread ID (1 or 2)
 * @return NULL
 */
void* thread_with_trylock(void* arg) {
    int thread_id = *((int*)arg);
    int result;
    int retry_count = 0;
    const int max_retries = 5;
    
    printf("Thread %d: Starting\n", thread_id);
    
    if (thread_id == 1) {
        printf("Thread 1: Trying to acquire Resource A\n");
        
        // Try to acquire resource A
        result = pthread_mutex_lock(&resource_a_mutex);
        
        if (result == 0) {
            printf("Thread 1: Acquired Resource A\n");
            
            // Simulate some work
            printf("Thread 1: Working with Resource A\n");
            sleep(1);
            
            printf("Thread 1: Trying to acquire Resource B\n");
            
            // Try to acquire resource B with retries
            while (retry_count < max_retries) {
                result = pthread_mutex_trylock(&resource_b_mutex);
                
                if (result == 0) {
                    printf("Thread 1: Acquired Resource B\n");
                    
                    // Work with both resources
                    printf("Thread 1: Working with both resources\n");
                    sleep(1);
                    
                    // Release resources
                    pthread_mutex_unlock(&resource_b_mutex);
                    printf("Thread 1: Released Resource B\n");
                    break;
                } else if (result == EBUSY) {
                    retry_count++;
                    printf("Thread 1: Resource B is busy (attempt %d/%d) - potential deadlock detected\n", 
                           retry_count, max_retries);
                    
                    if (retry_count >= max_retries) {
                        printf("Thread 1: Taking recovery action (releasing Resource A)\n");
                    } else {
                        // Wait before retrying
                        usleep(200000 * retry_count);  // Increasing backoff
                    }
                } else {
                    perror("pthread_mutex_trylock");
                    break;
                }
            }
            
            pthread_mutex_unlock(&resource_a_mutex);
            printf("Thread 1: Released Resource A\n");
        } else {
            perror("pthread_mutex_lock");
        }
    } else {
        printf("Thread 2: Trying to acquire Resource B\n");
        
        // Try to acquire resource B
        result = pthread_mutex_lock(&resource_b_mutex);
        
        if (result == 0) {
            printf("Thread 2: Acquired Resource B\n");
            
            // Simulate some work
            printf("Thread 2: Working with Resource B\n");
            sleep(1);
            
            printf("Thread 2: Trying to acquire Resource A\n");
            
            // Try to acquire resource A with retries
            while (retry_count < max_retries) {
                result = pthread_mutex_trylock(&resource_a_mutex);
                
                if (result == 0) {
                    printf("Thread 2: Acquired Resource A\n");
                    
                    // Work with both resources
                    printf("Thread 2: Working with both resources\n");
                    sleep(1);
                    
                    // Release resources
                    pthread_mutex_unlock(&resource_a_mutex);
                    printf("Thread 2: Released Resource A\n");
                    break;
                } else if (result == EBUSY) {
                    retry_count++;
                    printf("Thread 2: Resource A is busy (attempt %d/%d) - potential deadlock detected\n", 
                           retry_count, max_retries);
                    
                    if (retry_count >= max_retries) {
                        printf("Thread 2: Taking recovery action (releasing Resource B)\n");
                    } else {
                        // Wait before retrying
                        usleep(300000 * retry_count);  // Increasing backoff
                    }
                } else {
                    perror("pthread_mutex_trylock");
                    break;
                }
            }
            
            pthread_mutex_unlock(&resource_b_mutex);
            printf("Thread 2: Released Resource B\n");
        } else {
            perror("pthread_mutex_lock");
        }
    }
    
    printf("Thread %d: Finished\n", thread_id);
    return NULL;
}

/**
 * @brief Demonstrates deadlock detection using trylock
 */
void demonstrate_deadlock_detection() {
    pthread_t thread1, thread2;
    int thread1_id = 1;
    int thread2_id = 2;
    
    printf("\n=== Demonstrating Deadlock Detection using trylock ===\n");
    printf("In this scenario, threads use trylock to detect potential deadlocks.\n");
    printf("If a thread cannot acquire a lock after several attempts, it assumes a deadlock\n");
    printf("might be occurring and takes recovery action.\n\n");
    
    // Create threads
    if (pthread_create(&thread1, NULL, thread_with_trylock, &thread1_id) != 0) {
        perror("pthread_create");
        exit(EXIT_FAILURE);
    }
    
    // Small delay to ensure Thread 1 starts first
    usleep(100000);  // 100ms
    
    if (pthread_create(&thread2, NULL, thread_with_trylock, &thread2_id) != 0) {
        perror("pthread_create");
        exit(EXIT_FAILURE);
    }
    
    // Wait for threads to complete
    printf("Main: Waiting for threads to complete...\n");
    
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    
    printf("Main: Both threads completed successfully.\n");
}

/**
 * @brief Explains the four conditions necessary for a deadlock to occur
 */
void explain_deadlock_conditions() {
    printf("\n=== The Four Conditions for Deadlock ===\n");
    printf("A deadlock occurs when all four of the following conditions are met:\n\n");
    
    printf("1. Mutual Exclusion: At least one resource must be held in a non-sharable mode.\n");
    printf("   In our examples, mutexes provide mutual exclusion for resources A and B.\n\n");
    
    printf("2. Hold and Wait: A process must be holding at least one resource and waiting\n");
    printf("   to acquire additional resources held by other processes.\n");
    printf("   In our deadlock scenario, Thread 1 holds Resource A and waits for Resource B,\n");
    printf("   while Thread 2 holds Resource B and waits for Resource A.\n\n");
    
    printf("3. No Preemption: Resources cannot be forcibly taken away from a process.\n");
    printf("   They must be explicitly released by the process holding them.\n");
    printf("   In our examples, mutexes cannot be forcibly released by another thread.\n\n");
    
    printf("4. Circular Wait: A circular chain of processes exists, where each process\n");
    printf("   holds resources that are requested by the next process in the chain.\n");
    printf("   In our deadlock scenario, Thread 1 -> Resource A -> Thread 2 -> Resource B -> Thread 1\n");
    printf("   forms a circular wait condition.\n\n");
    
    printf("To prevent deadlocks, at least one of these conditions must be eliminated.\n");
    printf("Our prevention strategies focus on breaking the circular wait condition by\n");
    printf("ensuring resources are acquired in a consistent order.\n");
}

int main() {
    printf("=== Deadlock Example in C ===\n");
    
    // Explain deadlock conditions
    explain_deadlock_conditions();
    
    // Demonstrate a deadlock scenario
    demonstrate_deadlock();
    
    // Demonstrate deadlock prevention
    demonstrate_deadlock_prevention();
    
    // Demonstrate deadlock detection
    demonstrate_deadlock_detection();
    
    return EXIT_SUCCESS;
}
