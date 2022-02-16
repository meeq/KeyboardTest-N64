all: keyboardtest.z64
.PHONY: all

BUILD_DIR = build
include $(N64_INST)/include/n64.mk

OBJS = $(BUILD_DIR)/keyboardtest.o
DEPS := $(OBJS:.o=.d)

keyboardtest.z64: N64_ROM_TITLE = "Keyboard Test"

$(BUILD_DIR)/keyboardtest.elf: $(OBJS)

clean:
	rm -rf $(BUILD_DIR) *.z64
.PHONY: clean

-include $(DEPS)
