add_executable(main main.cpp)

target_link_libraries(main PRIVATE ElfFileParser)
target_link_libraries(main PRIVATE Logger)
