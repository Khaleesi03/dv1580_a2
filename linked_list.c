#include "linked_list.h"
#include <stdio.h>
#include <stdlib.h>

// Function to initialize the linked list
void list_init(Node **head, size_t size) {
    *head = NULL; // Start with an empty list
}

// Function to insert a node at the end of the list
void list_insert(Node **head, uint16_t data) {
    Node *new_node = (Node *)mem_alloc(sizeof(Node)); // Use custom memory manager
    if (!new_node) {
        fprintf(stderr, "Memory allocation failed\n");
        return;
    }

    new_node->data = data;
    new_node->next = NULL;
    pthread_mutex_init(&new_node->lock, NULL); // Initialize mutex for the new node

    if (*head == NULL) {
        *head = new_node; // List was empty, new node becomes the head
    } else {
        Node *current = *head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = new_node; // Add new node at the end
    }
}

// Function to insert a node after a given node
void list_insert_after(Node *prev_node, uint16_t data) {
    if (prev_node == NULL) {
        return; // Previous node cannot be NULL
    }

    Node *new_node = (Node *)mem_alloc(sizeof(Node)); // Use custom memory manager
    if (!new_node) {
        fprintf(stderr, "Memory allocation failed\n");
        return;
    }

    new_node->data = data;
    new_node->next = prev_node->next;
    prev_node->next = new_node; // Insert new node after the previous node
    pthread_mutex_init(&new_node->lock, NULL); // Initialize mutex for the new node
}

// Function to insert a node before a given node
void list_insert_before(Node **head, Node *next_node, uint16_t data) {
    if (next_node == NULL) {
        return; // Next node cannot be NULL
    }

    Node *new_node = (Node *)mem_alloc(sizeof(Node)); // Use custom memory manager
    if (!new_node) {
        fprintf(stderr, "Memory allocation failed\n");
        return;
    }

    new_node->data = data;

    // If inserting before the head
    if (*head == next_node) {
        new_node->next = *head;
        *head = new_node; // New node becomes the new head
        pthread_mutex_init(&new_node->lock, NULL); // Initialize mutex for the new node
        return;
    }

    Node *current = *head;
    while (current != NULL && current->next != next_node) {
        current = current->next;
    }

    if (current == NULL) {
        mem_free(new_node); // Clean up allocated memory if next_node not found
        return; // next_node not found in the list
    }

    new_node->next = next_node;
    current->next = new_node; // Insert new node before next_node
    pthread_mutex_init(&new_node->lock, NULL); // Initialize mutex for the new node
}

// Function to delete a node by value
void list_delete(Node **head, uint16_t data) {
    if (*head == NULL) {
        return; // List is empty
    }

    Node *current = *head;
    Node *previous = NULL;

    while (current != NULL && current->data != data) {
        previous = current;
        current = current->next;
    }

    if (current == NULL) {
        return; // Data not found in the list
    }

    if (previous == NULL) {
        *head = current->next; // Deleting the head node
    } else {
        previous->next = current->next; // Bypass the current node
    }

    mem_free(current); // Free memory using custom memory manager
}

// Function to search for a node by value
Node *list_search(Node **head, uint16_t data) {
    Node *current = *head;

    while (current != NULL) {
        if (current->data == data) {
            return current; // Return the node if found
        }
        current = current->next;
    }
    
    return NULL; // Data not found
}

// Function to display the elements of the list
void list_display(Node **head) {
    Node *current = *head;
    while (current != NULL) {
        printf("%u -> ", current->data);
        current = current->next;
    }
    printf("NULL\n");
}

// Function to display a range of nodes
void list_display_range(Node **head, Node *start_node, Node *end_node) {
    Node *current = start_node;

    while (current != NULL && current != end_node) {
        printf("%u -> ", current->data);
        current = current->next;
    }

    if (end_node != NULL) {
        printf("%u -> ", end_node->data);
    }

    printf("NULL\n");
}

// Function to count the number of nodes in the list
int list_count_nodes(Node **head) {
    int count = 0;
    Node *current = *head;

    while (current != NULL) {
        count++;
        current = current->next;
    }

    return count; // Return the number of nodes
}

// Function to clean up the list and free all nodes
void list_cleanup(Node **head) {
    Node *current = *head;
    Node *next;

    while (current != NULL) {
        next = current->next; // Store the next node
        mem_free(current); // Free each node using custom memory manager
        current = next; // Move to the next node
    }

    *head = NULL; // Set head to NULL after cleanup
}
