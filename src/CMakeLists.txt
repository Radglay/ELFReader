add_library(ElfFileParser
            ElfFileParserCreator.cpp
            ElfFileParserX32.cpp
            ElfFileParserX64.cpp
            utility/CheckingFileVersionUtility.cpp
            )

target_include_directories(ElfFileParser PUBLIC
                           ${PROJECT_SOURCE_DIR}/include
                           ${PROJECT_SOURCE_DIR}/include/structures
                           ${PROJECT_SOURCE_DIR}/include/utility
                           ${PROJECT_SOURCE_DIR}/include/exceptions
                           )

add_library(Logger OBJECT utility/LoggerSettingUtility.cpp)

target_include_directories(Logger PUBLIC
                           ${PROJECT_SOURCE_DIR}/include/utility
                           ${PROJECT_SOURCE_DIR}/lib/plog/include
                           )
