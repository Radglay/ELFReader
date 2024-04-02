add_subdirectory("${PROJECT_SOURCE_DIR}/lib/googletest" "lib/googletest")

add_executable(ElfFileParserCreatorTest ElfFileParserCreatorTest.cpp)

target_link_libraries(ElfFileParserCreatorTest gtest gmock gtest_main)
target_link_libraries(ElfFileParserCreatorTest ElfFileParser)


add_executable(ElfFileParserTest ElfFileParserX32Test.cpp)
target_link_libraries(ElfFileParserTest gtest gmock gtest_main)
target_link_libraries(ElfFileParserTest ElfFileParser)


gtest_discover_tests(ElfFileParserCreatorTest)
gtest_discover_tests(ElfFileParserTest)
