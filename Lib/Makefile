CC = gcc

CFLAGS = -Wall -Wextra -O2 -I. -Iinclude -Ilib -ffunction-sections -fdata-sections -MMD -MP

ifeq ($(OS),Windows_NT)
    LDFLAGS = -Wl,--gc-sections -lglfw3 -lvulkan -lgdi32 -luser32 -lshell32
    APP_EXT = .exe
else
    LDFLAGS = -Wl,--gc-sections -lglfw -lvulkan -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi
    APP_EXT =
endif

OBJ = .obj
BUILD = build

SRCS = $(wildcard src/*.c) $(wildcard lib/*.c)
OBJS = $(patsubst %.c, $(OBJ)/%.o, $(SRCS))
DEPS = $(OBJS:.o=.d)

APP_NAME = qgpuApp$(APP_EXT)
APP = $(BUILD)/$(APP_NAME)

all: prepare $(APP)

prepare:
	@mkdir -p $(OBJ)/src $(OBJ)/lib $(BUILD)

$(OBJ)/%.o: %.c
	@echo "Compilation $<..."
	@$(CC) $(CFLAGS) -c $< -o $@

$(APP): $(OBJS)
	@echo "Linking app $(APP)..."
	@$(CC) $(OBJS) -o $@ $(LDFLAGS)

-include $(DEPS)

run: all
	@./$(APP)

clean:
	@echo "Cleaning..."
	@rm -rf $(OBJ) $(BUILD)
	@echo "Cleaned!"

.PHONY: all prepare run clean
