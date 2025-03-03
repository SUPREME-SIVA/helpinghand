CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g
LDFLAGS = -lm

# List of all source files
SRCS = addition.c test_addition.c Palindrome.c StringRevers.c array_rotation.c bit_flip.c \
       memory_example.c processor_simulation.c simple_array_rotation.c swap_even_numbers.c test.c

# Generate executable names from source files (without .c extension)
EXECS = $(SRCS:.c=)

# Default target to build all executables
all: $(EXECS)

# Generic rule for building executables
%: %.c
	@echo "Building $@..."
	@$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)
	@echo "Done building $@"

# Special targets with dependencies
test_addition: test_addition.c
	@echo "Building $@..."
	@$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)
	@echo "Done building $@"

# Run tests
test: test_addition
	@echo "Running tests..."
	@./test_addition
	@echo "All tests completed"

# Run a specific program
run-%: %
	@echo "Running $*..."
	@./$*

# Clean all executables
clean:
	@echo "Cleaning up executables..."
	@rm -f $(EXECS)
	@echo "Clean complete"

# Create a directory for object files if needed
.PHONY: objdir
objdir:
	@mkdir -p obj

# List all available targets
help:
	@echo "====== HelpingHand C Project Makefile ======"
	@echo "Available targets:"
	@echo "  all           - Build all executables"
	@echo "  clean         - Remove all executables"
	@echo "  test          - Run test_addition"
	@echo "  run-PROGRAM   - Build and run a specific program (e.g., make run-addition)"
	@echo "  help          - Display this help message"
	@echo ""
	@echo "Individual executables:"
	@for exec in $(EXECS); do \
		echo "  $$exec"; \
	done
	@echo "=========================================="
