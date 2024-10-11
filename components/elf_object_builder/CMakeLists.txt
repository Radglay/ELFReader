add_library(ElfObjectBuilder
            src/ElfObjectBuilderX32.cpp
            src/ElfObjectBuilderX64.cpp
            src/ElfObjectBuilderCreator.cpp
            ${PROJECT_SOURCE_DIR}/components/common/src/EndiannessUtility.cpp
            )

target_include_directories(ElfObjectBuilder PUBLIC
            include
            ${PROJECT_SOURCE_DIR}/components/common/include
            ${PROJECT_SOURCE_DIR}/components/common/include/structures
            )

target_link_libraries(ElfObjectBuilder PRIVATE Logger)
target_link_libraries(ElfObjectBuilder PRIVATE ElfStructureInfoBuilder)
