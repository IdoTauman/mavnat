#include "arena.h"
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define DEFAULT_ALIGN 8

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

void *mavnat_arena_alloc_aligned(Arena *arena, size_t size, size_t align) {
    // ensure a block exists
    if (!(arena->head)) {
        size_t block_size = size > arena->block_size ? size : arena->block_size;
        arena->head = mavnat_new_block(block_size + align, NULL);
        arena->tail = arena->head;
    }

    // calculate padding
    uintptr_t curr_ptr = (uintptr_t)arena->tail->data + arena->tail->used;
    uintptr_t aligned_ptr = (curr_ptr + align - 1) & ~(align - 1);
    size_t padding = aligned_ptr - curr_ptr;

    // step forward if the allocation doesnt fit
    if (arena->tail->used + padding + size > arena->tail->capacity) {
        size_t block_size = size > arena->block_size ? size : arena->block_size;
        
        // check for a reusable block
        if (arena->tail->next && arena->tail->next->capacity >= size + align) {
            arena->tail = arena->tail->next;
        } else {
            // free orphaned small blocks
            ArenaBlock *drop = arena->tail->next;
            while (drop) {
                ArenaBlock *temp = drop;
                drop = drop->next;
                free(temp);
            }
            arena->tail->next = mavnat_new_block(block_size + align, arena->tail);
            arena->tail = arena->tail->next;
        }

        // change tail pointers
        curr_ptr = (uintptr_t)arena->tail->data + arena->tail->used;
        aligned_ptr = (curr_ptr + align - 1) & ~(align - 1);
        padding = aligned_ptr - curr_ptr;
    }

    // update usage
    arena->tail->used += padding + size;
    return (void*)aligned_ptr;
}

void *mavnat_arena_alloc(Arena *arena, size_t size) {
    return mavnat_arena_alloc_aligned(arena, size, DEFAULT_ALIGN);
}

void mavnat_arena_reset(Arena *arena) {
    if (!arena->head) return;

    ArenaBlock *curr = arena->head;
    while (curr) {
        curr->used = 0;
        curr = curr->next;
    }
    // move tail back to the start
    arena->tail = arena->head;
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