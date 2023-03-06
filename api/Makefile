##
## win32/cyginw/mingw64
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
WRAP_DIR := ./wrap
BUILD_DIR := ./build
INCLUDE_DIR := ./include
SURF_INTERP_DIR := ../surf

##
## Files
##

DUMMY_FILE_NAME := runtime
DUMMY_ML_FILE := $(OBJ_DIR)/$(DUMMY_FILE_NAME).ml
DUMMY_OBJ_FILE := $(OBJ_DIR)/$(DUMMY_FILE_NAME).o

WRAP_FILE_NAME := wrap
WRAP_C_FILE := $(WRAP_DIR)/$(WRAP_FILE_NAME).c
WRAP_OBJ_FILE := $(OBJ_DIR)/$(WRAP_FILE_NAME).o

##
## Out
##

EXE := $(BUILD_DIR)/surf.exe
SURF_CAML_LIB := $(BUILD_DIR)/surf.a
SURF_WRAP_LIB := $(BUILD_DIR)/libwrap.a
SURF_CAML_COMPILED_LIB := $(SURF_INTERP_DIR)/_build/default/lib/surf.a

##
## Compilation
##

OCAMLC := ocamlc
OCAMLCFLAGS := -custom -output-complete-obj

FLEXLINK := flexlink
FLEXLINKFLAGS := -chain mingw64 -exe -o $(EXE) $(SURF_CAML_LIB) $(SURF_WRAP_LIB) -lversion -lws2_32

CC := gcc
CFLAGS := -I$(INCLUDE_DIR) -I$(WRAP_DIR) -I`$(OCAMLC) -where`

##
## Source
##

SRCS := $(wildcard $(SRC_DIR)/*.c)
OBJS := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))

.PHONY: ml unix win32 clean

# Build the interpreter and copy the static lib created by dune
ml:
	$(MAKE) -C ../surf build
	cp $(SURF_CAML_COMPILED_LIB) $(SURF_CAML_LIB)
	$(MAKE) ml-int

# Hack: inject the OCaml runtime by compiling a single ocaml file
# Compile the OCaml -> C wrapper 
ml-int:
	echo "(** $(DUMMY_FILE_NAME) *)" >> $(DUMMY_ML_FILE)
	$(OCAMLC) $(OCAMLCFLAGS) -o $(DUMMY_OBJ_FILE) $(DUMMY_ML_FILE)
	$(OCAMLC) -c $(WRAP_C_FILE) -o $(WRAP_OBJ_FILE)
	ar r $(SURF_WRAP_LIB) $(WRAP_OBJ_FILE) $(DUMMY_OBJ_FILE)

unix: $(OBJS) | $(BUILD_DIR)
	$(MAKE) ml
	$(CC) $(CFLAGS) $^ -Lbuild -lsurf -ldl

win32: $(OBJS) | $(BUILD_DIR)
	$(MAKE) ml
	$(FLEXLINK) $(FLEXLINKFLAGS) $^ 

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR) $(OBJ_DIR):
	$(MKDIR) $@

clean:
	$(RIMRAF) $(BUILD_DIR)/*.o
	$(RIMRAF) $(BUILD_DIR) $(OBJ_DIR)
	$(RIMRAF) $(WRAP_DIR)/*.cm*

##
## Commands/Ideas for trying to convert Mingw DLLs tio MSVC
## dlltool -z build/dllapi.def build/dllapi.dll --export-all-symbols  ## creates def file for dll (atexit redef)
## lib /def:build\dllapi.def /out:build\dllapi.lib /machine:x64       ## creates an MSVC .lib
##