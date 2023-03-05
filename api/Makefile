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
SURF_LIB_DIR := ../surf/lib

##
## Out
##

EXE := $(BUILD_DIR)/surf.exe
SURF_CAML := $(OBJ_DIR)/modsurf.o
SURF_CAML_LIB := $(BUILD_DIR)/libsurf.a

##
## Compilation
##

OCAMLC := ocamlc
OCAMLCFLAGS := -custom -output-complete-obj -o $(SURF_CAML)

FLEXLINK := flexlink
FLEXLINKFLAGS := -chain mingw64 -exe -o $(EXE) $(SURF_CAML_LIB) -lversion -lws2_32

CC := gcc
CFLAGS := -I$(INCLUDE_DIR) -I$(WRAP_DIR) -I`$(OCAMLC) -where`

##
## Source
##

SRCS := $(wildcard $(SRC_DIR)/*.c)
OBJS := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))

.PHONY: ml win32 clean

# TODO: Glob .ml files that have wrapper methods

ml:
	$(OCAMLC) $(OCAMLCFLAGS) $(SURF_LIB_DIR)/base.ml 
	$(CC) $(CFLAGS) -c $(WRAP_DIR)/wrap.c -o obj/wrap.o
	ar r $(SURF_CAML_LIB) $(SURF_CAML) obj/wrap.o

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
	$(RIMRAF) $(SURF_LIB_DIR)/*.cm*

##
## Commands/Ideas for trying to convert Mingw DLLs tio MSVC
## dlltool -z build/dllapi.def build/dllapi.dll --export-all-symbols  ## creates def file for dll (atexit redef)
## lib /def:build\dllapi.def /out:build\dllapi.lib /machine:x64       ## creates an MSVC .lib
##