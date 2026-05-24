CC = gcc
GLSLC = glslc

CFLAGS = -Wall -Wextra -O2 -I.
LDFLAGS = -lglfw -lvulkan -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi

SHADERS_DIR = shaders
BIN = bin
BUILD = build

# Znajduje automatycznie qgpu.c, main.c i ewentualne inne pliki .c w tym folderze
SRCS = $(wildcard *.c)
# Mapuje pliki .c na obiekty .o wewnątrz folderu bin/
OBJS = $(patsubst %.c, $(BIN)/%.o, $(SRCS))

APP_TARGET = $(BUILD)/qgpu_app

VERT_SRC = $(SHADERS_DIR)/shader.vert
FRAG_SRC = $(SHADERS_DIR)/shader.frag
SHADERS_SPV = $(BIN)/vert.spv $(BIN)/frag.spv

all: prepare $(SHADERS_SPV) $(APP_TARGET)

prepare:
	@mkdir -p $(BIN)
	@mkdir -p $(BUILD)

# Reguła dla shaderów
$(BIN)/%.spv: $(SHADERS_DIR)/shader.%
	@echo "Shader compilation $*..."
	@$(GLSLC) $< -o $@

# Uniwersalna reguła kompilacji plików .c z głównego folderu do bin/%.o
$(BIN)/%.o: %.c
	@echo "Compilation $<..."
	@$(CC) $(CFLAGS) -c $< -o $@

# Linkowanie aplikacji bezpośrednio ze wszystkich obiektów .o (bez libqgpu.a)
$(APP_TARGET): $(OBJS)
	@echo "Linking app $(APP_TARGET)..."
	@$(CC) -o $@ $(OBJS) $(LDFLAGS)

run: all
	@echo "--- Starting $(APP_TARGET) ---"
	@./$(APP_TARGET)

clean:
	@echo "Cleaning..."
	@rm -rf $(BIN) $(BUILD)

.PHONY: all prepare run clean
