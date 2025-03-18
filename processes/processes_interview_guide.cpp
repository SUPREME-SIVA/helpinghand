/**
 * @file processes_interview_guide.cpp
 * @brief Comprehensive guide to process concepts in Unix/Linux for interview preparation
 * 
 * This guide covers:
 * 1. Process fundamentals
 * 2. Process creation and management
 * 3. Inter-Process Communication (IPC)
 * 4. Process synchronization
 * 5. Signals and signal handling
 * 6. Daemon processes
 * 7. Common interview questions
 * 
 * Compilation: g++ -std=c++17 -o processes_interview_guide processes_interview_guide.cpp
 * Usage: ./processes_interview_guide
 */

#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/msg.h>
#include <sys/sem.h>
#include <signal.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <vector>
#include <chrono>
#include <thread>

// For cleaner output
#define SECTION_TITLE(title) std::cout << "\n=== " << title << " ===\n\n"
#define SUBSECTION_TITLE(title) std::cout << "\n--- " << title << " ---\n"
#define CODE_EXAMPLE(title) std::cout << "\n* " << title << ":\n"

// Global variables for signal handling example
volatile sig_atomic_t signal_received = 0;

/**
 * SECTION 1: PROCESS FUNDAMENTALS
 * ===============================
 * - What is a process?
 * - Process states
 * - Process control block
 * - Process vs Thread
 */
void process_fundamentals() {
    SECTION_TITLE("SECTION 1: PROCESS FUNDAMENTALS");
    
    std::cout << "A process is an instance of a program in execution. Each process has:\n"
              << "- A unique Process ID (PID)\n"
              << "- Its own memory space (text, data, heap, stack)\n"
              << "- Resources allocated by the OS (file descriptors, etc.)\n"
              << "- One or more threads of execution\n\n";
    
    SUBSECTION_TITLE("Process States");
    std::cout << "Processes can be in various states during their lifecycle:\n"
              << "1. NEW: Process is being created\n"
              << "2. READY: Process is waiting to be assigned to a processor\n"
              << "3. RUNNING: Instructions are being executed\n"
              << "4. WAITING: Process is waiting for some event to occur\n"
              << "5. TERMINATED: Process has finished execution\n\n";
    
    SUBSECTION_TITLE("Process Control Block (PCB)");
    std::cout << "The PCB is a data structure maintained by the OS for each process. It contains:\n"
              << "- Process ID (PID)\n"
              << "- Process state\n"
              << "- Program counter\n"
              << "- CPU registers\n"
              << "- CPU scheduling information\n"
              << "- Memory management information\n"
              << "- Accounting information\n"
              << "- I/O status information\n\n";
    
    SUBSECTION_TITLE("Process vs Thread");
    std::cout << "Key differences between processes and threads:\n\n"
              << "PROCESSES:\n"
              << "- Independent execution units with separate memory spaces\n"
              << "- More overhead for creation and context switching\n"
              << "- Communication between processes requires IPC mechanisms\n"
              << "- More isolated and secure\n\n"
              << "THREADS:\n"
              << "- Lightweight execution units within a process\n"
              << "- Share the same memory space\n"
              << "- Less overhead for creation and context switching\n"
              << "- Communication via shared memory (global variables)\n"
              << "- Less isolated, bugs can affect other threads\n\n";
    
    CODE_EXAMPLE("Getting current process information");
    std::cout << "Current process ID (PID): " << getpid() << "\n";
    std::cout << "Parent process ID (PPID): " << getppid() << "\n";
}

/**
 * SECTION 2: PROCESS CREATION AND MANAGEMENT
 * =========================================
 * - fork() and exec() family
 * - wait() and waitpid()
 * - exit() and _exit()
 * - Zombie and Orphan processes
 */
