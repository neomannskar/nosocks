# nosocks

Wrapper for socket programming in C on Windows.

Abstracts away the initialization of:

1. windows.h
2. winsock2.h
3. ws2tcpip.h

Compile lib:

```
gcc -Wall -Iinclude -DNOSOCKS_EXPORTS -c lib/nosocks.c -o bin/nosocks.o -lws2_32

gcc -shared -o nosocks.dll nosocks.o -lws2_32
```

Make file for project:

```makefile
# Define variables
CC = gcc
CFLAGS = -Wall -Iinclude
LDFLAGS = -Llib -lnosocks -lws2_32
SRC_DIR = src
BUILD_DIR = build
BIN_DIR = bin
LIB_DIR = lib

# Define the executable name
EXEC = main.exe

# Source files and object files
SOURCES = $(wildcard $(SRC_DIR)/*.c)
OBJECTS = $(SOURCES:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)

# Default target
all: $(EXEC)

# Link the executable
$(EXEC): $(OBJECTS)
	$(CC) -o $(BIN_DIR)/$(EXEC) $(OBJECTS) $(LDFLAGS)

# Compile source files to object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up build artifacts
clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)/$(EXEC)
```

Compile test:

```
gcc -Wall -Wextra -o test-nosocks test-nosocks.c -L../lib -lws2_32 -lnosocks
```
