cmake_minimum_required(VERSION 3.10)
project(RushHourV.1.0)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set output directory for binaries (changed to output to project root/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)

# Include directories
include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/include/model
    ${CMAKE_SOURCE_DIR}/include/view
    ${CMAKE_SOURCE_DIR}/include/controller
)

# Collect source files
file(GLOB_RECURSE SOURCES 
    "${CMAKE_SOURCE_DIR}/src/model/*.cpp"
    "${CMAKE_SOURCE_DIR}/src/view/*.cpp"
    "${CMAKE_SOURCE_DIR}/src/controller/*.cpp"
)

# Collect header files (optional, for IDEs)
file(GLOB_RECURSE HEADERS 
    "${CMAKE_SOURCE_DIR}/include/*.h"
    "${CMAKE_SOURCE_DIR}/include/*.hpp"
)

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Optionally, add compiler warnings
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
elseif(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4)
endif()

# Add subdirectory for tests if needed
if(EXISTS "${CMAKE_SOURCE_DIR}/test/CMakeLists.txt")
    add_subdirectory(test)
endif()

# Optional: Create install target
install(TARGETS ${PROJECT_NAME} DESTINATION bin)

# Make sure the bin directory exists
file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)