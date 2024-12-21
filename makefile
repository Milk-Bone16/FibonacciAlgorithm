# Compiler
CC = gcc

# Compiler flags for performance and warnings
CFLAGS = -O3 -Wall -Wextra -march=native -mtune=native

# Linker flags (for math.h and gmp.h)
LDFLAGS = -lm -lgmp

# Source and executable
SRC = fib.c
EXEC = fib
PROFILED_EXEC = fib-profiled

# Default target
all: $(EXEC)

# Rule to build the executable
$(EXEC): $(SRC)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

benchmark: CFLAGS += -DBENCHMARK
benchmark: $(PROFILED_EXEC)

$(PROFILED_EXEC): $(SRC)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# Clean up generated files
clean:
	rm -f $(EXEC)

# Phony targets
.PHONY: all clean benchmark

