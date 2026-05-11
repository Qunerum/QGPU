CC = gcc
GLSLC = glslc
AR = ar

CFLAGS = -Wall -Wextra -O2 -Iinclude
LDFLAGS = -lglfw -lvulkan -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi

SRC_DIR = src
SHADERS_DIR = shaders
BIN = bin
BUILD = build

ALL_SRCS = $(wildcard $(SRC_DIR)/*.c)
APP_SRC = $(SRC_DIR)/main.c
ENGINE_SRCS = $(filter-out $(APP_SRC), $(ALL_SRCS))

ENGINE_OBJ = $(patsubst $(SRC_DIR)/%.c, $(BIN)/%.o, $(ENGINE_SRCS))
APP_OBJ = $(BIN)/main.o

LIB_TARGET = $(BUILD)/libqgpu.a
APP_TARGET = $(BUILD)/qgpu_app

VERT_SRC = $(SHADERS_DIR)/shader.vert
FRAG_SRC = $(SHADERS_DIR)/shader.frag
SHADERS_SPV = $(BIN)/vert.spv $(BIN)/frag.spv

all: prepare $(SHADERS_SPV) $(LIB_TARGET) $(APP_TARGET)

prepare:
	@mkdir -p $(BIN)
	@mkdir -p $(BUILD)

$(BIN)/%.spv: $(SHADERS_DIR)/shader.%
	@echo "Shader compilation $*..."
	@$(GLSLC) $< -o $@

$(BIN)/%.o: $(SRC_DIR)/%.c
	@echo "Compilation $<..."
	@$(CC) $(CFLAGS) -c $< -o $@

$(LIB_TARGET): $(ENGINE_OBJ)
	@echo "Creating static library $(LIB_TARGET)..."
	@$(AR) rcs $@ $(ENGINE_OBJ)

$(APP_TARGET): $(APP_OBJ) $(LIB_TARGET)
	@echo "Linking app $(APP_TARGET)..."
	@$(CC) -o $@ $(APP_OBJ) -L$(BUILD) -lqgpu $(LDFLAGS)

run: all
	@echo "--- Starting $(APP_TARGET) ---"
	@./$(APP_TARGET)

clean:
	@echo "Cleaning..."
	@rm -rf $(BIN) $(BUILD)

.PHONY: all prepare run clean
