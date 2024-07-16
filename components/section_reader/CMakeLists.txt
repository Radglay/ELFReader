add_library(ElfSectionReader
            src/ElfSectionReaderX64.cpp
            ${PROJECT_SOURCE_DIR}/components/common/src/EndiannessUtility.cpp
            )

target_include_directories(ElfSectionReader PUBLIC
            include
            ${PROJECT_SOURCE_DIR}/components/common/include
            )

target_link_libraries(ElfSectionReader PRIVATE Logger)
