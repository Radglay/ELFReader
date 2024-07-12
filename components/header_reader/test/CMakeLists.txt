add_subdirectory("${PROJECT_SOURCE_DIR}/lib/googletest" "lib/googletest")

macro(add_tests_from_file TESTNAME)
    add_executable(${TESTNAME} ${ARGN})
    target_link_libraries(${TESTNAME}
                          gtest
                          gmock
                          gtest_main
                          ElfHeaderReader)
    gtest_discover_tests(${TESTNAME})
endmacro()

add_tests_from_file(ElfHeaderReaderCreatorTest ElfHeaderReaderCreatorTest.cpp)
add_tests_from_file(ElfHeaderReaderX32FileHeaderTest ElfHeaderReaderX32FileHeaderTest.cpp)
add_tests_from_file(ElfHeaderReaderX32ProgramHeaderTest ElfHeaderReaderX32ProgramHeaderTest.cpp)
add_tests_from_file(ElfHeaderReaderX32SectionHeaderTest ElfHeaderReaderX32SectionHeaderTest.cpp)
add_tests_from_file(ElfHeaderReaderX64FileHeaderTest ElfHeaderReaderX64FileHeaderTest.cpp)
add_tests_from_file(ElfHeaderReaderX64ProgramHeaderTest ElfHeaderReaderX64ProgramHeaderTest.cpp)
add_tests_from_file(ElfHeaderReaderX64SectionHeaderTest ElfHeaderReaderX64SectionHeaderTest.cpp)
