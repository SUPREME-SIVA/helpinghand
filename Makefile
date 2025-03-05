CC = gcc
CXX = g++
CFLAGS = -Wall -Wextra -std=c99 -g
CXXFLAGS = -Wall -Wextra -std=c++17 -g
LDFLAGS = -lm

# Directory for executables
BIN_DIR = bin
TESTS_DIR = tests
EXAMPLES_DIR = examples

# List of all C source files
C_SRCS = addition.c Palindrome.c StringRevers.c array_rotation.c bit_flip.c \
       memory_example.c processor_simulation.c simple_array_rotation.c swap_even_numbers.c test.c

# List of all C++ source files
CXX_SRCS = advanced_solid_example.cpp array_rotation.cpp casting_examples.cpp constexpr_optimization.cpp \
       copy_constructor_example.cpp custom_deleter.cpp custom_memory_manager.cpp data_container_example.cpp \
       database_singleton.cpp document_factory.cpp file_processor.cpp if_init_example.cpp logger_singleton.cpp \
       non_virtual_access.cpp optimized_derived_access.cpp packet_handler_example.cpp parallel_sum_improved.cpp \
       payment_factory.cpp shared_ptr_demo.cpp simple_memory_manager.cpp solid_notification_example.cpp \
       solid_payment_example.cpp stock_observer.cpp structured_bindings_example.cpp switch_init_example.cpp \
       unique_ptr_demo.cpp vector_capacity_demo.cpp weather_observer.cpp

# List of all C test files
C_TEST_SRCS = $(TESTS_DIR)/test_palindrome.c $(TESTS_DIR)/test_string_reverse.c \
       $(TESTS_DIR)/test_array_rotation.c $(TESTS_DIR)/test_bit_flip.c $(TESTS_DIR)/test_addition.c

# List of all C++ test files
CXX_TEST_SRCS = $(TESTS_DIR)/test_shared_ptr_demo.cpp $(TESTS_DIR)/test_unique_ptr_demo.cpp \
       $(TESTS_DIR)/test_weather_observer.cpp $(TESTS_DIR)/test_vector_capacity_demo.cpp \
       $(TESTS_DIR)/test_logger_singleton.cpp

# List of all C++ example files
CXX_EXAMPLE_SRCS = $(EXAMPLES_DIR)/vector_basics.cpp $(EXAMPLES_DIR)/vector_custom_objects.cpp \
       $(EXAMPLES_DIR)/vector_advanced.cpp $(EXAMPLES_DIR)/list_basics.cpp \
       $(EXAMPLES_DIR)/list_lru_cache.cpp $(EXAMPLES_DIR)/list_priority_task_manager.cpp

# Generate executable names from source files (without extensions)
C_EXECS = $(C_SRCS:.c=)
CXX_EXECS = $(CXX_SRCS:.cpp=)
C_TEST_EXECS = $(notdir $(C_TEST_SRCS:.c=))
CXX_TEST_EXECS = $(notdir $(CXX_TEST_SRCS:.cpp=))
CXX_EXAMPLE_EXECS = $(notdir $(CXX_EXAMPLE_SRCS:.cpp=))

# All executables
ALL_EXECS = $(C_EXECS) $(CXX_EXECS)
ALL_C_TEST_EXECS = $(addprefix $(TESTS_DIR)/, $(C_TEST_EXECS))
ALL_CXX_TEST_EXECS = $(addprefix $(TESTS_DIR)/, $(CXX_TEST_EXECS))
ALL_TEST_EXECS = $(ALL_C_TEST_EXECS) $(ALL_CXX_TEST_EXECS)
ALL_CXX_EXAMPLE_EXECS = $(addprefix $(EXAMPLES_DIR)/, $(CXX_EXAMPLE_EXECS))

# Default target to build all executables
all: c-execs cxx-execs examples

# Build all C executables
c-execs: $(C_EXECS)

# Build all C++ executables
cxx-execs: $(CXX_EXECS)

# Build all example executables
examples: cxx-examples

# Build all C++ example executables
cxx-examples: $(ALL_CXX_EXAMPLE_EXECS)

# Build all test executables
tests: c-tests cxx-tests

# Build all C test executables
c-tests: $(ALL_C_TEST_EXECS)

# Build all C++ test executables
cxx-tests: $(ALL_CXX_TEST_EXECS)

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

