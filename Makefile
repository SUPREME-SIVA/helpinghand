CC = gcc
CXX = g++
CFLAGS = -Wall -Wextra -std=c99 -g
CXXFLAGS = -Wall -Wextra -std=c++17 -g
LDFLAGS = -lm

# Directory for executables
BIN_DIR = bin
TESTS_DIR = tests
EXAMPLES_DIR = examples
DESIGN_PATTERNS_DIR = design_patterns
BIT_MANIPULATION_DIR = bit_manipulation

# List of all C source files
C_SRCS = addition.c Palindrome.c StringRevers.c array_rotation.c bit_flip.c \
       memory_example.c processor_simulation.c simple_array_rotation.c swap_even_numbers.c test.c \
       $(BIT_MANIPULATION_DIR)/binary_print_and_swap.c

# List of all C++ source files
CXX_SRCS = advanced_solid_example.cpp array_rotation.cpp casting_examples.cpp constexpr_optimization.cpp \
       copy_constructor_example.cpp custom_deleter.cpp custom_memory_manager.cpp data_container_example.cpp \
       database_singleton.cpp document_factory.cpp file_processor.cpp if_init_example.cpp logger_singleton.cpp \
       non_virtual_access.cpp optimized_derived_access.cpp packet_handler_example.cpp parallel_sum_improved.cpp \
       payment_factory.cpp shared_ptr_demo.cpp simple_memory_manager.cpp solid_notification_example.cpp \
       solid_payment_example.cpp stock_observer.cpp structured_bindings_example.cpp switch_init_example.cpp \
       unique_ptr_demo.cpp vector_capacity_demo.cpp weather_observer.cpp \
       $(BIT_MANIPULATION_DIR)/binary_print_and_swap.cpp

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

