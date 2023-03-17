##
## api/server - Makefile
## Compiles the api's server source and builds a shared library for the surf interpreter 
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
STUBS_DIR := ./stubs
MISC_DIR := ./misc
BUILD_DIR := ../build/server
INCLUDE_DIR := ./include
SURF_INTERP_DIR := ../../surf

##
## Out
##

WIN_EXE := $(BUILD_DIR)/winsurf.exe
SURF_CAML_SHARED_LIB = $(BUILD_DIR)/libsurf.so
SHARED_LIB_EXT = so

##
## OS-Specifics
##

PLATFORM = linux
ifeq ($(OS), Windows_NT)
	PLATFORM = WIN32
	SURF_CAML_SHARED_LIB = $(BUILD_DIR)/libsurf.dll
	SHARED_LIB_EXT = dll
endif

##
## Compilation
##

OCAMLC := ocamlc
OCAMLFINDOPT := ocamlfind opt

# NOTE: surf HAS to be installed locally for codegen to work
PACKAGES=surf,ctypes.stubs
OCAMLFINDOPTFLAGS := -output-obj -package $(PACKAGES) -thread -linkpkg -cclib -implib -o ../$(SURF_CAML_SHARED_LIB)

# OCaml requires 'flexlink' to compile dll's on Windows
FLEXLINK := flexlink
FLEXLINKFLAGS := -chain mingw64 -exe -o $(WIN_EXE) -lversion -lws2_32 -L$(BUILD_DIR) -lsurf

CC := gcc
CFLAGS := -I$(INCLUDE_DIR) -I$(STUBS_DIR) -I`$(OCAMLC) -where` -I`$(OCAMLC) -where`/../ctypes -D$(PLATFORM)

##
## Source
##

SRCS := $(wildcard $(SRC_DIR)/*.c)
OBJS := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))

.PHONY: win32 clean

# Build the interpreter and use the 'gen' package to create c-stubs for the interpreter
# This could probably be simplified with make magic...
$(SHARED_LIB_EXT): $(OBJ_DIR) $(BUILD_DIR)
	$(MAKE) -C $(SURF_INTERP_DIR) prep-api
	$(CC) $(CFLAGS) -c misc/Init.c -o obj/Init.o
	$(CC) $(CFLAGS) -c stubs/gen.c -o obj/gen.o
	$(OCAMLFINDOPT) -c -o stubs/gen_bindings.cmx -package $(PACKAGES) stubs/gen_bindings.ml -thread
	$(OCAMLFINDOPT) -c -o stubs/bindings.cmx -package $(PACKAGES) stubs/bindings.ml -thread
	cd stubs && $(OCAMLFINDOPT) $(OCAMLFINDOPTFLAGS) gen_bindings.ml bindings.cmx link.ml ../obj/Init.o ../obj/gen.o    

run:
	$(WIN_EXE)

win32: $(OBJS) | $(BUILD_DIR)
	$(FLEXLINK) $(FLEXLINKFLAGS) $^ 

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR) $(OBJ_DIR):
	$(MKDIR) $@

clean:
	$(RIMRAF) $(STUBS_DIR) $(BUILD_DIR) $(OBJ_DIR)

##
## Commands/Ideas for trying to convert Mingw DLLs tio MSVC
## dlltool -z build/dllapi.def build/dllapi.dll --export-all-symbols  ## creates def file for dll (atexit redef)
## lib /def:build\dllapi.def /out:build\dllapi.lib /machine:x64       ## creates an MSVC .lib
##