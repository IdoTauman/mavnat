#ifndef MAVNAT_ARRAYLIST_H
#define MAVNAT_ARRAYLIST_H

#include "../../util/arena.h"
#include <stddef.h>

/* Standard comparator signature for your void* generics */
typedef int (*mavnat_compare_fn)(const void *a, const void *b);

/* Opaque handle for the ArrayList */
typedef struct ArrayList ArrayList;

/**
 * Initializes a new ArrayList using the provided arena.
 * @param arena The arena to allocate the struct and initial buffer from.
 * @param initial_capacity Recommended to be a power of 2 (e.g., 8, 16).
 * @return Pointer to the newly allocated ArrayList.
 */
ArrayList *mavnat_arraylist_init(Arena *arena, size_t initial_capacity);

/**
 * Appends an element to the end of the list.
 * If capacity is reached, it allocates a new buffer in the arena and memcpys.
 */
void mavnat_arraylist_push(ArrayList *list, void *element);

/**
 * Removes and returns the last element in the list.
 * @return The popped element, or NULL if the list is empty.
 */
void *mavnat_arraylist_pop(ArrayList *list);

/**
 * Retrieves the element at the specified index.
 */
void *mavnat_arraylist_get(ArrayList *list, size_t index);

/**
 * Overwrites the element at the specified index.
 */
void mavnat_arraylist_set(ArrayList *list, size_t index, void *element);

/**
 * Returns the current number of elements in the list.
 */
size_t mavnat_arraylist_size(ArrayList *list);

/**
 * Returns the current maximum capacity before a reallocation is triggered.
 */
size_t mavnat_arraylist_capacity(ArrayList *list);

/**
 * Clears the list by setting size to 0. 
 * Does NOT free or zero out the memory, allowing for instant reuse of the capacity.
 */
void mavnat_arraylist_clear(ArrayList *list);

#endif // MAVNAT_ARRAYLIST_H