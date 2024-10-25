#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <pthread.h>
#include "linked_list.h"

// Global mutex to ensure thread-safe operations
pthread_mutex_t list_lock = PTHREAD_MUTEX_INITIALIZER;

// List initialization
// Initializes the head of the list to NULL and sets up the memory pool using mem_init
void list_init(Node **head, size_t size) {
    *head = NULL; // Start with an empty list
    mem_init(size); // Initialize memory manager with the given size
}

// Insertion function
// Inserts a new node at the end of the list
void list_insert(Node **head, uint16_t data) {
    pthread_mutex_lock(&list_lock);  // Lock the list for thread safety

    Node *new_node = (Node *) mem_alloc(sizeof(Node));  // Allocate memory for the new node using memory manager
    if (new_node == NULL) {
        printf("Memory allocation failed\n");
        pthread_mutex_unlock(&list_lock);  // Unlock before returning
        return;
    }
    new_node->data = data;
    new_node->next = NULL;

    // If the list is empty, make the new node the head
    if (*head == NULL) {
        *head = new_node;
    } else {
        // Traverse to the end of the list and add the new node
        Node *temp = *head;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = new_node;
    }

    pthread_mutex_unlock(&list_lock);  // Unlock the list after insertion
}

// Insert after a specific node
void list_insert_after(Node *prev_node, uint16_t data) {
    pthread_mutex_lock(&list_lock);  // Lock the list for thread safety

    if (prev_node == NULL) {
        printf("The previous node cannot be NULL\n");
        pthread_mutex_unlock(&list_lock);  // Unlock before returning
        return;
    }

    Node *new_node = (Node *) mem_alloc(sizeof(Node));  // Allocate memory for the new node using memory manager
    if (new_node == NULL) {
        printf("Memory allocation failed\n");
        pthread_mutex_unlock(&list_lock);  // Unlock before returning
        return;
    }

    new_node->data = data;
    new_node->next = prev_node->next;
    prev_node->next = new_node;

    pthread_mutex_unlock(&list_lock);  // Unlock the list after insertion
}

// Insert before a specific node
void list_insert_before(Node **head, Node *next_node, uint16_t data) {
    pthread_mutex_lock(&list_lock);  // Lock the list for thread safety

    if (next_node == NULL || *head == NULL) {
        printf("The node to insert before cannot be NULL\n");
        pthread_mutex_unlock(&list_lock);  // Unlock before returning
        return;
    }

    Node *new_node = (Node *) mem_alloc(sizeof(Node));  // Allocate memory for the new node using memory manager
    if (new_node == NULL) {
        printf("Memory allocation failed\n");
        pthread_mutex_unlock(&list_lock);  // Unlock before returning
        return;
    }

    new_node->data = data;

    // If the new node is the head, insert the new node as the new head
    if (*head == next_node) {
        new_node->next = *head;
        *head = new_node;
    } else {
        Node *temp = *head;
        while (temp->next != next_node) {
            temp = temp->next;
        }
        new_node->next = next_node;
        temp->next = new_node;
    }

    pthread_mutex_unlock(&list_lock);  // Unlock the list after insertion
}

// Deletion function
void list_delete(Node **head, uint16_t data) {
    pthread_mutex_lock(&list_lock);  // Lock the list for thread safety

    if (*head == NULL) {
        printf("List is empty, cannot delete\n");
        pthread_mutex_unlock(&list_lock);  // Unlock before returning
        return;
    }

    Node *temp = *head;
    Node *prev = NULL;

    if (temp != NULL && temp->data == data) {
        *head = temp->next;  // Move the head to the next node
        mem_free(temp);  // Use memory manager to free the memory of the old head
        pthread_mutex_unlock(&list_lock);  // Unlock after deletion
        return;
    }

    while (temp != NULL && temp->data != data) {
        prev = temp;
        temp = temp->next;
    }

    if (temp == NULL) {
        printf("Node with data %d not found\n", data);
        pthread_mutex_unlock(&list_lock);  // Unlock if no node is found
        return;
    }

    prev->next = temp->next;
    mem_free(temp);  // Use custom memory manager to free memory

    pthread_mutex_unlock(&list_lock);  // Unlock after deletion
}

// Search function
Node *list_search(Node **head, uint16_t data) {
    pthread_mutex_lock(&list_lock);  // Lock the list for thread safety

    Node *temp = *head;
    while (temp != NULL) {
        if (temp->data == data) {
            pthread_mutex_unlock(&list_lock);  // Unlock after finding the node
            return temp;
        }
        temp = temp->next;
    }

    pthread_mutex_unlock(&list_lock);  // Unlock if no node is found
    return NULL;
}

// Display all nodes
void list_display(Node **head) {
    pthread_mutex_lock(&list_lock);  // Lock the list for thread safety

    Node *temp = *head;
    printf("[");
    while (temp != NULL) {
        printf("%d", temp->data);
        if (temp->next != NULL) {
            printf(", ");
        }
        temp = temp->next;
    }
    printf("]\n");

    pthread_mutex_unlock(&list_lock);  // Unlock after displaying
}

// Display a range of nodes
void list_display_range(Node **head, Node *start_node, Node *end_node) {
    pthread_mutex_lock(&list_lock);  // Lock the list for thread safety

    Node *temp = *head;

    if (start_node == NULL) {
        start_node = *head;
    }

    while (temp != start_node && temp != NULL) {
        temp = temp->next;
    }

    printf("[");
    while (temp != NULL) {
        printf("%d", temp->data);
        if (temp->next != end_node->next) {
            printf(", ");
        }
        temp = temp->next;

        if (temp == end_node) {
            printf(", %d", temp->data);
            break;
        }
    }
    printf("]\n");

    pthread_mutex_unlock(&list_lock);  // Unlock after displaying the range
}

// Count the number of nodes
size_t list_current_nodes(Node *head) {
    pthread_mutex_lock(&list_lock);  // Lock the list for thread safety

    size_t count = 0;
    Node *current = head;

    while (current != NULL) {
        count++;
        current = current->next;
    }

    pthread_mutex_unlock(&list_lock);  // Unlock after counting
    return count;
}

// Cleanup function
void list_cleanup(Node **head) {
    pthread_mutex_lock(&list_lock);  // Lock the list for thread safety

    Node *temp = *head;
    Node *next_node;

    while (temp != NULL) {
        next_node = temp->next;
        mem_free(temp);  // Use memory manager to free memory
        temp = next_node;
    }

    *head = NULL;  // Set the head to NULL after cleanup
    pthread_mutex_unlock(&list_lock);  // Unlock after cleanup
}

int main() {
    Node *head = NULL;
    list_init(&head, 1024);  // Example initialization

    list_insert(&head, 10);   // Insert example data
    list_insert(&head, 20);

    list_display(&head);      // Display the list

    list_cleanup(&head);      // Clean up the list
    return 0;                 // Return 0 to indicate success
}
