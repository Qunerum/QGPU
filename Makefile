CC = gcc
GLSLC = glslc

CFLAGS = -Wall -Wextra -O2 -Iinclude
LDFLAGS = -lglfw -lvulkan -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi -lm

SRC_DIR = src
SHADERS_DIR = shaders
BIN_DIR = bin

SRC = $(SRC_DIR)/main.c $(SRC_DIR)/qgpu.c $(SRC_DIR)/qgpu_core.c
OBJ = $(SRC:$(SRC_DIR)/%.c=$(BIN_DIR)/%.o)
TARGET = $(BIN_DIR)/qgpu_app

VERT_SRC = $(SHADERS_DIR)/shader.vert
FRAG_SRC = $(SHADERS_DIR)/shader.frag
SHADERS_SPV = $(BIN_DIR)/vert.spv $(BIN_DIR)/frag.spv

all: $(BIN_DIR) $(SHADERS_SPV) $(TARGET)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(BIN_DIR)/vert.spv: $(VERT_SRC)
	@echo "Kompilacja shadera vertex..."
	$(GLSLC) $< -o $@

$(BIN_DIR)/frag.spv: $(FRAG_SRC)
	@echo "Kompilacja shadera fragment..."
	$(GLSLC) $< -o $@

$(BIN_DIR)/%.o: $(SRC_DIR)/%.c
	@echo "Kompilacja $<..."
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJ)
	@echo "Linkowanie projektu..."
	$(CC) -o $@ $(OBJ) $(LDFLAGS)

run: all
	@echo "--- Odpalam qgpu_app ---"
	./$(TARGET)

clean:
	rm -rf $(BIN_DIR)

.PHONY: all run clean
