CC = gcc
CFLAGS = -Wall -fPIC -g
LDFLAGS = -pthread

# Source files
SRC = memory_manager.c linked_list.c
OBJ = $(SRC:.c=.o)

# Target library
libmemory_manager.so: memory_manager.o
	$(CC) -shared -o $@ $^

# Target to build the linked list application
linked_list_app: linked_list.o
	$(CC) -o $@ linked_list.o -L. -lmemory_manager $(LDFLAGS)

# Target to build test applications
test_linked_list: test_linked_list.c linked_list.o memory_manager.o
	$(CC) -o $@ test_linked_list.c linked_list.o -L. -lmemory_manager $(LDFLAGS)

# Compile object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up generated files
clean:
	rm -f *.o libmemory_manager.so linked_list_app test_linked_list

# Phony targets
.PHONY: clean all

# Default target
all: clean libmemory_manager.so linked_list_app test_linked_list

