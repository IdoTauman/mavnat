#include "search_seq.h"
#include "../imp/arraylist_internal.h"
#include <stdint.h>
#include <immintrin.h>
#include <assert.h>

#define mavnat_unlikely(x) __builtin_expect(!!(x), 0)

size_t mavnat_arraylist_linsearch(ArrayList *list, void *element, mavnat_compare_fn cmp) {
    assert(cmp != NULL && "mavnat_arraylist_linsearch requires a valid comparator");
    const size_t size = list->size;
    for (size_t i = 0; i < size; i++) {
        void *curr = list->data[i];
        if (mavnat_unlikely(cmp(curr, element) == 0)) {
            return i;
        }
    }
    return MAVNAT_NOT_FOUND;
}

size_t mavnat_arraylist_linsearch_avx512(ArrayList *list, void *element, mavnat_compare_fn cmp) {
}

size_t mavnat_arraylist_bsearch(ArrayList *list, void *element, mavnat_compare_fn cmp) {
}