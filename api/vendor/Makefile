##
## api/vendor - Makefile
## Compiles the api's vendor/third-party code
##

##
## Utilities
##

MKDIR := mkdir -p
RIMRAF := rm -rf

##
## Directories
##

BUILD_DIR := ../build/vendor
CHECK_OUT_DIR := $(BUILD_DIR)/check

CMAKE := cmake

.PHONY: check clean

check: $(BUILD_DIR) $(CHECK_OUT_DIR)
	$(CMAKE) -B$(CHECK_OUT_DIR) -Scheck/ -G"Unix Makefiles"
	$(MAKE) -C $(CHECK_OUT_DIR)

$(BUILD_DIR) $(CHECK_OUT_DIR):
	$(MKDIR) $@

clean: 
	$(RIMRAF) $(BUILD_DIR)
