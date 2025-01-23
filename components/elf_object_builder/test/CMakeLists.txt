macro(add_tests_from_file TESTNAME)
    add_executable(${TESTNAME} ${ARGN} mock/ElfStructureInfoBuilderMock.hpp)
    target_include_directories(${TESTNAME} PRIVATE mock)
    target_link_libraries(${TESTNAME}
                          gtest
                          gmock
                          gtest_main
                          ElfObjectBuilder)
    gtest_discover_tests(${TESTNAME})
endmacro()


add_tests_from_file(Elf32BitSymbolTableSectionsBuildingTest Elf32BitSymbolTableSectionsBuildingTest.cpp)
add_tests_from_file(Elf64BitSymbolTableSectionsBuildingTest Elf64BitSymbolTableSectionsBuildingTest.cpp)


add_tests_from_file(Elf32BitNoteSectionsBuildingTest Elf32BitNoteSectionsBuildingTest.cpp)
add_tests_from_file(Elf64BitNoteSectionsBuildingTest Elf64BitNoteSectionsBuildingTest.cpp)


add_tests_from_file(Elf32BitRelocationWithAddendSectionsBuildingTest Elf32BitRelocationWithAddendSectionsBuildingTest.cpp)
add_tests_from_file(Elf64BitRelocationWithAddendSectionsBuildingTest Elf64BitRelocationWithAddendSectionsBuildingTest.cpp)


add_tests_from_file(Elf32BitRelocationSectionsBuildingTest Elf32BitRelocationSectionsBuildingTest.cpp)
add_tests_from_file(Elf64BitRelocationSectionsBuildingTest Elf64BitRelocationSectionsBuildingTest.cpp)


add_tests_from_file(Elf32BitStringTableSectionsBuildingTest Elf32BitStringTableSectionsBuildingTest.cpp)
add_tests_from_file(Elf64BitStringTableSectionsBuildingTest Elf64BitStringTableSectionsBuildingTest.cpp)


add_tests_from_file(Elf32BitElfBuildDirectorTest Elf32BitElfBuildDirectorTest.cpp)
add_tests_from_file(Elf64BitElfBuildDirectorTest Elf64BitElfBuildDirectorTest.cpp)
