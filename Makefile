# Compiler
CC = gcc

# Directories
INCLUDE_DIR = include
SRC_DIR = src
LIB_DIR = lib
OBJ_DIR = bin
TEST_DIR = test

# Output
LIB_NAME = nosocks
LIB_FILE = $(LIB_DIR)/$(LIB_NAME).dll
OBJ_FILE = $(OBJ_DIR)/$(LIB_NAME).o

# Compilation flags
CFLAGS = -Wall -I$(INCLUDE_DIR) -DNOSOCKS_EXPORTS -shared -fPIC
LDFLAGS = -shared
LIBS = -lws2_32

# Source file
SRC_FILE = $(SRC_DIR)/nosocks.c

# Build the DLL
$(LIB_FILE): $(OBJ_FILE)
	if not exist $(LIB_DIR) mkdir $(LIB_DIR)
	$(CC) $(LDFLAGS) -o $(LIB_FILE) $(OBJ_FILE) $(LIBS)

# Compile the source file
$(OBJ_FILE): $(SRC_FILE)
	if not exist $(OBJ_DIR) mkdir $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $(SRC_FILE) -o $(OBJ_FILE)

# Test program
test: $(LIB_FILE)
	$(CC) -o $(TEST_DIR)/test-nosocks $(TEST_DIR)/test-nosocks.c -I$(INCLUDE_DIR) -L$(LIB_DIR) -l$(LIB_NAME) -lws2_32

# Clean up build artifacts
clean:
	del /Q $(OBJ_DIR)\*.o $(LIB_DIR)\*.dll $(TEST_DIR)\test-nosocks.exe

.PHONY: all clean test
