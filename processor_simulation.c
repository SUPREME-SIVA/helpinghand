#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

#define NUM_PROCESSORS 4

void processor_task(int id) {
    while (1) {
        printf("Processor %d is running...\n", id);
        sleep(2);  // Simulate work
    }
}

int main() {
    pid_t pids[NUM_PROCESSORS];
    int i;

    printf("Starting simulation with %d processors\n", NUM_PROCESSORS);

    // Create multiple processor processes
    for (i = 0; i < NUM_PROCESSORS; i++) {
        pids[i] = fork();
        
        if (pids[i] < 0) {
            perror("Fork failed");
            exit(1);
        }
        
        if (pids[i] == 0) {  // Child process
            processor_task(i + 1);
            exit(0);
        }
    }

    // Parent process
    printf("\nAll processors are running. Press Enter to simulate processor 2 failure...\n");
    getchar();

    // Simulate processor 2 failure by sending SIGTERM
    printf("Simulating failure of processor 2...\n");
    kill(pids[1], SIGTERM);

    // Wait for remaining processors
    printf("\nRemaining processors continue to run. Press Enter to exit...\n");
    getchar();

    // Cleanup: terminate all remaining processors
    for (i = 0; i < NUM_PROCESSORS; i++) {
        kill(pids[i], SIGTERM);
    }

    // Wait for all children to finish
    for (i = 0; i < NUM_PROCESSORS; i++) {
        wait(NULL);
    }

    printf("Simulation ended\n");
    return 0;
}
