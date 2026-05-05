#include "binaryheap.h"
#include <assert.h>

#define mavnat_likely(x)   __builtin_expect(!!(x), 1)
#define mavnat_unlikely(x) __builtin_expect(!!(x), 0)

#define MAVNAT_SWAP(a, b) do { \
    void *tmp = (a);           \
    (a) = (b);                 \
    (b) = tmp;                 \
} while(0)

static void _mavnat_bin_heap_sift_up(ArrayList *list, size_t index, mavnat_compare_fn cmp) {
    if (mavnat_unlikely(index == 0)) return;
    void **data = list->data;
    // step up is likelier than exiting (E(steps) ≈ 1.6 > 1) assuming uniform order statistics
    while (mavnat_likely(index > 0) && mavnat_likely(cmp(data[index], data[mavnat_bin_heap_parent(index)]) < 0)) {
        MAVNAT_SWAP(data[index], data[mavnat_bin_heap_parent(index)]);
        index = mavnat_bin_heap_parent(index);
    }
}

static void _mavnat_bin_heap_sift_down(ArrayList *list, size_t index, mavnat_compare_fn cmp) {
    const size_t size = list->size;
    void **data = list->data;

    // step down is likelier than exiting (E(steps) ≈ h - 2.6) assuming uniform order statistics
    while (mavnat_likely(mavnat_bin_heap_left(index) < size)) {
        size_t left = mavnat_bin_heap_left(index);
        size_t right = mavnat_bin_heap_right(index);
        size_t best = left;

        if (right < size && cmp(data[right], data[left]) < 0) {
            best = right;
        }

        if (mavnat_unlikely(cmp(data[best], data[index]) >= 0)) break;

        MAVNAT_SWAP(data[index], data[best]);
        index = best;
    }
}

/* --- API Implementation --- */

Heap* mavnat_heap_init(Arena *arena, size_t initial_capacity) {
    Heap *heap = mavnat_arena_alloc(arena, sizeof(Heap));

    ArrayList *inner = mavnat_arraylist_init(arena, initial_capacity);
    heap->list = *inner;

    return heap;
}

void mavnat_heap_push(Heap *heap, void *item, mavnat_compare_fn cmp) {
    assert(heap != NULL && cmp != NULL);
    mavnat_arraylist_push(&heap->list, item);
    _mavnat_bin_heap_sift_up(&heap->list, heap->list.size - 1, cmp);
}

void* mavnat_heap_pop(Heap *heap, mavnat_compare_fn cmp) {
    assert(heap != NULL && cmp != NULL);
    void **data = heap->list.data;
    MAVNAT_SWAP(data[0], data[heap->list.size - 1]);
    void *val = mavnat_arraylist_pop(&heap->list);
    _mavnat_bin_heap_sift_down(&heap->list, 0, cmp);
    return val;
}

Heap* mavnat_heapify(ArrayList *arr, mavnat_compare_fn cmp) {
    assert(arr != NULL && cmp != NULL);

    // treat the existing ArrayList as the list member of a Heap
    Heap *heap = (Heap*)arr; 

    if (arr->size > 1) {
        for (size_t i = (arr->size / 2); i > 0; i--) {
            _mavnat_bin_heap_sift_down(arr, i - 1, cmp);
        }
    }
    return heap;
}