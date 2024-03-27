add_library(ElfFileParser
            ElfFileParserCreator.cpp
            ElfFileParserX32.cpp
            ElfFileParserX64.cpp
            CheckingFileVersionUtility.cpp)

target_include_directories(ElfFileParser PUBLIC
                           ../include
                           ../include/structures
                           ../include/utility)
