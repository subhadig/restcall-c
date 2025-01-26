# Variable declarations
TARGET_EXEC := restcallc
BUILD_DIR := ./build
BIN_DIR := ./bin
SRC_DIR := ./src
INCLUDE_DIR := ./include
LIB_DIR := lib
TEST_DIR := tests
UNIT_TEST_DIR := $(TEST_DIR)/unit

SRCS := $(shell find $(SRC_DIR) -name '*.c')
OBJS := $(SRCS:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)

SRCSU := $(wildcard $(UNIT_TEST_DIR)/*.c)
OBJSU := $(SRCSU:$(UNIT_TEST_DIR)/%.c=$(BUILD_DIR)/%.o)

## Libraries
LIB_CJSON_NAME := cjson
LIB_CJSON := lib$(LIB_CJSON_NAME).a
LIB_CJSON_URL := https://github.com/DaveGamble/cJSON/archive/refs/tags/v1.7.18.tar.gz
LIB_CJSON_DIR := $(LIB_DIR)/$(LIB_CJSON_NAME)

CC := cc
CFLAGS := -g -std=gnu17 -pedantic -Wall -Werror -O2 -g3 -D_FORTIFY_SOURCE=2
CPPFLAGS := -I$(INCLUDE_DIR) -I$(LIB_CJSON_DIR)
LDFLAGS := -L$(BUILD_DIR)
LDLIBS := -l$(LIB_CJSON_NAME)

# Targets
restcallc: $(OBJS)
	@echo Building $@...
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $(BIN_DIR)/$@ 
 
unittest: $(OBJSU) $(filter-out %/main.o, $(OBJS))
	@echo Building test executable
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -lcunit -o $(BIN_DIR)/unittests
	@echo Running unittests
	$(BIN_DIR)/unittests

$(OBJSU): $(BUILD_DIR)/%.o: $(UNIT_TEST_DIR)/%.c buildinfra
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $< -lcunit -o $@

$(OBJS): $(BUILD_DIR)/%.o: $(SRC_DIR)/%.c $(LIB_CJSON_NAME) buildinfra
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $< -o $@

buildinfra: $(LIB_CJSON_NAME)

$(LIB_CJSON_NAME): $(BUILD_DIR)/$(LIB_CJSON)

$(BUILD_DIR)/$(LIB_CJSON): dir
	@echo Building $(LIB_CJSON_NAME)...
	cd $(LIB_CJSON_DIR) && $(MAKE) static
	cp $(LIB_CJSON_DIR)/$(LIB_CJSON) $(BUILD_DIR)/

extractlibs:
	@echo Fetching $(LIB_CJSON_NAME)...
	mkdir -p $(LIB_CJSON_DIR)
	wget -qO - $(LIB_CJSON_URL) | tar -xzf - --strip-components=1 -C $(LIB_CJSON_DIR)

cleanlibs:
	rm -rf $(LIB_CJSON_DIR)

dir:
	@echo Creating directories...
	mkdir -p $(BIN_DIR)
	mkdir -p $(BUILD_DIR)

clean:
	@echo Cleaning up...
	rm -rf $(BUILD_DIR) $(BIN_DIR)
	cd $(LIB_CJSON_DIR) && $(MAKE) clean

.PHONY: restcallc clean extractlibs cleanlibs dir unittest buildinfra
