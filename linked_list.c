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
        free(iter);
    }
    return true;
}

struct iterator * linked_list_create_iterator(struct linked_list * ll,
                                              size_t index) {
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