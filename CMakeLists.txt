cmake_minimum_required(VERSION 3.28)

project(ELFReader
        VERSION 1.0
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(TO_CMAKE_PATH "${PROJECT_BINARY_DIR}/CMakeLists.txt" LOC_PATH)
if(EXISTS "${LOC_PATH}")
    message(FATAL_ERROR "You cannot build in a source directory (or any directory with a CMakeLists.txt file). Please make a build subdirectory. Feel free to remove CMakeCache.txt and CMakeFiles.")
endif()

add_subdirectory(lib/plog)
file(MAKE_DIRECTORY logs)

add_subdirectory(components/ui)
add_subdirectory(components/common)
add_subdirectory(components/elf_structure_info_builder)
add_subdirectory(components/elf_parts_assembler)
add_subdirectory(components/elf_section_builder)
add_subdirectory(app)

option(BUILD_TESTS "Build all unit tests" OFF)
if (BUILD_TESTS)
    enable_testing()
    include(GoogleTest)
    add_subdirectory(lib/googletest)

    add_subdirectory(components/elf_structure_info_builder/test)
    add_subdirectory(components/elf_section_builder/test)
    add_subdirectory(components/elf_parts_assembler/test)
endif()
unset(BUILD_TESTS CACHE)
