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
LINKED_LIST_EXECUTABLE = linked_list_app
TEST_MEMORY_MANAGER_EXECUTABLE = test_memory_manager
TEST_LINKED_LIST_EXECUTABLE = test_linked_list

.PHONY: all clean

# Default target to build everything
all: $(LIBRARY) $(LINKED_LIST_EXECUTABLE) $(TEST_MEMORY_MANAGER_EXECUTABLE) $(TEST_LINKED_LIST_EXECUTABLE)

# Build the memory manager library
$(LIBRARY): $(MEMORY_MANAGER_OBJ)
	$(CC) -shared -o $@ $^

# Build the linked list application
$(LINKED_LIST_EXECUTABLE): $(LINKED_LIST_OBJ) $(TEST_LINKED_LIST_OBJ)
	$(CC) -o $@ $^ -L. -lmemory_manager $(LDFLAGS)

# Build the test memory manager application
$(TEST_MEMORY_MANAGER_EXECUTABLE): $(TEST_MEMORY_MANAGER_OBJ) $(MEMORY_MANAGER_OBJ)
	$(CC) -o $@ $^ -L. -lmemory_manager $(LDFLAGS)

# Build the test linked list application
$(TEST_LINKED_LIST_EXECUTABLE): $(TEST_LINKED_LIST_OBJ) $(LINKED_LIST_OBJ)
	$(CC) -o $@ $^ -L. -lmemory_manager $(LDFLAGS)

# Compile the object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up generated files
clean:
	rm -f $(MEMORY_MANAGER_OBJ) $(LINKED_LIST_OBJ) $(TEST_MEMORY_MANAGER_OBJ) $(TEST_LINKED_LIST_OBJ) $(LINKED_LIST_EXECUTABLE) $(TEST_MEMORY_MANAGER_EXECUTABLE) $(TEST_LINKED_LIST_EXECUTABLE) $(LIBRARY)
