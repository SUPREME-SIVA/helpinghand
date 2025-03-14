/**
 * @file basic_processing.cpp
 * @brief Demonstrates basic multiprocessing concepts in C++ using fork() and C++ wrappers
 * 
 * This example shows:
 * 1. Creating child processes with fork()
 * 2. Process isolation (separate memory spaces)
 * 3. Inter-process communication using pipes
 * 4. Process synchronization with wait()
 * 5. C++ abstractions for process management
 * 
 * Compilation: g++ -std=c++17 -o basic_processing basic_processing.cpp
 * Usage: ./basic_processing
 */

#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <thread>
#include <random>
#include <system_error>
#include <unistd.h>
#include <sys/wait.h>
#include <cstring>

// Constants
constexpr int NUM_PROCESSES = 4;
constexpr int BUFFER_SIZE = 100;

/**
 * @brief Wrapper for system error handling
 * @param result The result of a system call
 * @param message Error message to display
 */
void check_system_call(int result, const std::string& message) {
    if (result == -1) {
        throw std::system_error(errno, std::generic_category(), message);
    }
}

/**
 * @brief Demonstrates basic process creation and memory isolation
 */
void demonstrate_process_isolation() {
    pid_t pid;
    int shared_value = 100;
    
    std::cout << "\n=== Demonstration of Process Isolation ===\n";
    
    // Create a child process
    pid = fork();
    
    if (pid < 0) {
        // Error occurred
        throw std::system_error(errno, std::generic_category(), "fork failed");
    } else if (pid == 0) {
        // Child process
        std::cout << "Child process (PID: " << getpid() << ") - Initial shared_value: " 
                  << shared_value << std::endl;
        
        // Modify the value in child process
        shared_value = 200;
        std::cout << "Child process (PID: " << getpid() << ") - Modified shared_value: " 
                  << shared_value << std::endl;
        
        exit(EXIT_SUCCESS);
    } else {
        // Parent process
        // Wait for child to complete
        int status;
        waitpid(pid, &status, 0);
        
        // Parent's copy of shared_value remains unchanged
        std::cout << "Parent process (PID: " << getpid() 
                  << ") - shared_value after child execution: " << shared_value << std::endl;
        
        std::cout << "Note: The child's modification did not affect the parent's copy of shared_value\n";
        std::cout << "This demonstrates that processes have separate memory spaces.\n";
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
    
    std::cout << "\n=== Demonstration of Inter-Process Communication using Pipes ===\n";
    
    // Create a pipe
    check_system_call(pipe(pipe_fd), "pipe creation failed");
    
    // Create a child process
    pid = fork();
    
    if (pid < 0) {
        // Error occurred
        throw std::system_error(errno, std::generic_category(), "fork failed");
    } else if (pid == 0) {
        // Child process
        
        // Close the write end of the pipe
        close(pipe_fd[1]);
        
        // Read from the pipe
        read(pipe_fd[0], read_msg, BUFFER_SIZE);
        std::cout << "Child process (PID: " << getpid() << ") received message: " 
                  << read_msg << std::endl;
        
        // Close the read end of the pipe
        close(pipe_fd[0]);
        
        exit(EXIT_SUCCESS);
    } else {
        // Parent process
        
        // Close the read end of the pipe
        close(pipe_fd[0]);
        
        // Write to the pipe
        std::cout << "Parent process (PID: " << getpid() << ") sending message: " 
                  << write_msg << std::endl;
        write(pipe_fd[1], write_msg, strlen(write_msg) + 1);
        
        // Close the write end of the pipe
        close(pipe_fd[1]);
        
        // Wait for child to complete
        int status;
        waitpid(pid, &status, 0);
    }
}

/**
 * @brief Demonstrates creating multiple processes and waiting for them
 */
void demonstrate_multiple_processes() {
    std::vector<pid_t> pids(NUM_PROCESSES);
    
    std::cout << "\n=== Demonstration of Multiple Processes ===\n";
    
    // Fork multiple processes
    for (int i = 0; i < NUM_PROCESSES; i++) {
        pids[i] = fork();
        
        if (pids[i] < 0) {
            // Error occurred
            throw std::system_error(errno, std::generic_category(), "fork failed");
        } else if (pids[i] == 0) {
            // Child process
            
            // Seed random number generator with process ID
            std::random_device rd;
            std::mt19937 gen(rd());
            gen.seed(getpid());
            std::uniform_int_distribution<> dist(1, 3);
            
            int sleep_time = dist(gen);  // Random sleep time between 1-3 seconds
            std::cout << "Child process " << i << " (PID: " << getpid() 
                      << ") starting, will sleep for " << sleep_time << " seconds\n";
            
            // Simulate work
            std::this_thread::sleep_for(std::chrono::seconds(sleep_time));
            
            std::cout << "Child process " << i << " (PID: " << getpid() << ") completed\n";
            exit(i);  // Exit with child number as status
        }
    }
    
    // Parent process waits for all children
    std::cout << "Parent process (PID: " << getpid() 
              << ") waiting for all child processes to complete...\n";
    
    for (int i = 0; i < NUM_PROCESSES; i++) {
        int status;
        pid_t terminated_pid = waitpid(pids[i], &status, 0);
        
        if (WIFEXITED(status)) {
            std::cout << "Parent: Child process " << i << " (PID: " << terminated_pid 
                      << ") terminated with status " << WEXITSTATUS(status) << std::endl;
        } else {
            std::cout << "Parent: Child process " << i << " (PID: " << terminated_pid 
                      << ") terminated abnormally\n";
        }
    }
    
    std::cout << "All child processes have completed\n";
}

/**
 * @brief Demonstrates a simple process manager class
 */
class ProcessManager {
private:
    std::vector<pid_t> child_pids;
    
public:
    ProcessManager() = default;
    
    /**
     * @brief Spawn a new child process
     * @param func Function to run in the child process
     * @return PID of the child process (0 in the child, >0 in the parent)
     */
    template<typename Func>
    pid_t spawn_process(Func func) {
        pid_t pid = fork();
        
        if (pid < 0) {
            // Error occurred
            throw std::system_error(errno, std::generic_category(), "fork failed");
        } else if (pid == 0) {
            // Child process
            try {
                func();  // Execute the function in the child process
                exit(EXIT_SUCCESS);
            } catch (const std::exception& e) {
                std::cerr << "Exception in child process: " << e.what() << std::endl;
                exit(EXIT_FAILURE);
            } catch (...) {
                std::cerr << "Unknown exception in child process" << std::endl;
                exit(EXIT_FAILURE);
            }
        } else {
            // Parent process
            child_pids.push_back(pid);
        }
        
        return pid;
    }
    
    /**
     * @brief Wait for all child processes to complete
     */
    void wait_for_all() {
        for (pid_t pid : child_pids) {
            int status;
            waitpid(pid, &status, 0);
            
            if (WIFEXITED(status)) {
                std::cout << "Process " << pid << " exited with status " 
                          << WEXITSTATUS(status) << std::endl;
            } else if (WIFSIGNALED(status)) {
                std::cout << "Process " << pid << " killed by signal " 
                          << WTERMSIG(status) << std::endl;
            }
        }
        
        // Clear the list of child PIDs
        child_pids.clear();
    }
    
    /**
     * @brief Terminate all child processes
     * @param signal Signal to send to the child processes
     */
    void terminate_all(int signal = SIGTERM) {
        for (pid_t pid : child_pids) {
            kill(pid, signal);
        }
    }
    
    /**
     * @brief Destructor that ensures all child processes are terminated
     */
    ~ProcessManager() {
        terminate_all();
    }
};

/**
 * @brief Demonstrates using the ProcessManager class
 */
void demonstrate_process_manager() {
    std::cout << "\n=== Demonstration of Process Manager Class ===\n";
    
    // Create a process manager
    ProcessManager manager;
    
    // Spawn multiple processes with different work
    for (int i = 0; i < NUM_PROCESSES; i++) {
        // Use a lambda to capture the process number
        manager.spawn_process([i]() {
            std::cout << "Managed process " << i << " (PID: " << getpid() << ") starting\n";
            
            // Simulate different work in each process
            std::random_device rd;
            std::mt19937 gen(rd());
            gen.seed(getpid());
            std::uniform_int_distribution<> dist(1, 3);
            
            int work_time = dist(gen);
            std::cout << "Managed process " << i << " working for " << work_time << " seconds\n";
            
            // Simulate work
            std::this_thread::sleep_for(std::chrono::seconds(work_time));
            
            std::cout << "Managed process " << i << " completed work\n";
            
            // Return the process number as the exit status
            return i;
        });
    }
    
    std::cout << "All child processes spawned, waiting for completion...\n";
    
    // Wait for all processes to complete
    manager.wait_for_all();
    
    std::cout << "All managed processes have completed\n";
}

int main() {
    try {
        std::cout << "=== Basic Multiprocessing Example in C++ ===\n";
        std::cout << "Parent process PID: " << getpid() << std::endl;
        
        // Demonstrate process isolation (separate memory spaces)
        demonstrate_process_isolation();
        
        // Demonstrate inter-process communication using pipes
        demonstrate_pipe_communication();
        
        // Demonstrate creating multiple processes
        demonstrate_multiple_processes();
        
        // Demonstrate process manager class
        demonstrate_process_manager();
        
        return EXIT_SUCCESS;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
}
