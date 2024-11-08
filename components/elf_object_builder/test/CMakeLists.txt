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

add_tests_from_file(Elf32BitElfStructureInfoBuildingTest Elf32BitElfStructureInfoBuildingTest.cpp)
add_tests_from_file(Elf64BitElfStructureInfoBuildingTest Elf64BitElfStructureInfoBuildingTest.cpp)


add_tests_from_file(Elf32BitSymbolHeadersBuildingTest Elf32BitSymbolHeadersBuildingTest.cpp)
add_tests_from_file(Elf64BitSymbolHeadersBuildingTest Elf64BitSymbolHeadersBuildingTest.cpp)


add_tests_from_file(Elf32BitNoteHeaderBuildingTest Elf32BitNoteHeaderBuildingTest.cpp)
add_tests_from_file(Elf64BitNoteHeaderBuildingTest Elf64BitNoteHeaderBuildingTest.cpp)


add_tests_from_file(Elf32BitRelocationWithAddendHeadersBuildingTest Elf32BitRelocationWithAddendHeadersBuildingTest.cpp)
add_tests_from_file(Elf64BitRelocationWithAddendHeadersBuildingTest Elf64BitRelocationWithAddendHeadersBuildingTest.cpp)


add_tests_from_file(Elf32BitRelocationHeadersBuildingTest Elf32BitRelocationHeadersBuildingTest.cpp)
add_tests_from_file(Elf64BitRelocationHeadersBuildingTest Elf64BitRelocationHeadersBuildingTest.cpp)
