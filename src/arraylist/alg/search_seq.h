#ifndef MAVNAT_ARRAYLIST_SEARCH_SEQ_H
#define MAVNAT_ARRAYLIST_SEARCH_SEQ_H

#include "../imp/arraylist.h"

#define MAVNAT_NOT_FOUND ((size_t)-1)

/**
 * Performs a linear search over the arraylist.
 * Time complexity: O(n)
 * Space complexity: O(1)
 */
size_t mavnat_arraylist_linsearch(ArrayList *list, void *element, mavnat_compare_fn cmp);

/**
 * Performs a linear search over the arraylist with AVX512.
 * Note: this is much faster than regular linear search.
 * Time complexity: O(n)
 * Space complexity: O(1)
 */
size_t mavnat_arraylist_linsearch_avx512(ArrayList *list, void *element, mavnat_compare_fn cmp);

/**
 * Performs a binary search over the arraylist.
 * Warning: passing a non sorted (non decreasing order) list will result in undefined behavior.
 * Time complexity: O(logn)
 * Space complexity: O(1)
 */
size_t mavnat_arraylist_bsearch(ArrayList *list, void *element, mavnat_compare_fn cmp);

#endif // MAVNAT_ARRAYLIST_SEARCH_SEQ_H