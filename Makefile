CC = gcc
CXX = g++
CFLAGS = -Wall -Wextra -std=c99 -g
CXXFLAGS = -Wall -Wextra -std=c++17 -g
LDFLAGS = -lm

# Directory for executables
BIN_DIR = bin

# List of all C source files
C_SRCS = addition.c test_addition.c Palindrome.c StringRevers.c array_rotation.c bit_flip.c \
       memory_example.c processor_simulation.c simple_array_rotation.c swap_even_numbers.c test.c

# List of all C++ source files
CXX_SRCS = advanced_solid_example.cpp array_rotation.cpp casting_examples.cpp constexpr_optimization.cpp \
       copy_constructor_example.cpp custom_deleter.cpp custom_memory_manager.cpp data_container_example.cpp \
       database_singleton.cpp document_factory.cpp file_processor.cpp if_init_example.cpp logger_singleton.cpp \
       non_virtual_access.cpp optimized_derived_access.cpp packet_handler_example.cpp parallel_sum_improved.cpp \
       payment_factory.cpp shared_ptr_demo.cpp simple_memory_manager.cpp solid_notification_example.cpp \
       solid_payment_example.cpp stock_observer.cpp structured_bindings_example.cpp switch_init_example.cpp \
       unique_ptr_demo.cpp vector_capacity_demo.cpp weather_observer.cpp

# Generate executable names from source files (without extensions)
C_EXECS = $(C_SRCS:.c=)
CXX_EXECS = $(CXX_SRCS:.cpp=)

# All executables
ALL_EXECS = $(C_EXECS) $(CXX_EXECS)

# Default target to build all executables
all: c-execs cxx-execs

# Build all C executables
c-execs: $(C_EXECS)

# Build all C++ executables
cxx-execs: $(CXX_EXECS)

# Generic rule for building C executables
%: %.c
	@echo "Building C program: $@..."
	@$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)
	@echo "Done building $@"

# Generic rule for building C++ executables
%: %.cpp
	@echo "Building C++ program: $@..."
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)
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
	@rm -f $(ALL_EXECS)
	@rm -f $(BIN_DIR)/*
	@echo "Clean complete"

# Create the bin directory if it doesn't exist
$(BIN_DIR):
	@mkdir -p $(BIN_DIR)

# Copy all executables to bin directory
install: all $(BIN_DIR)
	@echo "Copying executables to $(BIN_DIR)..."
	@for exec in $(ALL_EXECS); do \
		if [ -f $$exec ]; then \
			cp $$exec $(BIN_DIR)/$$exec 2>/dev/null || true; \
			echo "  - Copied: $$exec"; \
		else \
			echo "  - Skipped: $$exec (not found)"; \
		fi; \
	done
	@echo "All executables copied to $(BIN_DIR)"
	@ls -la $(BIN_DIR)

# List all available targets
help:
	@echo "====== HelpingHand C/C++ Project Makefile ======"
	@echo "Available targets:"
	@echo "  all           - Build all executables (C and C++)"
	@echo "  c-execs       - Build only C executables"
	@echo "  cxx-execs     - Build only C++ executables"
	@echo "  clean         - Remove all executables"
	@echo "  test          - Run test_addition"
	@echo "  run-PROGRAM   - Build and run a specific program (e.g., make run-addition)"
	@echo "  install       - Copy all executables to $(BIN_DIR) directory"
	@echo "  help          - Display this help message"
	@echo ""
	@echo "C Executables ($(words $(C_EXECS))):"
	@for exec in $(C_EXECS); do \
		echo "  $$exec"; \
	done
	@echo ""
	@echo "C++ Executables ($(words $(CXX_EXECS))):"
	@for exec in $(CXX_EXECS); do \
		echo "  $$exec"; \
	done
	@echo "============================================="
