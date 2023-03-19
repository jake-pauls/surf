##
## api/client - Makefile
## Commands for compiling the client API as a static library
##

##
## Utilities
##

RIMRAF := rm -rf
MKDIR := mkdir -p

##
## Directories
##

SRC_DIR := ./src
TEST_DIR := ./tests
UNIT_TEST_DIR := $(TEST_DIR)/Unit
OBJ_DIR := ./obj
SRC_OBJ_DIR := $(OBJ_DIR)/src
TEST_OBJ_DIR := $(OBJ_DIR)/tests
BUILD_DIR := ../build/client
TEST_OUT_DIR := $(BUILD_DIR)/tests
INCLUDE_DIR := ./include
SURF_INTERP_DIR := ../../surf

VENDOR_BUILD_DIR := ../build/vendor
CHECK_BUILD_DIR := $(VENDOR_BUILD_DIR)/check
CHECK_INCLUDE_DIR := $(CHECK_BUILD_DIR)/src

##
## Out
##

STATIC_LIB := $(BUILD_DIR)/libsurf.a
TEST_EXE := $(BUILD_DIR)/tests.exe

##
## OS-Specifics
##

PLATFORM = linux
SHARED_LIB_EXT = .so
ifeq ($(OS), Windows_NT)
	PLATFORM = WIN32
	SHARED_LIB_EXT = .dll
endif

##
## Compiler
##

CC := gcc
CFLAGS := -I$(INCLUDE_DIR) -D$(PLATFORM)

CHECK_INCLUDES := -I$(CHECK_BUILD_DIR) -I$(CHECK_INCLUDE_DIR)
CHECK_LINKS := -L$(CHECK_INCLUDE_DIR) -lcheck -pthread -L$(BUILD_DIR) -lsurf -lws2_32

AR := ar
ARFLAGS := rcs $(STATIC_LIB)

##
## Source
##

SRCS := $(wildcard $(SRC_DIR)/*.c)
TEST_SRCS := $(wildcard $(TEST_DIR)/*.c)
UNIT_TEST_SRCS := $(wildcard $(UNIT_TEST_DIR)/*.c)
OBJS := $(patsubst $(SRC_DIR)/%.c,$(SRC_OBJ_DIR)/%.o,$(SRCS))
TEST_OBJS := $(patsubst $(TEST_DIR)/%.c,$(TEST_OBJ_DIR)/%.o, $(TEST_SRCS))
UNIT_TEST_OBJS := $(patsubst $(UNIT_TEST_DIR)/%.c, $(TEST_OBJ_DIR)/%.o, $(UNIT_TEST_SRCS))

.PHONY: win32 tests run-tests clean

win32: $(OBJS) | $(BUILD_DIR)
	$(AR) $(ARFLAGS) $^ 

tests: $(TEST_EXE)
	cp $(CHECK_INCLUDE_DIR)/libcheck$(SHARED_LIB_EXT) $(BUILD_DIR)
	$(TEST_EXE)

$(TEST_EXE): $(TEST_OBJS) $(UNIT_TEST_OBJS) | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(CHECK_INCLUDES) $^ -o $(TEST_EXE) $(CHECK_LINKS)

$(SRC_OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR) $(SRC_OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(TEST_OBJ_DIR)/%.o: $(TEST_DIR)/%.c | $(OBJ_DIR) $(TEST_OBJ_DIR)
	$(CC) $(CFLAGS) $(CHECK_INCLUDES) -c $< -o $@

$(TEST_OBJ_DIR)/%.o: $(UNIT_TEST_DIR)/%.c | $(OBJ_DIR) $(TEST_OBJ_DIR)
	$(CC) $(CFLAGS) $(CHECK_INCLUDES) -c $< -o $@

$(BUILD_DIR) $(OBJ_DIR) $(SRC_OBJ_DIR) $(TEST_OBJ_DIR):
	$(MKDIR) $@

clean:
	$(RIMRAF) $(BUILD_DIR) $(OBJ_DIR)