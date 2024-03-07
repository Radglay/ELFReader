include common.mk


.DEFAULT_GOAL := main_test


main_test: create_dir $(TEST_SOURCES) main_test_runner.o libgtest.a libgmock.a
	$(CC) $(BUILD_DIR)/main_test_runner.o \
	$(BUILD_DIR)/libgtest.a $(BUILD_DIR)/libgmock.a \
	-o main_test

    # -I $(GTEST_DIR)/include -I $(GMOCK_DIR)/include $(INCLUDES)

main_test_runner.o: $(TESTS_DIR)/main_test_runner.cpp
	$(CC) $(COMPILE_FLAG) $(TESTS_DIR)/main_test_runner.cpp -o \
	$(BUILD_DIR)/main_test_runner.o -I$(GTEST_DIR)/include

libgtest.a: gtest-all.o
	ar -rv $(BUILD_DIR)/libgtest.a $(BUILD_DIR)/gtest-all.o

libgmock.a: gmock-all.o
	ar -rv $(BUILD_DIR)/libgmock.a $(BUILD_DIR)/gmock-all.o

gtest-all.o: $(GTEST_DIR)/src/gtest-all.cc
	$(CC) $(COMPILE_FLAG) -I $(GTEST_DIR)/include -I $(GTEST_DIR) \
	-pthread $(GTEST_DIR)/src/gtest-all.cc -o $(BUILD_DIR)/gtest-all.o

gmock-all.o: $(GMOCK_DIR)/src/gmock-all.cc
	$(CC) $(COMPILE_FLAG) -I $(GMOCK_DIR)/include -I $(GMOCK_DIR) -I $(GTEST_DIR)/include \
	-pthread $(GMOCK_DIR)/src/gmock-all.cc -o $(BUILD_DIR)/gmock-all.o