# Generic rule for building C test executables
$(TESTS_DIR)/%: $(TESTS_DIR)/%.c
	@echo "Building C test: $@..."
	@$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)
	@echo "Done building $@"

# Generic rule for building C++ test executables
$(TESTS_DIR)/%: $(TESTS_DIR)/%.cpp
	@echo "Building C++ test: $@..."
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)
	@echo "Done building $@"

# Generic rule for building C++ example executables
$(EXAMPLES_DIR)/%: $(EXAMPLES_DIR)/%.cpp
	@echo "Building C++ example: $@..."
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)
	@echo "Done building $@"

# Run all tests
run-tests: tests
	@echo "Running all tests..."
	@for test in $(ALL_TEST_EXECS); do \
		echo "=== Running $$test ==="; \
		./$$test; \
		echo ""; \
	done
	@echo "All tests completed"

# Run all C tests
run-c-tests: c-tests
	@echo "Running C tests..."
	@for test in $(ALL_C_TEST_EXECS); do \
		echo "=== Running $$test ==="; \
		./$$test; \
		echo ""; \
	done
	@echo "All C tests completed"

# Run all C++ tests
run-cxx-tests: cxx-tests
	@echo "Running C++ tests..."
	@for test in $(ALL_CXX_TEST_EXECS); do \
		echo "=== Running $$test ==="; \
		./$$test; \
		echo ""; \
	done
	@echo "All C++ tests completed"

# Run all examples
run-examples: examples
	@echo "Running all examples..."
	@for example in $(ALL_CXX_EXAMPLE_EXECS); do \
		echo "=== Running $$example ==="; \
		./$$example; \
		echo ""; \
	done
	@echo "All examples completed"

# Run a specific test
run-test-%: $(TESTS_DIR)/%
	@echo "Running test $*..."
	@$(TESTS_DIR)/$*

# Run a specific example
run-example-%: $(EXAMPLES_DIR)/%
	@echo "Running example $*..."
	@$(EXAMPLES_DIR)/$*

# Run a specific program
run-%: %
	@echo "Running $*..."
	@./$*

# Clean all executables
clean:
	@echo "Cleaning up executables..."
	@rm -f $(ALL_EXECS)
	@rm -f $(ALL_TEST_EXECS)
	@rm -f $(ALL_CXX_EXAMPLE_EXECS)
	@rm -f $(BIN_DIR)/*
	@echo "Clean complete"

# Create the bin directory if it doesn't exist
$(BIN_DIR):
	@mkdir -p $(BIN_DIR)

# Create the tests directory if it doesn't exist
$(TESTS_DIR):
	@mkdir -p $(TESTS_DIR)

# Create the examples directory if it doesn't exist
$(EXAMPLES_DIR):
	@mkdir -p $(EXAMPLES_DIR)

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
	@for exec in $(ALL_CXX_EXAMPLE_EXECS); do \
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
	@echo "  all           - Build all executables (C, C++, and examples)"
	@echo "  c-execs       - Build only C executables"
	@echo "  cxx-execs     - Build only C++ executables"
	@echo "  examples      - Build all example executables"
	@echo "  tests         - Build all test executables (C and C++)"
	@echo "  c-tests       - Build only C test executables"
	@echo "  cxx-tests     - Build only C++ test executables"
	@echo "  run-tests     - Build and run all tests"
	@echo "  run-c-tests   - Build and run C tests"
	@echo "  run-cxx-tests - Build and run C++ tests"
	@echo "  run-examples  - Build and run all examples"
	@echo "  run-test-NAME - Build and run a specific test (e.g., make run-test-test_palindrome)"
	@echo "  run-example-NAME - Build and run a specific example (e.g., make run-example-vector_basics)"
	@echo "  clean         - Remove all executables"
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
	@echo ""
	@echo "C Test Executables ($(words $(C_TEST_EXECS))):"
	@for exec in $(C_TEST_EXECS); do \
		echo "  $$exec"; \
	done
	@echo ""
	@echo "C++ Test Executables ($(words $(CXX_TEST_EXECS))):"
	@for exec in $(CXX_TEST_EXECS); do \
		echo "  $$exec"; \
	done
	@echo ""
	@echo "C++ Example Executables ($(words $(CXX_EXAMPLE_EXECS))):"
	@for exec in $(CXX_EXAMPLE_EXECS); do \
		echo "  $$exec"; \
	done
	@echo "============================================="
