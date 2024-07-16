macro(add_tests_from_file TESTNAME)
    add_executable(${TESTNAME} ${ARGN})
    target_link_libraries(${TESTNAME}
                          gtest
                          gmock
                          gtest_main
                          ElfSectionReader)
    gtest_discover_tests(${TESTNAME})
endmacro()

add_tests_from_file(ElfSectionReaderX64SymbolTableTest ElfSectionReaderX64SymbolTableTest.cpp)
