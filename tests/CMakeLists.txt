add_subdirectory("${PROJECT_SOURCE_DIR}/lib/googletest" "lib/googletest")

add_executable(ElfFileParserCreatorTest ElfFileParserCreatorTest.cpp)

target_link_libraries(ElfFileParserCreatorTest gtest gmock gtest_main)
target_link_libraries(ElfFileParserCreatorTest ElfFileParser)

gtest_discover_tests(ElfFileParserCreatorTest)
