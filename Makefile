CC      := gcc
CFLAGS  := -std=c99 -Wall -Wextra -Wpedantic \
		   -Wshadow -Wpointer-arith -Wcast-qual \
		   -Wstrict-prototypes -Wmissing-prototypes \
		   -Wconversion -Wuninitialized -Wunreachable-code \
		   -Wfloat-equal -Wwrite-strings -Wswitch-enum \
		   -Wredundant-decls -Wformat=2 -Wno-discarded-qualifiers

# Collect all source files recursively
SRC     := $(shell find src -name '*.c')
OBJ     := $(SRC:.c=.o)

# Collect all include directories recursively
INC     := $(shell find include -type d)
CFLAGS  += $(patsubst %,-I%,$(INC))

TARGET  := odon
LIB     := libodon.a

.PHONY: all clean unit integration

all: $(TARGET)

# Build static library from core sources
$(LIB): $(OBJ)
	ar rcs $@ $^

# Build main executable using the library
$(TARGET): $(LIB) main.o
	$(CC) $(CFLAGS) -o $@ main.o $(LIB)

# Generic object rule (works with subfolders)
%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJ) main.o $(TARGET) $(LIB) $(TEST_BIN)

# ---------- Unit testing ----------

TEST_SRC := $(shell find test/unit -name '*.c')
TEST_BIN := $(TEST_SRC:.c=)

unit: $(TEST_BIN)
	@for t in $(TEST_BIN); do \
		echo "Running $$t"; \
		./$$t || exit 1; \
	done

# Build test executables linking against the core library
test/unit/%: test/unit/%.c $(LIB)
	$(CC) $(CFLAGS) -o $@ $< $(LIB)

# ---------- Integration tests (Go) ----------

integration: all
	cd test/integration && go test -v .