void process_creation() {
    SECTION_TITLE("SECTION 2: PROCESS CREATION AND MANAGEMENT");
    
    SUBSECTION_TITLE("Process Creation with fork()");
    std::cout << "fork() creates a new process by duplicating the calling process:\n"
              << "- Child process is an exact copy of the parent\n"
              << "- Child gets a new PID\n"
              << "- fork() returns child's PID to parent, 0 to child\n"
              << "- Memory is copied (copy-on-write)\n"
              << "- File descriptors are shared\n\n";
    
    CODE_EXAMPLE("Basic fork() example");
    pid_t pid = fork();
    
    if (pid < 0) {
        // Fork failed
        std::cerr << "Fork failed: " << strerror(errno) << std::endl;
    } else if (pid == 0) {
        // Child process
        std::cout << "Child process (PID: " << getpid() << ")\n";
        std::cout << "My parent's PID: " << getppid() << "\n";
        
        // Exit child process to continue with the guide
        exit(0);
    } else {
        // Parent process
        std::cout << "Parent process (PID: " << getpid() << ")\n";
        std::cout << "My child's PID: " << pid << "\n";
        
        // Wait for child to finish
        int status;
        waitpid(pid, &status, 0);
        
        if (WIFEXITED(status)) {
            std::cout << "Child exited with status: " << WEXITSTATUS(status) << "\n";
        }
    }
    
    SUBSECTION_TITLE("The exec() Family");
    std::cout << "The exec() family replaces the current process image with a new one:\n"
              << "- execl(), execlp(), execle(): List of arguments\n"
              << "- execv(), execvp(), execvpe(): Vector of arguments\n"
              << "- PID remains the same\n"
              << "- If successful, exec() never returns\n\n";
    
    CODE_EXAMPLE("exec() example (not executed to continue the guide)");
    std::cout << "// Example code for exec():\n"
              << "pid_t pid = fork();\n"
              << "if (pid == 0) {\n"
              << "    // Child process\n"
              << "    execlp(\"ls\", \"ls\", \"-l\", NULL); // Replace with 'ls -l' command\n"
              << "    // If execlp returns, it failed\n"
              << "    perror(\"execlp failed\");\n"
              << "    exit(1);\n"
              << "} else {\n"
              << "    // Parent process\n"
              << "    wait(NULL); // Wait for child to finish\n"
              "}\n\n";
    
    SUBSECTION_TITLE("Process Termination");
    std::cout << "Processes can terminate in several ways:\n"
              << "- Normal termination: exit(), return from main()\n"
              << "- Abnormal termination: abort(), signals\n\n"
              << "exit() vs _exit():\n"
              << "- exit(): Performs cleanup (flushes buffers, calls atexit handlers)\n"
              << "- _exit(): Immediate termination without cleanup\n\n";
    
    SUBSECTION_TITLE("Zombie and Orphan Processes");
    std::cout << "Zombie Process:\n"
              << "- A process that has terminated but its parent hasn't called wait()\n"
              << "- Entry remains in process table\n"
              << "- Resources are released except the process table entry\n\n"
              << "Orphan Process:\n"
              << "- A process whose parent has terminated\n"
              << "- Adopted by the init process (PID 1)\n"
              << "- init automatically calls wait() for orphaned children\n\n";
    
    CODE_EXAMPLE("Creating a zombie process (not executed to continue the guide)");
    std::cout << "// Example code for zombie process:\n"
              << "pid_t pid = fork();\n"
              << "if (pid == 0) {\n"
              << "    // Child process\n"
              << "    exit(0); // Child exits immediately\n"
              << "} else {\n"
              << "    // Parent process\n"
              << "    sleep(10); // Sleep without calling wait()\n"
              << "    // During this time, child is a zombie\n"
              << "    system(\"ps -l\"); // Would show zombie process\n"
              << "    wait(NULL); // Finally reap the zombie\n"
              "}\n\n";
}

/**
 * SECTION 3: INTER-PROCESS COMMUNICATION (IPC)
 * ===========================================
 * - Pipes
 * - Named pipes (FIFOs)
 * - Message queues
 * - Shared memory
 * - Memory-mapped files
 * - Sockets
 */
