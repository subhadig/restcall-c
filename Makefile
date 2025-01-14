TARGET_EXEC := restcallc
BUILD_DIR := ./build
SRC_DIRS := ./src
INCLUDE_DIR := ./include

SRCS := $(shell find $(SRC_DIRS) -name '*.c')
OBJS := $(SRCS:%.c=$(BUILD_DIR)/%.o)
