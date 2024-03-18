include common.mk
include src.mk
include test.mk

.DEFAULT_GOAL := debug_build

.PHONY: clean debug_build run test test_run


debug_build: create_debug_dir $(BIN_DIR)/main
	@$(SET_SUCCESS_LEVEL)
	@echo "Build finished"
	@$(SET_NORMAL_LEVEL)

run:
	$(BIN_DIR)/main

test: debug_build test_all $(BIN_DIR)/main_test
	@$(SET_SUCCESS_LEVEL)
	@echo "Tests finished"
	@$(SET_NORMAL_LEVEL)

test_all:
	@$(SET_START_LEVEL)
	@echo "Tests build..."
	@$(SET_NORMAL_LEVEL)

test_run:
	$(BIN_DIR)/main_test


clean:
	@$(SET_START_LEVEL)
	@echo "Cleaning..."
	@$(SET_NORMAL_LEVEL)
	@rm -rf $(BUILD_DIR) $(BIN_DIR)