# List of all filesystem example files
FILESYSTEM_SRCS = $(wildcard filesystem/*.cpp)

# List of all C++ design pattern files
CXX_DESIGN_PATTERN_SRCS = $(DESIGN_PATTERNS_DIR)/thread_safe_singleton.cpp \
       $(DESIGN_PATTERNS_DIR)/singleton_interview_guide.cpp

# List of all interview question files
INTERVIEW_QUESTIONS_DIR = interview_questions
C_INTERVIEW_QUESTION_SRCS = $(INTERVIEW_QUESTIONS_DIR)/substring_search.c \
       $(INTERVIEW_QUESTIONS_DIR)/bit_counting.c
CXX_INTERVIEW_QUESTION_SRCS = $(INTERVIEW_QUESTIONS_DIR)/substring_search.cpp \
       $(INTERVIEW_QUESTIONS_DIR)/bit_counting.cpp \
       $(INTERVIEW_QUESTIONS_DIR)/array_sorting.cpp

# List of all linked list implementations
LINKED_LISTS_DIR = linked_lists
C_LINKED_LISTS_SRCS = $(LINKED_LISTS_DIR)/singly_linked_list.c
CXX_LINKED_LISTS_SRCS = $(LINKED_LISTS_DIR)/singly_linked_list.cpp

# Generate executable names from source files (without extensions)
C_EXECS = $(C_SRCS:.c=)
CXX_EXECS = $(CXX_SRCS:.cpp=)
C_TEST_EXECS = $(notdir $(C_TEST_SRCS:.c=))
CXX_TEST_EXECS = $(notdir $(CXX_TEST_SRCS:.cpp=))
CXX_EXAMPLE_EXECS = $(notdir $(CXX_EXAMPLE_SRCS:.cpp=))
CXX_DESIGN_PATTERN_EXECS = $(notdir $(CXX_DESIGN_PATTERN_SRCS:.cpp=))

# All executables
ALL_EXECS = $(C_EXECS) $(CXX_EXECS)
ALL_C_TEST_EXECS = $(addprefix $(TESTS_DIR)/, $(C_TEST_EXECS))
ALL_CXX_TEST_EXECS = $(addprefix $(TESTS_DIR)/, $(CXX_TEST_EXECS))
ALL_TEST_EXECS = $(ALL_C_TEST_EXECS) $(ALL_CXX_TEST_EXECS)
ALL_CXX_EXAMPLE_EXECS = $(addprefix $(EXAMPLES_DIR)/, $(CXX_EXAMPLE_EXECS))
ALL_CXX_DESIGN_PATTERN_EXECS = $(addprefix $(DESIGN_PATTERNS_DIR)/, $(CXX_DESIGN_PATTERN_EXECS))
FILESYSTEM_EXECS = $(FILESYSTEM_SRCS:.cpp=)
C_INTERVIEW_QUESTION_EXECS = $(C_INTERVIEW_QUESTION_SRCS:.c=)
CXX_INTERVIEW_QUESTION_EXECS = $(CXX_INTERVIEW_QUESTION_SRCS:.cpp=)
ALL_INTERVIEW_QUESTION_EXECS = $(C_INTERVIEW_QUESTION_EXECS) $(CXX_INTERVIEW_QUESTION_EXECS)
C_LINKED_LISTS_EXECS = $(C_LINKED_LISTS_SRCS:.c=)
CXX_LINKED_LISTS_EXECS = $(CXX_LINKED_LISTS_SRCS:.cpp=)
ALL_LINKED_LISTS_EXECS = $(C_LINKED_LISTS_EXECS) $(CXX_LINKED_LISTS_EXECS)
BIT_MANIPULATION_EXECS = $(BIT_MANIPULATION_DIR)/binary_print_and_swap

# Default target to build all executables
all: c-execs cxx-execs examples design-patterns filesystem interview-questions linked-lists bit-manipulation

# Build all C executables
c-execs: $(C_EXECS)

# Build all C++ executables
cxx-execs: $(CXX_EXECS)

# Build all example executables
examples: cxx-examples

# Build all C++ example executables
cxx-examples: $(ALL_CXX_EXAMPLE_EXECS)

# Build all design pattern executables
design-patterns: $(ALL_CXX_DESIGN_PATTERN_EXECS)

# Build all filesystem executables
filesystem: $(FILESYSTEM_EXECS)

# Build all test executables
tests: c-tests cxx-tests

# Build all C test executables
c-tests: $(ALL_C_TEST_EXECS)

# Build all C++ test executables
cxx-tests: $(ALL_CXX_TEST_EXECS)

# Build all interview question executables
interview-questions: c-interview-questions cxx-interview-questions

# Build all C interview question executables
c-interview-questions: $(C_INTERVIEW_QUESTION_EXECS)

# Build all C++ interview question executables
cxx-interview-questions: $(CXX_INTERVIEW_QUESTION_EXECS)

# Build all linked list implementations
linked-lists: c-linked-lists cxx-linked-lists

# Build all C linked list implementations
c-linked-lists: $(C_LINKED_LISTS_EXECS)

# Build all C++ linked list implementations
cxx-linked-lists: $(CXX_LINKED_LISTS_EXECS)

# Build all bit manipulation implementations
bit-manipulation: $(BIT_MANIPULATION_EXECS)

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

# Generic rule for building C++ design pattern executables
$(DESIGN_PATTERNS_DIR)/%: $(DESIGN_PATTERNS_DIR)/%.cpp
	@echo "Building C++ design pattern: $@..."
	@$(CXX) $(CXXFLAGS) -pthread -o $@ $< $(LDFLAGS)
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

# Run all design patterns
run-design-patterns: design-patterns
	@echo "Running all design patterns..."
	@for pattern in $(ALL_CXX_DESIGN_PATTERN_EXECS); do \
		echo "=== Running $$pattern ==="; \
		./$$pattern; \
		echo ""; \
	done
	@echo "All design patterns completed"

# Run all filesystem examples
run-filesystem: filesystem
	@echo "Running all filesystem examples..."
	@for fs_example in $(FILESYSTEM_EXECS); do \
		echo "=== Running $$fs_example ==="; \
		./$$fs_example; \
		echo ""; \
	done
	@echo "All filesystem examples completed"

# Run a specific test
run-test-%: $(TESTS_DIR)/%
	@echo "Running test $*..."
	@$(TESTS_DIR)/$*

# Run a specific example
run-example-%: $(EXAMPLES_DIR)/%
	@echo "Running example $*..."
	@$(EXAMPLES_DIR)/$*

# Run a specific design pattern
run-design-pattern-%: $(DESIGN_PATTERNS_DIR)/%
	@echo "Running design pattern $*..."
	@$(DESIGN_PATTERNS_DIR)/$*

# Run all interview question executables
run-interview-questions: interview-questions
	@echo "Running all interview questions..."
	@for question in $(ALL_INTERVIEW_QUESTION_EXECS); do \
		echo "=== Running $$question ==="; \
		./$$question; \
		echo ""; \
	done
	@echo "All interview questions completed"

# Run all C interview question executables
run-c-interview-questions: c-interview-questions
	@echo "Running C interview questions..."
	@for question in $(C_INTERVIEW_QUESTION_EXECS); do \
		echo "=== Running $$question ==="; \
		./$$question; \
		echo ""; \
	done
	@echo "All C interview questions completed"

# Run all C++ interview question executables
run-cxx-interview-questions: cxx-interview-questions
	@echo "Running C++ interview questions..."
	@for question in $(CXX_INTERVIEW_QUESTION_EXECS); do \
		echo "=== Running $$question ==="; \
		./$$question; \
		echo ""; \
	done
	@echo "All C++ interview questions completed"

# Run a specific interview question
run-interview-question-%: $(INTERVIEW_QUESTIONS_DIR)/%
	@echo "Running interview question $*..."
	@$(INTERVIEW_QUESTIONS_DIR)/$*

# Run a specific filesystem example
run-filesystem-%: filesystem/%
	@echo "Running filesystem example $*..."
	@filesystem/$*

# Run all linked list implementations
run-linked-lists: linked-lists
	@echo "Running all linked list implementations..."
	@for impl in $(ALL_LINKED_LISTS_EXECS); do \
		echo "=== Running $$impl ==="; \
		./$$impl; \
		echo ""; \
	done
	@echo "All linked list implementations completed"

# Run all C linked list implementations
run-c-linked-lists: c-linked-lists
	@echo "Running C linked list implementations..."
	@for impl in $(C_LINKED_LISTS_EXECS); do \
		echo "=== Running $$impl ==="; \
		./$$impl; \
		echo ""; \
	done
	@echo "All C linked list implementations completed"

# Run all C++ linked list implementations
run-cxx-linked-lists: cxx-linked-lists
	@echo "Running C++ linked list implementations..."
	@for impl in $(CXX_LINKED_LISTS_EXECS); do \
		echo "=== Running $$impl ==="; \
		./$$impl; \
		echo ""; \
	done
	@echo "All C++ linked list implementations completed"

# Run all bit manipulation implementations
run-bit-manipulation: bit-manipulation
	@echo "Running all bit manipulation implementations..."
	@for impl in $(BIT_MANIPULATION_EXECS); do \
		echo "=== Running $$impl ==="; \
		./$$impl; \
		echo ""; \
	done
	@echo "All bit manipulation implementations completed"

# Run a specific bit manipulation implementation
run-bit-manipulation-%: $(BIT_MANIPULATION_DIR)/%
	@echo "Running bit manipulation implementation: $*"
	@if [ -f "$(BIT_MANIPULATION_DIR)/$*.c" ] && [ -x "$(BIT_MANIPULATION_DIR)/$*" ]; then \
		./$(BIT_MANIPULATION_DIR)/$*; \
	elif [ -f "$(BIT_MANIPULATION_DIR)/$*.cpp" ] && [ -x "$(BIT_MANIPULATION_DIR)/$*" ]; then \
		./$(BIT_MANIPULATION_DIR)/$*; \
	else \
		echo "Error: $* is not a valid bit manipulation implementation"; \
		exit 1; \
	fi

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
	@rm -f $(ALL_CXX_DESIGN_PATTERN_EXECS)
	@rm -f $(FILESYSTEM_EXECS)
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

# Create the design patterns directory if it doesn't exist
$(DESIGN_PATTERNS_DIR):
	@mkdir -p $(DESIGN_PATTERNS_DIR)

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
	@for exec in $(ALL_CXX_DESIGN_PATTERN_EXECS); do \
		if [ -f $$exec ]; then \
			cp $$exec $(BIN_DIR)/$$exec 2>/dev/null || true; \
			echo "  - Copied: $$exec"; \
		else \
			echo "  - Skipped: $$exec (not found)"; \
		fi; \
	done
	@for exec in $(FILESYSTEM_EXECS); do \
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
	@echo "  all                   - Build all executables (C, C++, examples, design patterns, filesystem, interview questions, linked lists, and bit manipulation)"
	@echo "  c-execs               - Build only C executables"
	@echo "  cxx-execs             - Build only C++ executables"
	@echo "  examples              - Build all example executables"
	@echo "  design-patterns       - Build all design pattern executables"
	@echo "  filesystem            - Build all filesystem executables"
	@echo "  tests                 - Build all test executables"
	@echo "  interview-questions   - Build all interview question executables"
	@echo "  linked-lists          - Build all linked list implementations"
	@echo "  bit-manipulation      - Build all bit manipulation implementations"
	@echo "  run-tests             - Run all tests"
	@echo "  run-c-tests           - Run all C tests"
	@echo "  run-cxx-tests         - Run all C++ tests"
	@echo "  run-examples          - Run all examples"
	@echo "  run-design-patterns   - Run all design patterns"
	@echo "  run-filesystem        - Run all filesystem examples"
	@echo "  run-interview-questions - Run all interview questions"
	@echo "  run-linked-lists      - Run all linked list implementations"
	@echo "  run-bit-manipulation  - Run all bit manipulation implementations"
	@echo "  run-bit-manipulation-NAME - Run a specific bit manipulation implementation"
	@echo "  clean                 - Remove all executables"
	@echo "  help                  - Display this help message"
	@echo ""
	@echo "For more information, see the README.md file."
