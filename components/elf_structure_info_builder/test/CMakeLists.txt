macro(add_tests_from_file TESTNAME)
    add_executable(${TESTNAME} ${ARGN})
    target_link_libraries(${TESTNAME}
                          gtest
                          gmock
                          gtest_main
                          ElfStructureInfoBuilder)
    gtest_discover_tests(${TESTNAME})
endmacro()

add_tests_from_file(Elf32BitFileHeaderBuildingTest Elf32BitFileHeaderBuildingTest.cpp)
add_tests_from_file(Elf64BitFileHeaderBuildingTest Elf64BitFileHeaderBuildingTest.cpp)

add_tests_from_file(Elf32BitProgramHeadersBuildingTest Elf32BitProgramHeadersBuildingTest.cpp)
add_tests_from_file(Elf64BitProgramHeadersBuildingTest Elf64BitProgramHeadersBuildingTest.cpp)

add_tests_from_file(Elf32BitSectionHeadersBuildingTest Elf32BitSectionHeadersBuildingTest.cpp)
add_tests_from_file(Elf64BitSectionHeadersBuildingTest Elf64BitSectionHeadersBuildingTest.cpp)
