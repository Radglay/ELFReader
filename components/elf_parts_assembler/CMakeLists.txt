find_package(Qt6 REQUIRED COMPONENTS Core)

add_library(ElfPartAssembler
            src/ReadingFileContentUtility.cpp
            src/ElfPartAssembler.cpp
            src/FieldDescriptionUtility.cpp
            )

target_compile_options(ElfPartAssembler PUBLIC -fPIC)

target_include_directories(ElfPartAssembler PUBLIC
            include
            )

target_link_libraries(ElfPartAssembler PRIVATE Logger)
target_link_libraries(ElfPartAssembler PUBLIC ElfObjectBuilder)
target_link_libraries(ElfPartAssembler PUBLIC Qt6::Core)