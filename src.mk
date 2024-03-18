
$(BIN_DIR)/main: $(BUILD_DIR)/main.o $(OBJECTS) create_bin_dir
	@echo $(BIN_DIR)/main
	@$(CC) $(OBJECTS) $(BUILD_DIR)/main.o -o $(BIN_DIR)/main

$(BUILD_DIR)/main.o: $(MAIN_DIR)/main.cpp
	@echo $(BUILD_DIR)/main.o
	@$(CC) $(COMPILE_FLAG) $(DEBUG_FLAG) $(MAIN_DIR)/main.cpp -o $(BUILD_DIR)/main.o $(INCLUDES)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@echo $@
	@$(CC) $(COMPILE_FLAG) $(DEBUG_FLAG) $< -o $@ $(INCLUDES)
