add_library(ElfSectionBuilder
            src/ElfSectionBuilder.cpp
            src/ElfBuildDirector.cpp
            ${PROJECT_SOURCE_DIR}/components/common/src/EndiannessUtility.cpp
            src/NoteSectionBuildingUtility.cpp
            )

target_include_directories(ElfSectionBuilder PUBLIC
            include
            include/sections
            include/sections/note_descriptors
            ${PROJECT_SOURCE_DIR}/components/common/include
            ${PROJECT_SOURCE_DIR}/components/common/include/structures
            )

target_link_libraries(ElfSectionBuilder PRIVATE Logger)
target_link_libraries(ElfSectionBuilder PUBLIC ElfStructureInfoBuilder)
target_link_libraries(ElfSectionBuilder PUBLIC ElfPartAssembler)
