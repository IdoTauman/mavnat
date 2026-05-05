#pragma once

#include "../imp/binaryheap.h"

#define mavnat_unlikely(x) __builtin_expect(!!(x), 0)

/**
 * Performs a heapsort on an ArrayList.
 * Time complexity: O(nlogn)
 * Space complexity: O(1)
 */
void mavnat_heapsort(ArrayList *list, mavnat_compare_fn cmp);
