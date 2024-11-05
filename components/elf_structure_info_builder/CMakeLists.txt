add_library(ElfStructureInfoBuilder
            src/ElfStructureInfoBuilder.cpp
            ${PROJECT_SOURCE_DIR}/components/common/src/EndiannessUtility.cpp
            )

target_include_directories(ElfStructureInfoBuilder PUBLIC
            include
            ${PROJECT_SOURCE_DIR}/components/common/include
            ${PROJECT_SOURCE_DIR}/components/common/include/structures
            ${PROJECT_SOURCE_DIR}/components/common/include/traits
            )

target_link_libraries(ElfStructureInfoBuilder PRIVATE Logger)
