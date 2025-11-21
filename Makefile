CC      := gcc
CFLAGS  := -std=c99 -Wall -Wextra -Wpedantic \
           -Wshadow -Wpointer-arith -Wcast-qual \
           -Wstrict-prototypes -Wmissing-prototypes \
           -Wconversion -Wuninitialized -Wunreachable-code \
           -Wfloat-equal
CPPFLAGS := -Iinclude

SRC_DIR := src
OBJ_DIR := build

SRCS := $(wildcard $(SRC_DIR)/*.c)
OBJS := $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

BIN := odon

$(BIN): $(OBJS)
	$(CC) $(OBJS) -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

TEST_SRCS := $(wildcard test/unit/*.c)
SRC_SRCS  := $(wildcard src/*.c)
SRC_IMPL  := $(filter-out src/main.c, $(SRC_SRCS))

TEST_BIN := $(TEST_SRCS:test/unit/%.c=test/unit/%)

unit: $(TEST_BIN)
	@for t in $(TEST_BIN); do \
		echo "Running $$t"; \
		./$$t || exit 1; \
	done

integration: $(BIN)
	cd test/integration && go test -v .

clean:
	rm -rf $(OBJ_DIR) $(BIN) $(TEST_BIN)

.PHONY: unit integration clean
