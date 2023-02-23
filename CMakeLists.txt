cmake_minimum_required(VERSION 3.16)
if("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")
    message(FATAL_ERROR "Please remove CMakeCache.txt and CMakeFiles/ "
                        "directory then build ouf-of-source.")
endif()

project(libcmap VERSION 0.0.1)

#
# Default compiler setup
#
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

# Enable all compiler warnings by default and make them errors
add_compile_options(-Wall -Werror)

#
# CMake setup
#
list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

include(CTest)

# Enable code coverage support
include(code-coverage)
add_code_coverage_all_targets(
    EXCLUDE
        /usr/include/*
        ${PROJECT_SOURCE_DIR}/extern/*
        ${PROJECT_BINARY_DIR}/*
)

# Enable the use of compiler sanitizers as a build option
include(sanitizers)

# Enable threads as a default package
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

# Add external dependenices (should be included first)
include(extern/dependencies.cmake)

# Add source directories
option(LIBCMAP_TIDY "run clang-tidy" OFF)
if(LIBCMAP_TIDY)
    set(CMAKE_C_CLANG_TIDY "clang-tidy;--header-filter=./src/*")
    message(STATUS "Enabled clang-tidy")
endif()

add_subdirectory(src)
add_subdirectory(tests)
