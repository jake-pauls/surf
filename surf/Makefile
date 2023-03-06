##
## surf - Makefile
## Driver for dune/opam project
##

##
## Utilities
##

RIMRAF := rm -rf

##
## Variables
##

# Stubs can be directly placed in the 'api' directory
STUBS_DIR := ../api/stubs

STUBS_ML_FILE := gen_bindings.ml
STUBS_C_FILE := gen.c
STUBS_H_FILE := gen.h

.PHONY: build build install surf gen test clean fmt

# Required to 'prep' the API
# Ideally, this is cleaned up at some point or merged into dune as a buildsystem
prep-api:
	$(MAKE) install
	$(MAKE) gen
	# Create file to link stubs to generated bindings
	echo "include Bindings.Stubs(Gen_bindings)" > $(STUBS_DIR)/link.ml
	# Copy file with bindings module over to api
	cp gen/lib/bindings.ml $(STUBS_DIR)

build: 
	dune build gen
	dune build surf

# Surf needs to be installed as a local package to be found with `ocamlfind`
install:
	dune install surf

surf:
	dune exec surf

# 'gen' is a cli executable that takes in files/directories for the stubs to output
gen:
	dune exec gen $(STUBS_DIR) $(STUBS_ML_FILE) $(STUBS_C_FILE) $(STUBS_H_FILE)

test:
	dune runtest -f

clean:
	dune clean
	$(RIMRAF) $(STUBS_DIR)

fmt:
	dune fmt 
