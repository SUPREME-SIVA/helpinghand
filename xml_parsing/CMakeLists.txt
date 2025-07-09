cmake_minimum_required(VERSION 3.10)
project(xml_firmware_parser)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find pugixml package
find_package(pugixml QUIET)

# If pugixml is not found, use FetchContent to download it
if(NOT pugixml_FOUND)
    include(FetchContent)
    FetchContent_Declare(
        pugixml
        GIT_REPOSITORY https://github.com/zeux/pugixml.git
        GIT_TAG v1.13
    )
    FetchContent_MakeAvailable(pugixml)
endif()

# Add the executable
add_executable(xml_firmware_parser xml_firmware_parser.cpp)

# Link against pugixml
target_link_libraries(xml_firmware_parser PRIVATE pugixml)
