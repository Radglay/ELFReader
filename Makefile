.DEFAULT_GOAL := debug_build

.PHONY := clean debug_build


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

SRC_DIR = src
MAIN_DIR = $(SRC_DIR)


# files
INCLUDES := -I include\
			-I include/structures\
			-I include/utility

SOURCES = $(SRC_DIR)/ElfFileParserX32.cpp\
		  $(SRC_DIR)/ElfFileParserX64.cpp\
		  $(SRC_DIR)/FileHeaderParserX32.cpp\
          $(SRC_DIR)/FileHeaderParserX64.cpp\
		  $(SRC_DIR)/CheckingFileVersionUtility.cpp\
		  $(SRC_DIR)/ElfFileParserCreator.cpp\
		  $(SRC_DIR)/ProgramHeaderParserX32.cpp\
		  $(SRC_DIR)/ProgramHeaderParserX64.cpp

OBJECTS = $(BUILD_DIR)/ElfFileParserX32.o\
	      $(BUILD_DIR)/ElfFileParserX64.o\
		  $(BUILD_DIR)/FileHeaderParserX32.o\
		  $(BUILD_DIR)/FileHeaderParserX64.o\
		  $(BUILD_DIR)/CheckingFileVersionUtility.o\
		  $(BUILD_DIR)/ElfFileParserCreator.o\
		  $(BUILD_DIR)/ProgramHeaderParserX32.o\
		  $(BUILD_DIR)/ProgramHeaderParserX64.o

create_dir: 
	@$(SET_START_LEVEL)
	@echo "Debug build..."
	@$(SET_NORMAL_LEVEL)
	@mkdir -p $(BUILD_DIR)

debug_build: create_dir main
	@$(SET_SUCCESS_LEVEL)
	@echo "Build finished"
	
main: main.o $(OBJECTS)
	@$(CC) $(OBJECTS) $(BUILD_DIR)/main.o -o main

main.o: $(MAIN_DIR)/main.cpp
	@$(CC) $(COMPILE_FLAG) $(DEBUG_FLAG) $(MAIN_DIR)/main.cpp -o $(BUILD_DIR)/main.o $(INCLUDES)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@echo $@
	$(CC) $(COMPILE_FLAG) $(DEBUG_FLAG) $< -o $@ $(INCLUDES)

clean:
	@$(SET_START_LEVEL)
	@echo "Cleaning build..."
	@$(SET_NORMAL_LEVEL)
	@rm -rf ./build

