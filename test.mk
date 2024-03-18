
$(BIN_DIR)/main_test: create_debug_dir $(TEST_OBJECTS) $(BUILD_DIR)/main_test_runner.o \
		              $(BUILD_DIR)/libgtest.a $(BUILD_DIR)/libgmock.a \
					  $(OBJECTS) create_bin_dir
	@echo $(BIN_DIR)/main_test
	@$(CC) $(BUILD_DIR)/main_test_runner.o \
	$(BUILD_DIR)/libgtest.a $(BUILD_DIR)/libgmock.a \
	$(TEST_OBJECTS) $(OBJECTS) -o $(BIN_DIR)/main_test

$(BUILD_DIR)/%.o: $(TEST_DIR)/%.cpp
	@echo $@
	@$(CC) $(COMPILE_FLAG) $(DEBUG_FLAG) $< -o $@ $(INCLUDES)

$(BUILD_DIR)/main_test_runner.o: $(TESTS_DIR)/main_test_runner.cpp
	@echo $(BUILD_DIR)/main_test_runner.o
	@$(CC) $(COMPILE_FLAG) $(TESTS_DIR)/main_test_runner.cpp -o \
	$(BUILD_DIR)/main_test_runner.o $(INCLUDES)

$(BUILD_DIR)/libgtest.a: $(BUILD_DIR)/gtest-all.o
	@echo $(BUILD_DIR)/libgtest.a
	@ar -rv $(BUILD_DIR)/libgtest.a $(BUILD_DIR)/gtest-all.o

$(BUILD_DIR)/libgmock.a: $(BUILD_DIR)/gmock-all.o
	@echo $(BUILD_DIR)/libgmock.a
	@ar -rv $(BUILD_DIR)/libgmock.a $(BUILD_DIR)/gmock-all.o

$(BUILD_DIR)/gtest-all.o: $(GTEST_DIR)/src/gtest-all.cc
	@echo $(BUILD_DIR)/gtest-all.o
	@$(CC) $(COMPILE_FLAG) -pthread $(GTEST_DIR)/src/gtest-all.cc -o $(BUILD_DIR)/gtest-all.o $(INCLUDES)

$(BUILD_DIR)/gmock-all.o: $(GMOCK_DIR)/src/gmock-all.cc
	@echo $(BUILD_DIR)/gmock-all.o
	@$(CC) $(COMPILE_FLAG) -pthread $(GMOCK_DIR)/src/gmock-all.cc -o $(BUILD_DIR)/gmock-all.o $(INCLUDES)
