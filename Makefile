# Compiler and flags
CC = gcc
CFLAGS = -Wall -std=c11 -I./src -I./src/control -I./src/draw -I./src/game -I./src/states -I./src/online
LDFLAGS = -lallegro -lallegro_font -lallegro_ttf -lallegro_image -lallegro_primitives -lm

# Enable AddressSanitizer with ASAN=1
ifeq ($(ASAN),1)
    CFLAGS += -fsanitize=address -g
    LDFLAGS += -fsanitize=address
endif

# Project paths
SRC_DIRS = src src/control src/control/validation src/draw src/game
OBJ_DIR = obj/Debug
BIN_DIR = bin/Debug
TARGET = $(BIN_DIR)/sudoku

# OS selector (unix or win)
OS ?= unix

ifeq ($(OS),unix)
    ONLINE_SRC := src/online/online_unix.c
    CFLAGS += -DUNIX_PLATFORM
else ifeq ($(OS),win)
    ONLINE_SRC := src/online/online_win.c
    CFLAGS += -DWIN_PLATFORM
else
    $(error Unknown OS type: $(OS))
endif

# Find all .c source files and map to .o object files
SRCS := main.c $(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/*.c)) $(ONLINE_SRC)
OBJS := $(patsubst %.c,$(OBJ_DIR)/%.o,$(SRCS))

# Default target
all: $(TARGET)

# Linking
$(TARGET): $(OBJS)
	@mkdir -p $(BIN_DIR)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

# Compilation
$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean target
clean:
	rm -rf $(OBJ_DIR) $(TARGET)

.PHONY: all clean

