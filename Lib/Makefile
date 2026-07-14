CC = gcc

CFLAGS = -Wall -Wextra -O2 -I. -Iinclude -Ilib
LDFLAGS = -lglfw -lvulkan -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi

OBJ = obj
BUILD = build
FILES = Files

SRCS = $(wildcard src/*.c) $(wildcard lib/*.c)
OBJS = $(patsubst %.c, $(OBJ)/%.o, $(notdir $(SRCS)))

vpath %.c src lib

APP_NAME = qgpuApp
APP = $(BUILD)/$(APP_NAME)

all: prepare $(APP)

prepare:
	@mkdir -p $(OBJ) $(BUILD) $(FILES) src include

$(OBJ)/%.o: %.c
	@echo "Compilation $<..."
	@$(CC) $(CFLAGS) -c $< -o $@

$(APP): $(OBJS)
	@echo "Linking app $(APP)..."
	@$(CC) -o $@ $(OBJS) $(LDFLAGS)

run: all
	@echo "--- Starting $(APP) ---"
	@cp -r $(FILES)/ $(BUILD)/ 2>/dev/null || true
	@cd $(BUILD) && ./$(APP_NAME)

clean:
	@echo "Cleaning..."
	@rm -rf $(OBJ) $(BUILD)
	@echo "Cleaned!"

.PHONY: all prepare run clean