void inter_process_communication() {
    SECTION_TITLE("SECTION 3: INTER-PROCESS COMMUNICATION (IPC)");
    
    std::cout << "IPC mechanisms allow processes to exchange data and synchronize actions.\n\n";
    
    SUBSECTION_TITLE("Pipes");
    std::cout << "Pipes provide a unidirectional communication channel between related processes:\n"
              << "- Anonymous pipes: For communication between parent and child\n"
              << "- Half-duplex (one-way) communication\n"
              << "- Implemented as a buffer in kernel memory\n\n";
    
    CODE_EXAMPLE("Pipe example");
    int pipefd[2];
    char buffer[100];
    
    if (pipe(pipefd) == -1) {
        std::cerr << "Pipe creation failed: " << strerror(errno) << std::endl;
    } else {
        pid_t pid = fork();
        
        if (pid == 0) {
            // Child process - writer
            close(pipefd[0]); // Close unused read end
            
            const char* message = "Hello from child process!";
            write(pipefd[1], message, strlen(message) + 1);
            
            close(pipefd[1]); // Close write end
            exit(0);
        } else {
            // Parent process - reader
            close(pipefd[1]); // Close unused write end
            
            // Wait for child to write
            wait(NULL);
            
            // Read message from pipe
            int bytes_read = read(pipefd[0], buffer, sizeof(buffer));
            if (bytes_read > 0) {
                std::cout << "Parent received: " << buffer << "\n";
            }
            
            close(pipefd[0]); // Close read end
        }
    }
    
    SUBSECTION_TITLE("Named Pipes (FIFOs)");
    std::cout << "Named pipes allow communication between unrelated processes:\n"
              << "- Exist as a file in the filesystem\n"
              << "- Processes open the FIFO by name\n"
              << "- Persist until explicitly deleted\n\n";
    
    CODE_EXAMPLE("Named pipe example (not executed to continue the guide)");
    std::cout << "// Example code for named pipes:\n"
              << "// Create a named pipe\n"
              << "const char* fifo_path = \"/tmp/my_fifo\";\n"
              << "mkfifo(fifo_path, 0666);\n\n"
              << "// Writer process\n"
              << "int fd = open(fifo_path, O_WRONLY);\n"
              << "write(fd, \"Hello via FIFO\", 15);\n"
              << "close(fd);\n\n"
              << "// Reader process\n"
              << "fd = open(fifo_path, O_RDONLY);\n"
              << "char buffer[100];\n"
              << "read(fd, buffer, sizeof(buffer));\n"
              << "close(fd);\n\n"
              << "// Remove the FIFO\n"
              << "unlink(fifo_path);\n\n";
    
    SUBSECTION_TITLE("Message Queues");
    std::cout << "Message queues allow processes to exchange formatted messages:\n"
              << "- Messages have types and priorities\n"
              << "- Bidirectional communication\n"
              << "- Persistent until explicitly removed\n\n";
    
    CODE_EXAMPLE("Message queue example (not executed to continue the guide)");
    std::cout << "// Example code for message queues:\n"
              << "// Define message structure\n"
              << "struct msg_buffer {\n"
              << "    long msg_type;\n"
              << "    char msg_text[100];\n"
              << "};\n\n"
              << "// Create or get a message queue\n"
              << "key_t key = ftok(\"/tmp\", 'A');\n"
              << "int msgid = msgget(key, 0666 | IPC_CREAT);\n\n"
              << "// Sender process\n"
              << "struct msg_buffer message;\n"
              << "message.msg_type = 1;\n"
              << "strcpy(message.msg_text, \"Hello via message queue\");\n"
              << "msgsnd(msgid, &message, sizeof(message.msg_text), 0);\n\n"
              << "// Receiver process\n"
              << "msgrcv(msgid, &message, sizeof(message.msg_text), 1, 0);\n"
              << "printf(\"Received: %s\\n\", message.msg_text);\n\n"
              << "// Remove the message queue\n"
              << "msgctl(msgid, IPC_RMID, NULL);\n\n";
    
    SUBSECTION_TITLE("Shared Memory");
    std::cout << "Shared memory allows multiple processes to access the same memory region:\n"
              << "- Fastest IPC mechanism (no copying)\n"
              << "- Requires explicit synchronization\n"
              << "- Persistent until explicitly removed\n\n";
    
    CODE_EXAMPLE("Shared memory example (not executed to continue the guide)");
    std::cout << "// Example code for shared memory:\n"
              << "// Create or get shared memory segment\n"
              << "key_t key = ftok(\"/tmp\", 'B');\n"
              << "int shmid = shmget(key, 1024, 0666 | IPC_CREAT);\n\n"
              << "// Writer process\n"
              << "char* shared_memory = (char*)shmat(shmid, NULL, 0);\n"
              << "strcpy(shared_memory, \"Hello via shared memory\");\n"
              << "shmdt(shared_memory);\n\n"
              << "// Reader process\n"
              << "shared_memory = (char*)shmat(shmid, NULL, 0);\n"
              << "printf(\"Data: %s\\n\", shared_memory);\n"
              << "shmdt(shared_memory);\n\n"
              << "// Remove shared memory segment\n"
              << "shmctl(shmid, IPC_RMID, NULL);\n\n";
    
    SUBSECTION_TITLE("Memory-Mapped Files");
    std::cout << "Memory-mapped files map a file directly into memory:\n"
              << "- File I/O becomes memory access\n"
              << "- Can be shared between processes\n"
              << "- Changes to memory are written back to the file\n\n";
    
    CODE_EXAMPLE("Memory-mapped file example (not executed to continue the guide)");
    std::cout << "// Example code for memory-mapped files:\n"
              << "// Open a file\n"
              << "int fd = open(\"/tmp/mmapped.txt\", O_RDWR | O_CREAT, 0666);\n"
              << "ftruncate(fd, 100); // Set file size\n\n"
              << "// Map the file into memory\n"
              << "char* mapped = (char*)mmap(NULL, 100, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);\n"
              << "close(fd); // Can close file descriptor after mapping\n\n"
              << "// Write to the mapped memory\n"
              << "strcpy(mapped, \"Hello via memory-mapped file\");\n\n"
              << "// Unmap when done\n"
              << "munmap(mapped, 100);\n\n";
    
    SUBSECTION_TITLE("Sockets");
    std::cout << "Sockets allow communication between processes on the same or different machines:\n"
              << "- Unix domain sockets: For local IPC\n"
              << "- Internet domain sockets: For network communication\n"
              << "- Full-duplex (two-way) communication\n\n";
    
    CODE_EXAMPLE("Unix domain socket example (not executed to continue the guide)");
    std::cout << "// Example code for Unix domain sockets:\n"
              << "// Server side\n"
              << "int server_fd = socket(AF_UNIX, SOCK_STREAM, 0);\n"
              << "struct sockaddr_un address;\n"
              << "address.sun_family = AF_UNIX;\n"
              << "strcpy(address.sun_path, \"/tmp/socket\");\n"
              << "unlink(\"/tmp/socket\"); // Remove if exists\n"
              << "bind(server_fd, (struct sockaddr*)&address, sizeof(address));\n"
              << "listen(server_fd, 5);\n"
              << "int client_fd = accept(server_fd, NULL, NULL);\n"
              << "char buffer[100];\n"
              << "read(client_fd, buffer, 100);\n"
              << "close(client_fd);\n"
              << "close(server_fd);\n\n"
              << "// Client side\n"
              << "int sock_fd = socket(AF_UNIX, SOCK_STREAM, 0);\n"
              << "struct sockaddr_un addr;\n"
              << "addr.sun_family = AF_UNIX;\n"
              << "strcpy(addr.sun_path, \"/tmp/socket\");\n"
              << "connect(sock_fd, (struct sockaddr*)&addr, sizeof(addr));\n"
              << "write(sock_fd, \"Hello via socket\", 17);\n"
              << "close(sock_fd);\n\n";
}

