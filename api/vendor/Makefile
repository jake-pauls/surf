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
MT_OUT_DIR := $(BUILD_DIR)/minitrace

CMAKE := cmake

.PHONY: check mt clean

check: $(BUILD_DIR) $(CHECK_OUT_DIR)
	$(CMAKE) -B$(CHECK_OUT_DIR) -Scheck/ -G"Unix Makefiles"
	$(MAKE) -C $(CHECK_OUT_DIR)

mt: $(BUILD_DIR) $(MT_OUT_DIR)
	$(CMAKE) -B$(MT_OUT_DIR) -Sminitrace/ -G"Unix Makefiles"
	$(MAKE) -C $(MT_OUT_DIR)

$(BUILD_DIR) $(CHECK_OUT_DIR) $(MT_OUT_DIR):
	$(MKDIR) $@

clean: 
	$(RIMRAF) $(BUILD_DIR)
