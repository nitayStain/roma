# --- Compiler and Flags ---
CC          := gcc
# -Iinclude tells the compiler to look in the include/ directory for headers
CFLAGS      := -Wall -Wextra -O2 -Iinclude
LDFLAGS     := 

# --- Directories ---
SRC_DIR     := src
OBJ_DIR     := obj
BIN_DIR     := .

# --- Target Binary ---
TARGET      := $(BIN_DIR)/roma

# --- Files ---
# Find all .c files in the src directory
SRCS        := $(shell find $(SRC_DIR) -name "*.c")
# Map those .c files to .o files inside the obj directory
OBJS        := $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# --- Phony Targets ---
.PHONY: all clean

# Default rule
all: $(TARGET)

# Rule to link the final executable
$(TARGET): $(OBJS) | $(BIN_DIR)
	$(CC) $(LDFLAGS) $^ -o $@

# Rule to compile source files into object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Create directories if they do not exist
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up build artifacts
clean:
	rm -rf $(OBJ_DIR) $(TARGET)