/**
 * SECTION 4: PROCESS SYNCHRONIZATION
 * =================================
 * - Race conditions
 * - Semaphores
 * - File locks
 */
void process_synchronization() {
    SECTION_TITLE("SECTION 4: PROCESS SYNCHRONIZATION");
    
    std::cout << "Process synchronization is necessary when multiple processes access shared resources.\n\n";
    
    SUBSECTION_TITLE("Race Conditions");
    std::cout << "Race conditions occur when multiple processes access and modify shared data concurrently:\n"
              << "- The outcome depends on the timing of execution\n"
              << "- Can lead to data corruption and unpredictable behavior\n"
              << "- Requires proper synchronization mechanisms\n\n";
    
    SUBSECTION_TITLE("Semaphores");
    std::cout << "Semaphores are synchronization primitives that control access to shared resources:\n"
              << "- Binary semaphores: Can have values 0 or 1 (similar to mutex)\n"
              << "- Counting semaphores: Can have arbitrary non-negative values\n"
              << "- Operations: wait (decrement) and signal (increment)\n\n";
    
    CODE_EXAMPLE("System V semaphores example (not executed to continue the guide)");
    std::cout << "// Example code for System V semaphores:\n"
              << "// Create or get a semaphore set\n"
              << "key_t key = ftok(\"/tmp\", 'C');\n"
              << "int semid = semget(key, 1, 0666 | IPC_CREAT);\n\n"
              << "// Initialize semaphore value to 1 (binary semaphore)\n"
              << "union semun {\n"
              << "    int val;\n"
              << "    struct semid_ds* buf;\n"
              << "    unsigned short* array;\n"
              << "};\n"
              << "union semun arg;\n"
              << "arg.val = 1;\n"
              << "semctl(semid, 0, SETVAL, arg);\n\n"
              << "// Wait operation (P)\n"
              << "struct sembuf sb = {0, -1, 0}; // Decrement by 1\n"
              << "semop(semid, &sb, 1);\n\n"
              << "// Critical section\n"
              << "printf(\"In critical section\\n\");\n\n"
              << "// Signal operation (V)\n"
              << "sb.sem_op = 1; // Increment by 1\n"
              << "semop(semid, &sb, 1);\n\n"
              << "// Remove semaphore set\n"
              << "semctl(semid, 0, IPC_RMID, 0);\n\n";
    
    SUBSECTION_TITLE("File Locks");
    std::cout << "File locks allow processes to coordinate access to shared files:\n"
              << "- Advisory locks: Processes must check the lock (not enforced by OS)\n"
              << "- Mandatory locks: Enforced by the OS\n"
              << "- Read locks (shared) and write locks (exclusive)\n\n";
    
    CODE_EXAMPLE("File locking example (not executed to continue the guide)");
    std::cout << "// Example code for file locking:\n"
              << "// Open a file\n"
              << "int fd = open(\"/tmp/lockfile\", O_RDWR | O_CREAT, 0666);\n\n"
              << "// Set up a write lock\n"
              << "struct flock fl;\n"
              << "fl.l_type = F_WRLCK;    // Write lock\n"
              << "fl.l_whence = SEEK_SET; // From beginning of file\n"
              << "fl.l_start = 0;         // Starting offset\n"
              << "fl.l_len = 0;           // Lock entire file\n\n"
              << "// Acquire the lock (blocking)\n"
              << "fcntl(fd, F_SETLKW, &fl);\n\n"
              << "// Critical section\n"
              << "printf(\"File is locked, writing data...\\n\");\n"
              << "write(fd, \"Protected data\", 15);\n\n"
              << "// Release the lock\n"
              << "fl.l_type = F_UNLCK;\n"
              << "fcntl(fd, F_SETLK, &fl);\n\n"
              << "close(fd);\n\n";
}

