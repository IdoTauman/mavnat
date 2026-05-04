#include "sort_seq.h"
#include "../imp/arraylist_internal.h"
#include <assert.h>

#define INSERTION_THRESHOLD 16

#define mavnat_unlikely(x) __builtin_expect(!!(x), 0)
#define mavnat_likely(x)   __builtin_expect(!!(x), 1)

static void _mavnat_insertionsort_internal(void **data, size_t low, size_t high, mavnat_compare_fn cmp) {
    if (low >= high) return;

    for (size_t i = low + 1; i <= high; i++) {
        void *key = data[i];
        size_t j = i;

        while (j > low && mavnat_unlikely(cmp(data[j - 1], key) > 0)) {
            data[j] = data[j - 1];
            j--;
        }
        data[j] = key;
    }
}

void mavnat_arraylist_insertionsort(ArrayList *list, mavnat_compare_fn cmp) {
    assert(list != NULL && cmp != NULL);
    _mavnat_insertionsort_internal(list->data, 0, list->size - 1, cmp);
}

static void* _mavnat_median_of_three(void **data, size_t low, size_t high, mavnat_compare_fn cmp) {
    size_t mid = low + ((high - low) >> 1);

    if (cmp(data[low], data[mid]) > 0)  MAVNAT_SWAP_PTRS(data[low], data[mid]);
    if (cmp(data[low], data[high]) > 0) MAVNAT_SWAP_PTRS(data[low], data[high]);
    if (cmp(data[mid], data[high]) > 0) MAVNAT_SWAP_PTRS(data[mid], data[high]);

    return data[mid];
}

static size_t _mavnat_partition(void **data, size_t low, size_t high, mavnat_compare_fn cmp) {
    void *pivot = _mavnat_median_of_three(data, low, high, cmp);

    size_t i = low;
    size_t j = high;

    while (1) {
        while (cmp(data[i], pivot) < 0) i++;
        while (cmp(data[j], pivot) > 0) j--;

        if (i >= j) return j;

        MAVNAT_SWAP_PTRS(data[i], data[j]);

        i++;
        j--;
    }
}

static void _mavnat_quicksort_internal(void **data, size_t low, size_t high, mavnat_compare_fn cmp) {
    // use insertion sort for small chunks
    if (high - low < INSERTION_THRESHOLD) {
        _mavnat_insertionsort_internal(data, low, high, cmp);
        return;
    }

    if (low < high) {
        size_t p = _mavnat_partition(data, low, high, cmp);

        _mavnat_quicksort_internal(data, low, p, cmp);
        _mavnat_quicksort_internal(data, p + 1, high, cmp);
    }
}

void mavnat_arraylist_sort(ArrayList *list, mavnat_compare_fn cmp) {
    assert(list != NULL && cmp != NULL);
    _mavnat_quicksort_internal(list->data, 0, list->size - 1, cmp);
}
