add_library(Logger OBJECT src/LoggerSettingUtility.cpp)

target_include_directories(Logger PUBLIC
                           ${PROJECT_SOURCE_DIR}/lib/plog/include)
