add_library(ElfStructureInfoBuilder
            src/ElfStructureInfoBuilderX32.cpp
            src/ElfStructureInfoBuilderX64.cpp
            ${PROJECT_SOURCE_DIR}/components/common/src/EndiannessUtility.cpp
            )

target_include_directories(ElfStructureInfoBuilder PUBLIC
            include
            ${PROJECT_SOURCE_DIR}/components/common/include
            ${PROJECT_SOURCE_DIR}/components/common/include/structures
            )

target_link_libraries(ElfStructureInfoBuilder PRIVATE Logger)
