macro(add_tests_from_file TESTNAME)
    add_executable(${TESTNAME} ${ARGN})
    target_include_directories(${TESTNAME} PRIVATE mock)
    target_link_libraries(${TESTNAME}
                          gtest
                          gmock
                          gtest_main
                          ElfPartAssembler)
    gtest_discover_tests(${TESTNAME})
endmacro()

add_tests_from_file(FieldDescriptionUtilityTest FieldDescriptionUtilityTest.cpp)
