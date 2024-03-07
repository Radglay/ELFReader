include common.mk


.DEFAULT_GOAL := debug_build

.PHONY := clean debug_build



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

