add_executable(main main.cpp)

target_link_libraries(main PRIVATE ElfObjectBuilder)
target_link_libraries(main PRIVATE Logger)
