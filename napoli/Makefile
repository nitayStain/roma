# --- Compiler and Flags ---
CC          := gcc
AR          := ar
# napoli.h lives at the module root (not in an include/ dir - it's
# napoli's only header, and also needs to be reachable as "napoli/napoli.h"
# from outside, which only works if it sits at the module root).
# -MMD -MP generate per-object .d files listing header dependencies, so editing
# a header forces a rebuild of every .o that includes it.
CFLAGS      := -Wall -Wextra -O2 -I. -MMD -MP
ARFLAGS     := rcs

# --- Directories ---
SRC_DIR     := src
OBJ_DIR     := obj

# --- Target Library ---
TARGET      := libnapoli.a

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

# Rule to archive the object files into a static library
$(TARGET): $(OBJS)
	$(AR) $(ARFLAGS) $@ $^

# Rule to compile source files into object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up build artifacts
clean:
	rm -rf $(OBJ_DIR) $(TARGET)

# Pull in generated dependency files (silently ignored if absent)
-include $(DEPS)
