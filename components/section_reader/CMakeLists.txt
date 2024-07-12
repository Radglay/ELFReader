add_library(ElfSectionReader
            src/ElfSectionReaderX64.cpp
            )

target_include_directories(ElfSectionReader PUBLIC
            include
            )

target_link_libraries(ElfSectionReader PRIVATE Logger)
