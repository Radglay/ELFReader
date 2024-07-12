add_library(ElfHeaderReader
            ElfHeaderReaderCreator.cpp
            ElfHeaderReaderX32.cpp
            ElfHeaderReaderX64.cpp
            utility/CheckingFileVersionUtility.cpp
            utility/EndiannessUtility.cpp
            )

target_include_directories(ElfHeaderReader PUBLIC
                           ${PROJECT_SOURCE_DIR}/include
                           ${PROJECT_SOURCE_DIR}/include/structures
                           ${PROJECT_SOURCE_DIR}/include/utility
                           ${PROJECT_SOURCE_DIR}/include/exceptions
                           )

add_library(ElfSectionReader
            ElfSectionReaderX64.cpp)

target_include_directories(ElfSectionReader PUBLIC
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
