#include "search_seq.h"
#include "../imp/arraylist_internal.h"
#include <stdint.h>
#include <immintrin.h>
#include <assert.h>

#define mavnat_unlikely(x) __builtin_expect(!!(x), 0)
#define mavnat_likely(x)   __builtin_expect(!!(x), 1)

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

size_t mavnat_arraylist_linsearch_avx512(ArrayList *list, void *element) {
    size_t size = list->size;
    void **data = list->data;
    size_t i = 0;
    __m512i cmp = _mm512_set1_epi64((intptr_t)element);

    for (; i + 8 <= size; i += 8) {
        __m512i val = _mm512_load_si512((const void *)(data + i));
        __mmask8 res = _mm512_cmpeq_epi64_mask(cmp, val);

        if (mavnat_unlikely(res != 0)) {
            int index = __builtin_ctz(res);
            return i + index;
        }
    }

    // sequential tail search
    for (; i < size; i++) {
        if (mavnat_unlikely(data[i] == element)) return i;
    }

    return MAVNAT_NOT_FOUND;
}

size_t mavnat_arraylist_bsearch(ArrayList *list, void *element, mavnat_compare_fn cmp) {
    size_t l = 0, r = list->size;
    void **data = list->data;

    while (mavnat_likely(l < r)) {
        size_t m = l + ((r - l) >> 1);

        int res = cmp(data[m], element);

        if (mavnat_unlikely(res == 0)) {
            return m;
        } else if (res > 0) {
            r = m;
        } else {
            l = m + 1;
        }
    }
    return MAVNAT_NOT_FOUND;
}