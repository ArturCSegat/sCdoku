# Compiler and flags
CC = gcc
CFLAGS = -Wall -std=c11 -I./src -I./src/control -I./src/draw -I./src/game -I./src/states -I./src/online
LDFLAGS = -lallegro -lallegro_font -lallegro_ttf -lallegro_image -lm

# Project paths
SRC_DIRS = src src/control src/draw src/game src/online
OBJ_DIR = obj/Debug
BIN_DIR = bin/Debug
TARGET = $(BIN_DIR)/sudoku

# Find all source files and map them to object files
SRCS := main.c $(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/*.c))
OBJS := $(patsubst %.c,$(OBJ_DIR)/%.o,$(SRCS))

# Default target
all: $(TARGET)

# Link
$(TARGET): $(OBJS)
	@mkdir -p $(BIN_DIR)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

# Compile
$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean
clean:
	rm -rf $(OBJ_DIR) $(TARGET)

.PHONY: all clean

