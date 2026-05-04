#ifndef MAVNAT_ARENA_H
#define MAVNAT_ARENA_H

#include <stddef.h>

/* Opaque handle */
typedef struct Arena Arena;

/**
 * Creates a new arena.
 * @param block_size The size of each memory block (e.g., 4096).
 *                   If a single allocation exceeds this, the arena 
 *                   should handle it by allocating a larger custom block.
 */
Arena *mavnat_arena_init(size_t block_size);

/**
 * Allocates memory from the arena.
 */
void *mavnat_arena_alloc(Arena *arena, size_t size);

/**
 * Allocates memory with a specific alignment requirement.
 * Non power of 2 alignment is undefined behavior.
 */
void *mavnat_arena_alloc_aligned(Arena *arena, size_t size, size_t align);

/**
 * Clears the arena for reuse. 
 * Keeps the memory blocks allocated but overwrites data.
 */
void mavnat_arena_reset(Arena *arena);

/**
 * Deallocates all memory blocks and the arena itself.
 */
void mavnat_arena_destroy(Arena *arena);

#endif