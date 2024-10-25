# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -fPIC -g -I.  # Include current directory for headers

# Libraries
LIBS = -pthread -lm

# Targets
TARGET_LIB = libmemory_manager.so
TARGET_APP = linked_list_app

# Source files for memory manager
MEMORY_MANAGER_SRCS = memory_manager.c
# Source files for linked list
LINKED_LIST_SRCS = linked_list.c test_linked_list.c

# Object files for memory manager
MEMORY_MANAGER_OBJS = memory_manager.o
# Object files for linked list
LINKED_LIST_OBJS = linked_list.o test_linked_list.o

# Default target
all: $(TARGET_LIB) $(TARGET_APP)

# Rule to create the shared library
$(TARGET_LIB): $(MEMORY_MANAGER_OBJS)
	$(CC) -shared -o $@ $^

# Rule to create the application
$(TARGET_APP): linked_list.o test_linked_list.o
	$(CC) -o $@ $^ -L. -lmemory_manager $(LIBS)

# Compile source files to object files for memory manager
$(MEMORY_MANAGER_OBJS): $(MEMORY_MANAGER_SRCS)
	$(CC) $(CFLAGS) -c $< -o $@

# Compile linked_list.c only once
linked_list.o: linked_list.c
	$(CC) $(CFLAGS) -c $< -o $@

# Compile test_linked_list.c separately
test_linked_list.o: test_linked_list.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean target
clean:
	rm -f $(MEMORY_MANAGER_OBJS) $(LINKED_LIST_OBJS) $(TARGET_LIB) $(TARGET_APP)

# Phony targets
.PHONY: all clean mmanager list

# Target to compile the memory manager only
mmanager: $(TARGET_LIB)

# Target to compile the linked list application only
list: $(TARGET_APP)
