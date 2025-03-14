/**
 * @file basic_processing.c
 * @brief Demonstrates basic multiprocessing concepts in C using fork()
 * 
 * This example shows:
 * 1. Creating child processes with fork()
 * 2. Process isolation (separate memory spaces)
 * 3. Inter-process communication using pipes
 * 4. Process synchronization with wait()
 * 
 * Compilation: gcc -o basic_processing basic_processing.c
 * Usage: ./basic_processing
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <time.h>

#define NUM_PROCESSES 4
#define BUFFER_SIZE 100

/**
 * @brief Demonstrates basic process creation and memory isolation
 */
void demonstrate_process_isolation() {
    pid_t pid;
    int shared_value = 100;
    
    printf("\n=== Demonstration of Process Isolation ===\n");
    
    // Create a child process
    pid = fork();
    
    if (pid < 0) {
        // Error occurred
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        // Child process
        printf("Child process (PID: %d) - Initial shared_value: %d\n", getpid(), shared_value);
        
        // Modify the value in child process
        shared_value = 200;
        printf("Child process (PID: %d) - Modified shared_value: %d\n", getpid(), shared_value);
        
        exit(EXIT_SUCCESS);
    } else {
        // Parent process
        // Wait for child to complete
        wait(NULL);
        
        // Parent's copy of shared_value remains unchanged
        printf("Parent process (PID: %d) - shared_value after child execution: %d\n", 
               getpid(), shared_value);
        
        printf("Note: The child's modification did not affect the parent's copy of shared_value\n");
        printf("This demonstrates that processes have separate memory spaces.\n");
    }
}

/**
 * @brief Demonstrates inter-process communication using pipes
 */
void demonstrate_pipe_communication() {
    int pipe_fd[2];  // File descriptors for the pipe
    pid_t pid;
    char write_msg[BUFFER_SIZE] = "Hello from parent process!";
    char read_msg[BUFFER_SIZE];
    
    printf("\n=== Demonstration of Inter-Process Communication using Pipes ===\n");
    
    // Create a pipe
    if (pipe(pipe_fd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }
    
    // Create a child process
    pid = fork();
    
    if (pid < 0) {
        // Error occurred
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        // Child process
        
        // Close the write end of the pipe
        close(pipe_fd[1]);
        
        // Read from the pipe
        read(pipe_fd[0], read_msg, BUFFER_SIZE);
        printf("Child process (PID: %d) received message: %s\n", getpid(), read_msg);
        
        // Close the read end of the pipe
        close(pipe_fd[0]);
        
        exit(EXIT_SUCCESS);
    } else {
        // Parent process
        
        // Close the read end of the pipe
        close(pipe_fd[0]);
        
        // Write to the pipe
        printf("Parent process (PID: %d) sending message: %s\n", getpid(), write_msg);
        write(pipe_fd[1], write_msg, strlen(write_msg) + 1);
        
        // Close the write end of the pipe
        close(pipe_fd[1]);
        
        // Wait for child to complete
        wait(NULL);
    }
}

/**
 * @brief Demonstrates creating multiple processes and waiting for them
 */
void demonstrate_multiple_processes() {
    pid_t pids[NUM_PROCESSES];
    int i, status;
    
    printf("\n=== Demonstration of Multiple Processes ===\n");
    
    // Fork multiple processes
    for (i = 0; i < NUM_PROCESSES; i++) {
        pids[i] = fork();
        
        if (pids[i] < 0) {
            // Error occurred
            perror("fork");
            exit(EXIT_FAILURE);
        } else if (pids[i] == 0) {
            // Child process
            srand(time(NULL) ^ getpid());  // Seed random number generator
            
            int sleep_time = rand() % 3 + 1;  // Random sleep time between 1-3 seconds
            printf("Child process %d (PID: %d) starting, will sleep for %d seconds\n", 
                   i, getpid(), sleep_time);
            
            // Simulate work
            sleep(sleep_time);
            
            printf("Child process %d (PID: %d) completed\n", i, getpid());
            exit(i);  // Exit with child number as status
        }
    }
    
    // Parent process waits for all children
    printf("Parent process (PID: %d) waiting for all child processes to complete...\n", getpid());
    
    for (i = 0; i < NUM_PROCESSES; i++) {
        pid_t terminated_pid = waitpid(pids[i], &status, 0);
        
        if (WIFEXITED(status)) {
            printf("Parent: Child process %d (PID: %d) terminated with status %d\n", 
                   i, terminated_pid, WEXITSTATUS(status));
        } else {
            printf("Parent: Child process %d (PID: %d) terminated abnormally\n", 
                   i, terminated_pid);
        }
    }
    
    printf("All child processes have completed\n");
}

int main() {
    printf("=== Basic Multiprocessing Example in C ===\n");
    printf("Parent process PID: %d\n", getpid());
    
    // Demonstrate process isolation (separate memory spaces)
    demonstrate_process_isolation();
    
    // Demonstrate inter-process communication using pipes
    demonstrate_pipe_communication();
    
    // Demonstrate creating multiple processes
    demonstrate_multiple_processes();
    
    return EXIT_SUCCESS;
}
