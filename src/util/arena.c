#include "arena.h"
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/**
 * Internal block structure to manage the linked list of memory chunks.
 */
typedef struct ArenaBlock ArenaBlock;
struct ArenaBlock {
    ArenaBlock *next;
    ArenaBlock *prev;
    size_t capacity;
    size_t used;
    void *data;
};

/**
 * The actual Arena definition (kept opaque in the header).
 */
struct Arena {
    ArenaBlock *head; // oldest
    ArenaBlock *tail; // newest
    size_t block_size;
};

/**
 * Internal functions that handles malloc errors.
 */
void *mavnat_malloc(size_t size) {
    void *block = malloc(size);
    if (!block) {
        fprintf(stderr, "FATAL: Out of memory");
        exit(1);
    }
    return block;
}

/**
 * Internal function that handles calloc errors.
 */
void *mavnat_calloc(size_t size) {
    void *block = calloc(1, size);
    if (!block) {
        fprintf(stderr, "FATAL: Out of memory");
        exit(1);
    }
    return block;
}

/**
 * Internal helper signature for allocating a new block.
 */
static ArenaBlock *mavnat_new_block(size_t size, ArenaBlock *prev) {
    size_t total_size = sizeof(ArenaBlock) + size;
    ArenaBlock *new = mavnat_malloc(total_size);
    new->data = (void*)(new + 1);
    new->prev = prev;
    new->capacity = size;
    if (prev) prev->next = new;
    return new;
}

Arena *mavnat_arena_init(size_t block_size) {
    Arena *new = mavnat_calloc(sizeof(Arena));
    new->block_size = block_size;
    return new;
}

void *mavnat_arena_alloc(Arena *arena, size_t size) {
    size_t block_size = size > arena->block_size ? size : arena->block_size;
    if (!(arena->head)) {
        arena->head = mavnat_new_block(block_size, NULL);
        arena->tail = arena->head;
    }

    ArenaBlock *tail = arena->tail;
    size_t space = tail->capacity - tail->used;
    if (space < size) {
        arena->tail = mavnat_new_block(block_size, arena->tail);
        tail = arena->tail;
    }

    void *ptr = (char*)tail->data + tail->used;
    tail->used += size;
    return ptr;
}

void mavnat_arena_destroy(Arena *arena) {
    ArenaBlock *head = arena->head;
    while (head) {
        ArenaBlock *temp = head;
        head = head->next;
        free(temp);
    }
    free(arena);
}