/**
 * SECTION 5: SIGNALS AND SIGNAL HANDLING
 * ====================================
 * - Signal concepts
 * - Signal handlers
 * - Common signals
 * - Signal masks
 */
// Signal handler function
void signal_handler(int signum) {
    signal_received = signum;
    std::cout << "Received signal: " << signum << std::endl;
}

void signals_and_handling() {
    SECTION_TITLE("SECTION 5: SIGNALS AND SIGNAL HANDLING");
    
    std::cout << "Signals are software interrupts sent to a process to notify it of an event.\n\n";
    
    SUBSECTION_TITLE("Signal Concepts");
    std::cout << "Key signal concepts:\n"
              << "- Asynchronous notifications sent to processes\n"
              << "- Can be generated by hardware exceptions, terminal input, or software events\n"
              << "- Each signal has a default action (terminate, ignore, stop, continue)\n"
              << "- Processes can handle, ignore, or block signals\n\n";
    
    SUBSECTION_TITLE("Common Signals");
    std::cout << "Common signals in Unix/Linux:\n"
              << "- SIGHUP (1): Hangup detected\n"
              << "- SIGINT (2): Interrupt from keyboard (Ctrl+C)\n"
              << "- SIGQUIT (3): Quit from keyboard (Ctrl+\\)\n"
              << "- SIGKILL (9): Kill signal (cannot be caught or ignored)\n"
              << "- SIGSEGV (11): Segmentation fault\n"
              << "- SIGTERM (15): Termination signal\n"
              << "- SIGCHLD (17): Child process terminated\n"
              << "- SIGUSR1 (10): User-defined signal 1\n"
              << "- SIGUSR2 (12): User-defined signal 2\n\n";
    
    SUBSECTION_TITLE("Signal Handlers");
    std::cout << "Signal handlers are functions that execute when a signal is received:\n"
              << "- Registered using signal() or sigaction()\n"
              << "- Should be kept simple and reentrant\n"
              << "- Should only use async-signal-safe functions\n\n";
    
    CODE_EXAMPLE("Signal handling example");
    // Register signal handler for SIGINT
    signal(SIGINT, signal_handler);
    
    std::cout << "Press Ctrl+C to send SIGINT signal (or wait 3 seconds)...\n";
    
    // Wait for signal or timeout
    for (int i = 0; i < 3; i++) {
        if (signal_received) {
            break;
        }
        sleep(1);
    }
    
    if (!signal_received) {
        std::cout << "No signal received within timeout.\n";
    }
    
    // Reset signal handler to default
    signal(SIGINT, SIG_DFL);
    
    SUBSECTION_TITLE("Signal Masks");
    std::cout << "Signal masks allow processes to block signals temporarily:\n"
              << "- Blocked signals are held pending until unblocked\n"
              << "- Useful during critical sections\n"
              << "- Managed with sigprocmask()\n\n";
    
    CODE_EXAMPLE("Signal mask example (not executed to continue the guide)");
    std::cout << "// Example code for signal masks:\n"
              << "sigset_t mask, old_mask;\n\n"
              << "// Initialize an empty signal set\n"
              << "sigemptyset(&mask);\n\n"
              << "// Add SIGINT to the mask\n"
              << "sigaddset(&mask, SIGINT);\n\n"
              << "// Block SIGINT\n"
              << "sigprocmask(SIG_BLOCK, &mask, &old_mask);\n\n"
              << "// Critical section (SIGINT is blocked)\n"
              << "printf(\"SIGINT is blocked, performing critical operation...\\n\");\n"
              << "sleep(5);\n\n"
              << "// Unblock SIGINT\n"
              << "sigprocmask(SIG_UNBLOCK, &mask, NULL);\n\n";
}

