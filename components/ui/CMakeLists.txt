find_package(Qt6 REQUIRED COMPONENTS Widgets)

set(CMAKE_AUTOUIC_SEARCH_PATHS form)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_library(MainWindowUI
    form/mainwindow.ui
    src/mainwindow.cpp
    include/mainwindow.h
    src/ObjectCodeContent.cpp
    include/ObjectCodeContent.h
    src/NavigationPanel.cpp
    include/NavigationPanel.h
    include/StructureDetails.h
    src/StructureDetails.cpp)

target_include_directories(MainWindowUI PUBLIC
    include)

target_link_libraries(MainWindowUI Qt6::Widgets)
target_link_libraries(MainWindowUI ElfPartAssembler)
target_link_libraries(MainWindowUI ElfObjectBuilder)
