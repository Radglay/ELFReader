find_package(Qt6 REQUIRED COMPONENTS Widgets)

set(CMAKE_AUTOUIC_SEARCH_PATHS form)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

qt_add_library(MainWindowUI
    form/mainwindow.ui
    src/mainwindow.cpp
    include/mainwindow.h)

target_include_directories(MainWindowUI PUBLIC
    include)

target_link_libraries(MainWindowUI Qt6::Widgets)
