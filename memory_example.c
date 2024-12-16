#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/resource.h>

#define MB (1024 * 1024)  // 1 Megabyte

// Function to get current memory usage
void print_memory_usage() {
    struct rusage r_usage;
    getrusage(RUSAGE_SELF, &r_usage);
    printf("Memory used: %ld KB\n", r_usage.ru_maxrss);
}

int main() {
    printf("Memory Example: Physical vs Virtual Memory\n");
    printf("------------------------------------------\n\n");

    // Initial memory usage
    printf("Initial ");
    print_memory_usage();

    // Allocate memory in chunks to demonstrate virtual memory
    int num_chunks = 5;
    void *chunks[5];
    int chunk_size = 100 * MB;  // 100MB chunks

    printf("\nAllocating %d chunks of %d MB each...\n\n", num_chunks, chunk_size/MB);

    for (int i = 0; i < num_chunks; i++) {
        chunks[i] = malloc(chunk_size);
        
        if (chunks[i] == NULL) {
            printf("Failed to allocate chunk %d\n", i);
            break;
        }

        // Write some data to force physical memory allocation
        memset(chunks[i], 1, chunk_size);
        
        printf("Allocated chunk %d (%d MB)\n", i + 1, chunk_size/MB);
        print_memory_usage();
        
        sleep(1); // Sleep to make the output readable
    }

    printf("\nMemory allocation complete!\n");
    printf("This program demonstrated:\n");
    printf("1. Physical Memory (RAM): Actually used memory shown above\n");
    printf("2. Virtual Memory: Total allocated = %d MB\n", (chunk_size/MB) * num_chunks);
    printf("\nNote: If the physical RAM was insufficient,\n");
    printf("the OS used virtual memory (swap space) to handle the allocation.\n");

    // Free the allocated memory
    for (int i = 0; i < num_chunks; i++) {
        if (chunks[i] != NULL) {
            free(chunks[i]);
        }
    }

    return 0;
}
