TARGET_EXEC := restcallc
BUILD_DIR := ./build
BIN_DIR := ./bin
SRC_DIR := ./src
INCLUDE_DIR := ./include

SRCS := $(shell find $(SRC_DIR) -name '*.c')
OBJS := $(SRCS:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)

CC := cc
CFLAGS := -std=gnu17 -pedantic -Wall -Werror -O2 -g3 -D_FORTIFY_SOURCE=2

restcallc: $(OBJS)
	mkdir -p $(BIN_DIR)
	$(CC) $^ -o $(BIN_DIR)/$@

$(OBJS): $(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(BUILD_DIR)
	$(CC) -c $(CFLAGS) -I$(INCLUDE_DIR) $^ -o $@

clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)

.PHONY: restcallc clean
