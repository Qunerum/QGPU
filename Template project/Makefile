# - - - CONFIGURATION - - -
TARGET_NAME = my_project
# - - -

# Compilators
CC_LINUX = gcc
# CC_WIN = i686-w64-mingw32-gcc # 32 bit
CC_WIN = x86_64-w64-mingw32-gcc # 64 bit


CFLAGS = -Wall -Wextra -O2 -Iinclude
LDFLAGS_LINUX = -Llibs/linux -lqgpu -lglfw -lvulkan -ldl -lpthread -lX11
LDFLAGS_WIN = -Llibs/win -lqgpu -lglfw3 -lvulkan-1 -lgdi32 -luser32 -lkernel32 -lm

SRC_DIR = src
BIN_DIR = bin
BUILD_DIR = build

ifeq ($(OS),Windows_NT)
    PLATFORM = win
    EXT = .exe
else
    PLATFORM = linux
    EXT =
endif

SRC = $(wildcard $(SRC_DIR)/*.c)

all: linux win

linux:
	@$(MAKE) build_platform PLATFORM=linux TARGET_CC=$(CC_LINUX) LDFLAGS="$(LDFLAGS_LINUX)" EXT=""

win:
	@$(MAKE) build_platform PLATFORM=win TARGET_CC=$(CC_WIN) LDFLAGS="$(LDFLAGS_WIN)" EXT=".exe"

build_platform: prepare
	@echo "--- Building for $(PLATFORM) ---"
	@$(MAKE) compile_objs PLATFORM=$(PLATFORM) TARGET_CC=$(TARGET_CC)
	@$(TARGET_CC) $(BIN_DIR)/$(PLATFORM)/*.o -o $(BUILD_DIR)/$(PLATFORM)/$(TARGET_NAME)$(EXT) $(LDFLAGS)
	@echo "Done: $(BUILD_DIR)/$(PLATFORM)/$(TARGET_NAME)$(EXT)"

compile_objs: $(SRC:$(SRC_DIR)/%.c=$(BIN_DIR)/$(PLATFORM)/%.o)

$(BIN_DIR)/$(PLATFORM)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	@echo "Compiling $(PLATFORM) [$<]..."
	@$(TARGET_CC) $(CFLAGS) -c $< -o $@

prepare:
	@mkdir -p $(BIN_DIR)/linux $(BIN_DIR)/win
	@mkdir -p $(BUILD_DIR)/linux $(BUILD_DIR)/win

run: linux
	@echo "--- Starting Linux Version ---"
	@./$(BUILD_DIR)/linux/$(TARGET_NAME)

clean:
	@echo "Cleaning..."
	@rm -rf $(BIN_DIR) $(BUILD_DIR)

.PHONY: all linux win run clean build_platform prepare compile_objs
