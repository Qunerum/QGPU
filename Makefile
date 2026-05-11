CC = gcc
GLSLC = glslc

CFLAGS = -Wall -Wextra -O2 -Iinclude
LDFLAGS = -lglfw -lvulkan -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi -lm

SRC_DIR = src
SHADERS_DIR = shaders
BIN = bin

OBJ = $(BIN)/main.o \
      $(BIN)/qgpu.o \
      $(BIN)/qgpu_core.o \
      $(BIN)/qgpu_texture.o \
      $(BIN)/qgpu_ui.o

TARGET = $(BIN)/qgpu_app

VERT_SRC = $(SHADERS_DIR)/shader.vert
FRAG_SRC = $(SHADERS_DIR)/shader.frag
SHADERS_SPV = $(BIN)/vert.spv $(BIN)/frag.spv

all: prepare $(SHADERS_SPV) $(TARGET)

prepare:
	@mkdir -p $(BIN)
	@mkdir -p $(BIN)

$(BIN)/%.spv: $(SHADERS_DIR)/shader.%
	@echo "Kompilacja shadera $*..."
	@$(GLSLC) $< -o $@

$(BIN)/%.o: $(SRC_DIR)/%.c
	@echo "Kompilacja $<..."
	@$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJ)
	@echo "Linkowanie projektu..."
	@$(CC) -o $@ $(OBJ) $(LDFLAGS)

run: all
	@echo "--- Odpalam qgpu_app ---"
	@./$(TARGET)

clean:
	@rm -rf $(BIN)

.PHONY: all prepare run clean
