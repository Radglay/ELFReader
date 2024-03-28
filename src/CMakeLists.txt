add_library(ElfFileParser
            ElfFileParserCreator.cpp
            ElfFileParserX32.cpp
            ElfFileParserX64.cpp
            utility/CheckingFileVersionUtility.cpp)

add_library(Logger OBJECT utility/LoggerSettingUtility.cpp)

target_include_directories(ElfFileParser PUBLIC
                           ../include
                           ../include/structures
                           ../include/utility)
