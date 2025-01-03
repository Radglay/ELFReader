add_executable(main main.cpp)

target_link_libraries(main PRIVATE MainWindowUI)
target_link_libraries(main PRIVATE Logger)
target_link_libraries(main PRIVATE ElfObjectBuilder)
