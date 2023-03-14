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
OBJ_DIR := ./obj
BUILD_DIR := ../build/client
INCLUDE_DIR := ./include
SURF_INTERP_DIR := ../../surf

##
## Out
##

STATIC_LIB := $(BUILD_DIR)/libsurf.a

##
## OS-Specifics
##

PLATFORM = linux
ifeq ($(OS), Windows_NT)
	PLATFORM = WIN32
endif

##
## Compiler
##

CC := gcc
CFLAGS := -I$(INCLUDE_DIR) -D$(PLATFORM)
AR := ar
ARFLAGS := rcs $(STATIC_LIB)

##
## Source
##

SRCS := $(wildcard $(SRC_DIR)/*.c)
OBJS := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))

.PHONY: win32 clean

win32: $(OBJS) | $(BUILD_DIR)
	$(AR) $(ARFLAGS) $^ 

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR) $(OBJ_DIR):
	$(MKDIR) $@

clean:
	$(RIMRAF) $(BUILD_DIR) $(OBJ_DIR)