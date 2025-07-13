#include "linked_list.h"

// Function pointers to (potentially) custom malloc() and
// free() functions.
//
static void * (*malloc_fptr)(size_t size) = NULL;
static void   (*free_fptr)(void* addr)    = NULL; 

bool linked_list_register_malloc(void * (*malloc)(size_t)) {
    if (malloc == NULL) {
        return false;
    }
    malloc_fptr = malloc;
    return true;
}

bool linked_list_register_free(void (*free)(void*)) {
    if (free == NULL) {
        return false;
    }
    free_fptr = free;
    return true;
}

bool linked_list_iterate(struct iterator * iter) {
    // Check if the iterator is NULL or if it has no current node or linked list
    // associated with it.
    if (iter == NULL || iter->current_node == NULL || iter->ll == NULL) {
        return false;
    }

    // Call the user-defined function with the current element
    if (iter->current_node->next != NULL) {
        return true;
    } else {
        // If the next node is NULL
        return false;
    }
}

bool linked_list_delete_iterator(struct iterator * iter) {
    if (iter == NULL) {
        return false;
    }

    // Free the iterator structure
    if (free_fptr != NULL) {
        free_fptr(iter);
    } else {
        free_fptr(iter);
    }
    return true;
}

struct iterator * linked_list_create_iterator(struct linked_list * ll, size_t index) {
    if (ll == NULL) {
        return NULL;
    }

    struct iterator *iter = (struct iterator *)malloc_fptr(sizeof(struct iterator));
    if (iter == NULL) {
        return NULL;
    }

    // Initialize the iterator
    iter->ll = ll;
    iter->current_node = ll->head;
    iter->current_index = 0;

    // Move the iterator to the specified index
    while (iter->current_index < index) {
        if (iter->current_node != NULL) {
            iter->current_node = iter->current_node->next;
            iter->current_index++;
        } else {
            return NULL; // Index out of bounds
        }
    }

    return iter;
}

bool linked_list_remove(struct linked_list * ll, size_t index) {
    if (ll == NULL) {
        return false;
    }

    struct node *current = ll->head;
    struct node *previous = NULL;

    // Traverse the list to find the node to remove
    for (size_t i = 0; i < index; i++) {
        if (current == NULL) {
            return false; // Index out of bounds
        }
        previous = current;
        current  = current->next;
    }

    // If we found the node to remove
    if (current != NULL) {
        previous->next = current->next;
        current->next  = NULL;
    } else {
        return false; // Index out of bounds
    }

    // Free the node
    free_fptr((void *)current);
    return true;
}

size_t linked_list_find(struct linked_list * ll, unsigned int data) {
    if (ll == NULL || ll->head == NULL) {
        return SIZE_MAX; // Linked list is empty or NULL
    }

    struct node *current = ll->head;
    size_t index = 0;

    // Traverse the linked list to find the data
    while (current != NULL) {
        if (current->data == data) {
            return index; // Return the index of the found data
        }
        current = current->next;
        index++;
    }

    return SIZE_MAX; // Data not found
}

bool linked_list_insert(struct linked_list * ll, size_t index, unsigned int data) {
    if (ll == NULL) {
        return false; // Invalid linked list or index
    }

    struct node *current = ll->head;
    struct node *previous = NULL;
    struct node * new = (struct node *)malloc_fptr(sizeof(struct node));
    if (new == NULL) {
        return false; // Memory allocation failed
    }

    if (ll->head == NULL) {
        if (index != 0) {
            return false; // Cannot insert at non-zero index in an empty list
        }
        new->data = data;
        new->next = current;
        ll->head = new;
        return true;
    }

    uint32_t i = 0;
    while (current != NULL && i < index) {
        previous = current;
        current = current->next;
        i++;
    }

    new->data = data;
    new->next = current;
    previous->next = new;
    return true; // Insertion successful   
}

bool linked_list_insert_front(struct linked_list * ll, unsigned int data) {
    if (ll == NULL) {
        return false; // Invalid linked list or index
    }
    struct node *new = (struct node *)malloc_fptr(sizeof(struct node));
    if (new == NULL) {
        return false; // Memory allocation failed
    }

    if (ll->head == NULL) {
        ll->head = new;
        new->data = data;
        new->next = NULL;
    } else {
        new->next = ll->head;
        new->data = data;
        ll->head = new;
    }
    return true; // Insertion successful
}

bool linked_list_insert_end(struct linked_list * ll, unsigned int data) {
    if (ll == NULL) {
        return false; // Invalid linked list or index
    }

    struct node *new = (struct node *)malloc_fptr(sizeof(struct node));
    if (new == NULL) {
        return false; // Memory allocation failed
    }

    new->data = data;
    new->next = NULL;

    if (ll->head == NULL) {
        ll->head = new; // Insert at the beginning if the list is empty
        return true;
    }

    struct node *current = ll->head;
    while (current->next != NULL) {
        current = current->next; // Traverse to the end of the list
    }
    current->next = new; // Insert at the end
    return true; // Insertion successful
}

size_t linked_list_size(struct linked_list * ll) {
    if (ll == NULL) {
        return SIZE_MAX; // Invalid linked list
    }
    if (ll->head == NULL) {
        return 0; // Empty linked list
    }
    size_t ll_size = 0;
    struct node *current = ll->head;
    while (current != NULL) {
        ll_size++;
        current = current->next;
    }
    return ll_size + 1;
}

bool linked_list_delete(struct linked_list * ll) {
    if (ll == NULL) {
        return false; // Invalid linked list
    }
    struct node *current = ll->head;
    struct node *next_node;

    while (current != NULL) {
        next_node = current->next;
        free_fptr((void *)current);
        current = next_node;
    }

    ll->head = NULL;
    return true;
}

struct linked_list * linked_list_create(void) {
    struct linked_list *ll = (struct linked_list *)malloc_fptr(sizeof(struct linked_list));
    if (ll == NULL) {
        return NULL; // Memory allocation failed
    }
    ll->head = NULL; // Initialize the head to NULL for an empty list
    return ll;
}

