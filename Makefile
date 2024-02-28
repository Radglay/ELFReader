.DEFAULT_GOAL := debug_build

# log levels
SET_START_LEVEL = echo -n "\033[0;33m"
SET_NORMAL_LEVEL = echo -n "\033[0;97m"
SET_ERROR_LEVEL = echo -n "\033[0;31m"
SET_SUCCESS_LEVEL = echo -n "\033[0;32m"

# build flags
CC = g++
DEBUG_FLAG = -g
COMPILE_FLAG = -c

# directories
BUILD_DIR = build
INCLUDE_DIR = include
SRC_DIR = src
MAIN_DIR = $(SRC_DIR)


# files
INCLUDES := -I $(INCLUDE_DIR)

SOURCES = $(SRC_DIR)/fileHeaderParser.cpp

OBJECTS = fileHeaderParser.o


create_dir: 
	@$(SET_START_LEVEL)
	@echo "Debug build..."
	@$(SET_NORMAL_LEVEL)
	@mkdir -p $(BUILD_DIR)

debug_build: create_dir main
	@$(SET_SUCCESS_LEVEL)
	@echo "Build finished"
	
main: main.o $(OBJECTS)
	@$(CC) $(BUILD_DIR)/$(OBJECTS) $(BUILD_DIR)/main.o -o main

main.o: $(MAIN_DIR)/main.cpp
	@$(CC) $(COMPILE_FLAG) $(DEBUG_FLAG) $(MAIN_DIR)/main.cpp -o $(BUILD_DIR)/main.o $(INCLUDES)

%.o: $(SRC_DIR)/%.cpp
	@echo $@
	@$(CC) $(COMPILE_FLAG) $(DEBUG_FLAG) $< -o $(BUILD_DIR)/$@ $(INCLUDES)

clean:
	@$(SET_START_LEVEL)
	@echo "Cleaning build..."
	@$(SET_NORMAL_LEVEL)
	@rm -rf ./build

