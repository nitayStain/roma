CC          := gcc

FLAGS      := -Wall -Wextra -O2 -I. -Iroc/include -MMD -MP
LDFLAGS     := -Lroc -lroc -Lnapoli -lnapoli

SRC_DIR     := src
OBJ_DIR     := obj
TARGET      := roma

ROC_LIB     := roc/libroc.a
NAPOLI_LIB  := napoli/libnapoli.a

SRCS        := $(shell find $(SRC_DIR) -name "*.c")
OBJS        := $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
DEPS        := $(OBJS:.o=.d)

.PHONY: all clean roc napoli

all: $(TARGET)

# Objects must come before -lroc -lnapoli on the link line (the linker
# only resolves symbols from a static library against what it's already
# seen).
$(TARGET): $(OBJS) $(ROC_LIB) $(NAPOLI_LIB)
	$(CC) $(OBJS) $(LDFLAGS) -o $@

$(ROC_LIB):
	$(MAKE) -C roc

$(NAPOLI_LIB):
	$(MAKE) -C napoli

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Convenience shortcuts for building/testing a single module in isolation
roc:
	$(MAKE) -C roc

napoli:
	$(MAKE) -C napoli

clean:
	rm -rf $(OBJ_DIR) $(TARGET)
	$(MAKE) -C roc clean
	$(MAKE) -C napoli clean

-include $(DEPS)
