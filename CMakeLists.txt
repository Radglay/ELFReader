cmake_minimum_required(VERSION 3.10)

project(ELFParser
        VERSION 1.0
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(TO_CMAKE_PATH "${PROJECT_BINARY_DIR}/CMakeLists.txt" LOC_PATH)
if(EXISTS "${LOC_PATH}")
    message(FATAL_ERROR "You cannot build in a source directory (or any directory with a CMakeLists.txt file). Please make a build subdirectory. Feel free to remove CMakeCache.txt and CMakeFiles.")
endif()

add_subdirectory(lib/plog)
include_directories(lib/plog/include)
file(MAKE_DIRECTORY logs)

add_subdirectory(src)
add_subdirectory(app)


option(BUILD_TESTS "Build ElfFileParser unit tests" OFF)
if (BUILD_TESTS)
    enable_testing()
    include(GoogleTest)
    add_subdirectory(tests)
endif()
unset(BUILD_TESTS CACHE)