##
## api - Makefile
## Driver commands for both the 'client' and 'server' packages
##

##
## Directories
##

VENDOR_DIR := ./vendor
BUILD_DIR := ./build
SERVER_DIR := ./server
CLIENT_DIR := ./client

##
## OS-Specifics
##

PLATFORM = linux
SHARED_LIB_EXT = so
ifeq ($(OS), Windows_NT)
	PLATFORM = win32
	SHARED_LIB_EXT = dll
endif

.PHONY: vendor server client clean

vendor:
	$(MAKE) -C $(VENDOR_DIR) check 

server:
	$(MAKE) -C $(SERVER_DIR) $(SHARED_LIB_EXT) $(PLATFORM)

client:
	$(MAKE) -C $(CLIENT_DIR) $(PLATFORM) tests

clean:
	$(MAKE) -C $(SERVER_DIR) $@ 
	$(MAKE) -C $(CLIENT_DIR) $@
	$(MAKE) -C $(VENDOR_DIR) $@