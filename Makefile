# --- Compiler and Flags ---
CC          := gcc
# -Iinclude tells the compiler to look in the include/ directory for headers
# -MMD -MP generate per-object .d files listing header dependencies, so editing
# a header (e.g. token.h) forces a rebuild of every .o that includes it.
CFLAGS      := -Wall -Wextra -O2 -Iinclude -MMD -MP
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
# Auto-generated header dependency files (one per .o)
DEPS        := $(OBJS:.o=.d)

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

# Pull in generated dependency files (silently ignored if absent)
-include $(DEPS)
