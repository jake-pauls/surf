##
## api - Makefile
## Driver commands for both the 'client' and 'server' packages
##

##
## Directories
##

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

.PHONY: server client clean

server:
	$(MAKE) -C $(SERVER_DIR) $(SHARED_LIB_EXT)
	$(MAKE) -C $(SERVER_DIR) $(PLATFORM)

client:
	$(MAKE) -C $(CLIENT_DIR) $(PLATFORM)

clean:
	$(MAKE) -C $(SERVER_DIR) $@ 
	$(MAKE) -C $(CLIENT_DIR) $@