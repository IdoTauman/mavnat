#pragma once

#include "../../util/arena.h"
#include <stddef.h>
#include <stdbool.h>

typedef int (*mavnat_compare_fn)(const void *a, const void *b);

/* forward declaration, can be a BinaryHeap etc */
typedef struct Heap Heap;

/* general heap API */
Heap*  mavnat_heap_init(Arena *arena, size_t initial_capacity);
void   mavnat_heap_push(Heap *heap, void *item, mavnat_compare_fn cmp);
void*  mavnat_heap_pop(Heap *heap, mavnat_compare_fn cmp);
void*  mavnat_heap_peek(const Heap *heap);
size_t mavnat_heap_size(const Heap *heap);
bool   mavnat_heap_is_empty(const Heap *heap);

/**
 * Reorganizes an ArrayList to a binary heap.
 * @returns same pointer as heap casted to BinaryHeap
 */
void *mavnat_heapify(ArrayList *arr, mavnat_compare_fn cmp);