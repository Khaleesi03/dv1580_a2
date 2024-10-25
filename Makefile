CC = gcc
CFLAGS = -Wall -fPIC -g
LDFLAGS = -pthread

# Source files
SRC = memory_manager.c linked_list.c test_linked_list.c
OBJ = $(SRC:.c=.o)

# Target library
libmemory_manager.so: memory_manager.o
	$(CC) -shared -o $@ $^

# Target to build the application
linked_list_app: $(OBJ)
	$(CC) -o linked_list_app $(OBJ) -L. -lmemory_manager $(LDFLAGS)

# Compile object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up generated files
clean:
	rm -f *.o libmemory_manager.so linked_list_app

# Phony targets
.PHONY: clean all

# Default target
all: clean linked_list_app
