#include "heapsort.h"

void mavnat_heapsort(ArrayList *list, mavnat_compare_fn cmp) {
    size_t n = list->size;
    if (mavnat_unlikely(n <= 1)) return;

    int flipped_cmp(const void *a, const void *b) {
        return cmp(b, a);
    }

    mavnat_heapify(list, flipped_cmp);
    void **data = list->data;
    // reverse iteration to save cmp instructions
    for (size_t i = n; i-- > 0;) {
        data[i] = mavnat_heap_pop((Heap*)list, flipped_cmp);
    }

    list->size = n;
}