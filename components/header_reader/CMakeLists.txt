add_library(ElfHeaderReader
            src/ElfHeaderReaderCreator.cpp
            src/ElfHeaderReaderX32.cpp
            src/ElfHeaderReaderX64.cpp
            src/CheckingFileVersionUtility.cpp
            ${PROJECT_SOURCE_DIR}/components/common/src/EndiannessUtility.cpp
            )

target_include_directories(ElfHeaderReader PUBLIC
            include
            ${PROJECT_SOURCE_DIR}/components/common/include
            ${PROJECT_SOURCE_DIR}/components/common/include/structures
            ${PROJECT_SOURCE_DIR}/components/common/include/exceptions
            )

target_link_libraries(ElfHeaderReader PRIVATE Logger)
