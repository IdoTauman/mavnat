#include "arraylist.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define mavnat_unlikely(x) __builtin_expect(!!(x), 0)

#define ARENA_BLOCK_SIZE 4096

/**
 * Internal definition of the opaque ArrayList structure.
 */
struct ArrayList {
    void **data;      // Array of generic pointers
    size_t capacity;  // Maximum number of items before reallocation
    size_t size;      // Current number of items
    Arena *arena;     // The arena that owns this memory
};

/**
 * Internal helper to handle the allocation of a new buffer and memcpy.
 */
static void mavnat_arraylist_grow(ArrayList *list) {
    size_t old_cap = list->capacity;
    list->capacity <<= 1;
    // overflow check
    if (list->capacity <= old_cap) {
        fprintf(stderr, "FATAL: arraylist size out of size_t range");
        exit(2);
    }

    void **new = (void **)mavnat_arena_alloc(list->arena, list->capacity * sizeof(void *));
    memcpy(new, list->data, list->size * sizeof(void *));
    list->data = new;
}

ArrayList *mavnat_arraylist_init(Arena *arena, size_t initial_capacity) {
    // invalid capacity fallback
    if (initial_capacity == 0) initial_capacity = 8;

    // init struct
    ArrayList *new = mavnat_arena_alloc(arena, sizeof(ArrayList));

    // fill initial values
    new->arena = arena;
    new->capacity = initial_capacity;
    new->size = 0;
    // 64 byte alignment for avx support
    new->data = mavnat_arena_alloc_aligned(arena, initial_capacity * sizeof(void *), 64);
    return new;
}

void mavnat_arraylist_push(ArrayList *list, void *element) {
    // grow the list if needed
    if (mavnat_unlikely(list->size >= list->capacity)) {
        mavnat_arraylist_grow(list);
    }

    list->data[list->size] = element;
    list->size++;
}

void *mavnat_arraylist_pop(ArrayList *list) {
    if (list->size == 0) return NULL;
    return list->data[--list->size];
}

void *mavnat_arraylist_get(ArrayList *list, size_t index) {
    assert(index < list->size);
    return list->data[index];
}

void mavnat_arraylist_set(ArrayList *list, size_t index, void *element) {
    assert(index < list->size);
    list->data[index] = element;
}

size_t mavnat_arraylist_size(ArrayList *list) {
    return list->size;
}

size_t mavnat_arraylist_capacity(ArrayList *list) {
    return list->capacity;
}

void mavnat_arraylist_clear(ArrayList *list) {
    list->size = 0;
}