/**
 * SECTION 6: DAEMON PROCESSES
 * =========================
 * - What are daemon processes
 * - Creating a daemon
 * - systemd services
 */
void daemon_processes() {
    SECTION_TITLE("SECTION 6: DAEMON PROCESSES");
    
    std::cout << "Daemon processes are background processes that run independently of any controlling terminal.\n\n";
    
    SUBSECTION_TITLE("Characteristics of Daemons");
    std::cout << "Key characteristics of daemon processes:\n"
              << "- Run in the background\n"
              << "- Not associated with any terminal\n"
              << "- Often started at boot time\n"
              << "- Usually have parent PID 1 (init/systemd)\n"
              << "- Follow specific naming conventions (often end with 'd')\n\n";
    
    SUBSECTION_TITLE("Creating a Daemon");
    std::cout << "Steps to create a daemon process:\n"
              << "1. Fork and exit the parent process\n"
              << "2. Create a new session (setsid())\n"
              << "3. Change working directory (typically to /)\n"
              << "4. Close standard file descriptors\n"
              << "5. Redirect standard file descriptors to /dev/null\n"
              << "6. Set appropriate file creation mask (umask)\n"
              << "7. Implement signal handlers\n"
              << "8. Optionally create a PID file\n\n";
    
    CODE_EXAMPLE("Daemon creation example (not executed to continue the guide)");
    std::cout << "// Example code for creating a daemon:\n"
              << "void daemonize() {\n"
              << "    // Fork and exit parent\n"
              << "    pid_t pid = fork();\n"
              << "    if (pid < 0) exit(1);\n"
              << "    if (pid > 0) exit(0); // Parent exits\n\n"
              << "    // Create new session\n"
              << "    if (setsid() < 0) exit(1);\n\n"
              << "    // Ignore signals\n"
              << "    signal(SIGCHLD, SIG_IGN);\n"
              << "    signal(SIGHUP, SIG_IGN);\n\n"
              << "    // Fork again to ensure not session leader\n"
              << "    pid = fork();\n"
              << "    if (pid < 0) exit(1);\n"
              << "    if (pid > 0) exit(0);\n\n"
              << "    // Set file creation mask\n"
              << "    umask(0);\n\n"
              << "    // Change working directory\n"
              << "    chdir(\"/\");\n\n"
              << "    // Close all open file descriptors\n"
              << "    for (int i = 0; i < sysconf(_SC_OPEN_MAX); i++) {\n"
              << "        close(i);\n"
              << "    }\n\n"
              << "    // Redirect standard file descriptors to /dev/null\n"
              << "    open(\"/dev/null\", O_RDWR); // stdin\n"
              << "    dup(0); // stdout\n"
              << "    dup(0); // stderr\n\n"
              << "    // Create PID file\n"
              << "    int pid_file = open(\"/var/run/mydaemon.pid\", O_RDWR | O_CREAT, 0644);\n"
              << "    if (pid_file >= 0) {\n"
              << "        char pid_str[20];\n"
              << "        sprintf(pid_str, \"%d\\n\", getpid());\n"
              << "        write(pid_file, pid_str, strlen(pid_str));\n"
              << "        close(pid_file);\n"
              << "    }\n"
              << "}\n\n";
    
    SUBSECTION_TITLE("systemd Services");
    std::cout << "Modern Linux systems use systemd for service management:\n"
              << "- Unit files define services (.service files)\n"
              << "- Located in /etc/systemd/system/ or /lib/systemd/system/\n"
              << "- Managed with systemctl commands\n\n";
    
    CODE_EXAMPLE("Example systemd service file");
    std::cout << "# /etc/systemd/system/myservice.service\n"
              << "[Unit]\n"
              << "Description=My Custom Service\n"
              << "After=network.target\n\n"
              << "[Service]\n"
              << "Type=simple\n"
              << "ExecStart=/usr/local/bin/myprogram\n"
              << "Restart=on-failure\n"
              << "User=nobody\n"
              << "Group=nobody\n\n"
              << "[Install]\n"
              << "WantedBy=multi-user.target\n\n";
}

