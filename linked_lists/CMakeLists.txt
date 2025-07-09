cmake_minimum_required(VERSION 3.10)

# Set project name
project(LinkedListTests)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find Google Test
find_package(GTest REQUIRED)
include(GoogleTest)

# Add the source files
add_library(singly_linked_list
    singly_linked_list_recursive.hpp
    singly_linked_list_recursive.cpp
)

# Add the test executable
add_executable(test_linked_list test_linked_list.cpp)

# Link the test executable with the library and Google Test
add_dependencies(test_linked_list singly_linked_list)
target_link_libraries(test_linked_list PRIVATE
    singly_linked_list
    GTest::GTest
    GTest::Main
)

# Enable testing
gtest_discover_tests(test_linked_list)
