#pragma once

#include "heap.h"
#include "../../arraylist/imp/arraylist_internal.h"

/**
 * Binary implementation: Heap is a wrapper around the ArrayList.
 */
struct Heap {
    ArrayList list;
};

typedef Heap BinaryHeap;

/* --- Binary Index Math (Inlined) --- */
static inline size_t mavnat_bin_heap_parent(size_t i) { return (i - 1) >> 1; }
static inline size_t mavnat_bin_heap_left(size_t i)   { return (i << 1) + 1; }
static inline size_t mavnat_bin_heap_right(size_t i)  { return (i << 1) + 2; }

/* --- Inlined Accessors --- */
static inline void *mavnat_heap_peek(const Heap *heap) {
    return (heap->list.size > 0) ? heap->list.data[0] : NULL;
}

static inline size_t mavnat_heap_size(const Heap *heap) {
    return heap->list.size;
}

static inline bool mavnat_heap_is_empty(const Heap *heap) {
    return heap->list.size == 0;
}