CC = gcc
CFLAGS = -Wall -Wextra -g -O0 -I. -Iinclude -Ilib

ifeq ($(OS),Windows_NT)
    LDFLAGS = -lglfw3 -lvulkan -lgdi32 -luser32 -lshell32
    APP_EXT = .exe
else
    LDFLAGS = -lglfw -lvulkan -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi
    APP_EXT =
endif

OBJ = .obj
BUILD = build

SRCS = $(wildcard src/*.c) $(wildcard lib/*.c)
OBJS = $(patsubst %.c, $(OBJ)/%.o, $(notdir $(SRCS)))

vpath %.c src lib

APP_NAME = qgpuApp$(APP_EXT)
APP = $(BUILD)/$(APP_NAME)

all: prepare $(APP)

prepare:
	@mkdir -p $(OBJ) $(BUILD)

$(OBJ)/%.o: %.c
	@echo "Kompilacja $<..."
	@$(CC) $(CFLAGS) -c $< -o $@

$(APP): $(OBJS)
	@echo "Linkowanie aplikacji $(APP)..."
	@$(CC) -o $@ $(OBJS) $(LDFLAGS)

run: all
	@./$(BUILD)/$(APP_NAME)

clean:
	@echo "Czyszczenie..."
	@rm -rf $(OBJ) $(BUILD)
	@echo "Wyczyszczone!"

.PHONY: all prepare run clean
