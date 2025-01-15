macro(add_tests_from_file TESTNAME)
    add_executable(${TESTNAME} ${ARGN})

    target_include_directories(${TESTNAME} PRIVATE mock common)
    target_link_libraries(${TESTNAME}
                          gtest
                          gmock
                          gtest_main
                          ElfPartAssembler
                          ElfPartTestingUtility)
    gtest_discover_tests(${TESTNAME})
endmacro()

add_subdirectory(common)

add_tests_from_file(FieldDescriptionUtilityTest FieldDescriptionUtilityTest.cpp)
add_tests_from_file(ElfPartFromFileHeaderAssemblingTest ElfPartFromFileHeaderAssemblingTest.cpp)
