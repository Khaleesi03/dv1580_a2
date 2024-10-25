# Makefile

CC = gcc
CFLAGS = -Wall -fPIC -g -I.
LDFLAGS = -pthread -lm

# Source files
MEMORY_MANAGER_SRC = memory_manager.c
LINKED_LIST_SRC = linked_list.c
TEST_MEMORY_MANAGER_SRC = test_memory_manager.c
TEST_LINKED_LIST_SRC = test_linked_list.c

# Object files
MEMORY_MANAGER_OBJ = $(MEMORY_MANAGER_SRC:.c=.o)
LINKED_LIST_OBJ = $(LINKED_LIST_SRC:.c=.o)
TEST_MEMORY_MANAGER_OBJ = $(TEST_MEMORY_MANAGER_SRC:.c=.o)
TEST_LINKED_LIST_OBJ = $(TEST_LINKED_LIST_SRC:.c=.o)

# Library and executable names
LIBRARY = libmemory_manager.so
EXECUTABLE = linked_list_app
TEST_EXECUTABLE = test_linked_list
TEST_MEMORY_EXECUTABLE = test_memory_manager

.PHONY: all clean mmanager list test

# Default target to build everything
all: mmanager list test_memory_manager

# Build the memory manager library
mmanager: $(MEMORY_MANAGER_OBJ)
	$(CC) -shared -o $(LIBRARY) $^

# Build the linked list application
list: $(LINKED_LIST_OBJ) $(TEST_LINKED_LIST_OBJ)
	$(CC) -o $(EXECUTABLE) $(LINKED_LIST_OBJ) $(TEST_LINKED_LIST_OBJ) -L. -lmemory_manager $(LDFLAGS)

# Build the test linked list application
test: $(TEST_LINKED_LIST_OBJ) $(LINKED_LIST_OBJ)
	$(CC) -o $(TEST_EXECUTABLE) $(TEST_LINKED_LIST_OBJ) $(LINKED_LIST_OBJ) -L. -lmemory_manager $(LDFLAGS)

# Build the test memory manager application
test_memory_manager: $(TEST_MEMORY_MANAGER_OBJ) $(MEMORY_MANAGER_OBJ)
	$(CC) -o $(TEST_MEMORY_EXECUTABLE) $(TEST_MEMORY_MANAGER_OBJ) $(MEMORY_MANAGER_OBJ) -L. -lmemory_manager $(LDFLAGS)

# Compile the object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up generated files
clean:
	rm -f $(MEMORY_MANAGER_OBJ) $(LINKED_LIST_OBJ) $(TEST_MEMORY_MANAGER_OBJ) $(TEST_LINKED_LIST_OBJ) $(EXECUTABLE) $(TEST_EXECUTABLE) $(TEST_MEMORY_EXECUTABLE) $(LIBRARY)
