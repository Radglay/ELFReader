add_library(ElfObjectBuilder
            src/ElfObjectBuilder.cpp
            src/ElfBuildDirector.cpp
            ${PROJECT_SOURCE_DIR}/components/common/src/EndiannessUtility.cpp
            )

target_include_directories(ElfObjectBuilder PUBLIC
            include
            include/sections
            ${PROJECT_SOURCE_DIR}/components/common/include
            ${PROJECT_SOURCE_DIR}/components/common/include/structures
            )

target_link_libraries(ElfObjectBuilder PRIVATE Logger)
target_link_libraries(ElfObjectBuilder PUBLIC ElfStructureInfoBuilder)
target_link_libraries(ElfObjectBuilder PUBLIC ElfPartAssembler)