/**
 * SECTION 7: INTERVIEW QUESTIONS AND ANSWERS
 * =======================================
 * Key process concepts for interviews
 */
void print_interview_questions() {
    SECTION_TITLE("SECTION 7: INTERVIEW QUESTIONS AND ANSWERS");
    
    std::cout << "1. What is a process?\n"
              << "   - A process is an instance of a program in execution\n"
              << "   - It has its own memory space, resources, and execution context\n"
              << "   - Each process has a unique Process ID (PID)\n\n";
    
    std::cout << "2. What is the difference between a process and a thread?\n"
              << "   - Processes have separate memory spaces; threads share memory\n"
              << "   - Process creation is more expensive than thread creation\n"
              << "   - Context switching between processes is more expensive\n"
              << "   - Processes communicate via IPC; threads via shared memory\n"
              << "   - Processes are more isolated and secure\n\n";
    
    std::cout << "3. Explain fork() and exec() system calls.\n"
              << "   - fork(): Creates a new process by duplicating the calling process\n"
              << "     - Child is an exact copy with a new PID\n"
              << "     - Returns child's PID to parent, 0 to child\n"
              << "   - exec(): Replaces the current process image with a new one\n"
              << "     - PID remains the same\n"
              << "     - If successful, never returns to the calling process\n\n";
    
    std::cout << "4. What is a zombie process and how do you prevent it?\n"
              << "   - A zombie is a process that has terminated but its parent hasn't called wait()\n"
              << "   - Prevention:\n"
              << "     - Always call wait() or waitpid() in the parent\n"
              << "     - Use signal handlers to catch SIGCHLD and call wait()\n"
              << "     - Double-fork technique for daemons\n\n";
    
    std::cout << "5. What is an orphan process?\n"
              << "   - A process whose parent has terminated before it\n"
              << "   - Adopted by the init process (PID 1)\n"
              << "   - init automatically calls wait() for orphaned children\n\n";
    
    std::cout << "6. What are the different IPC mechanisms in Unix/Linux?\n"
              << "   - Pipes: Unidirectional communication between related processes\n"
              << "   - Named pipes (FIFOs): Communication between unrelated processes\n"
              << "   - Message queues: Structured message exchange\n"
              << "   - Shared memory: Fastest IPC, direct memory access\n"
              << "   - Memory-mapped files: Map file content to memory\n"
              << "   - Sockets: Local or network communication\n"
              << "   - Signals: Asynchronous notifications\n\n";
    
    std::cout << "7. What is a race condition and how do you prevent it?\n"
              << "   - A race condition occurs when multiple processes access shared data concurrently\n"
              << "   - Prevention:\n"
              << "     - Use synchronization primitives (semaphores, mutexes)\n"
              << "     - Use file locks for shared files\n"
              << "     - Implement proper signaling between processes\n\n";
    
    std::cout << "8. What is a signal and how do you handle it?\n"
              << "   - A signal is a software interrupt sent to a process\n"
              << "   - Handling:\n"
              << "     - Register signal handlers with signal() or sigaction()\n"
              << "     - Block signals during critical sections with sigprocmask()\n"
              << "     - Use signal-safe functions in signal handlers\n\n";
    
    std::cout << "9. What is a daemon process and how do you create one?\n"
              << "   - A background process not associated with any terminal\n"
              << "   - Creation steps:\n"
              << "     - Fork and exit parent\n"
              << "     - Create new session with setsid()\n"
              << "     - Change working directory\n"
              << "     - Close/redirect standard file descriptors\n"
              << "     - Set appropriate umask\n\n";
    
    std::cout << "10. What is the difference between exit() and _exit()?\n"
              << "    - exit(): Performs cleanup (flushes buffers, calls atexit handlers)\n"
              << "    - _exit(): Immediate termination without cleanup\n"
              << "    - exit() is from the C library, _exit() is a system call\n\n";
}

/**
 * Main function to demonstrate process concepts
 */
int main() {
    std::cout << "=== UNIX/LINUX PROCESSES INTERVIEW GUIDE ===\n";
    std::cout << "This guide demonstrates key process concepts for interviews\n";
    
    // Demonstrate process fundamentals
    process_fundamentals();
    
    // Demonstrate process creation and management
    process_creation();
    
    // Demonstrate inter-process communication
    inter_process_communication();
    
    // Demonstrate process synchronization
    process_synchronization();
    
    // Demonstrate signals and signal handling
    signals_and_handling();
    
    // Demonstrate daemon processes
    daemon_processes();
    
    // Print interview questions and answers
    print_interview_questions();
    
    std::cout << "\n=== UNIX/LINUX PROCESSES INTERVIEW GUIDE COMPLETED ===\n";
    return 0;
}
