#ifndef MAVNAT_ARRAYLIST_SORT_SEQ_H
#define MAVNAT_ARRAYLIST_SORT_SEQ_H

#include "../imp/arraylist.h"

/**
 * Performs an in-place Insertion Sort on the arraylist.
 * Best for very small arrays (e.g., < 32 elements) or nearly sorted data.
 * Time complexity: O(n^2) worst case, O(n) best case.
 * Space complexity: O(1)
 * @param list The arraylist to sort
 * @param cmp  The comparison function to determine order
 */
void mavnat_arraylist_insertionsort(ArrayList *list, mavnat_compare_fn cmp);

/**
 * Performs a highly optimized, in-place sequential sort on the arraylist.
 * (Will be implemented as Introsort: QuickSort with a insertion sort fallback).
 * Time complexity: O(n log n) worst/average case.
 * Space complexity: O(log n) auxiliary stack space.
 * @param list The arraylist to sort
 * @param cmp  The comparison function to determine order
 */
void mavnat_arraylist_sort(ArrayList *list, mavnat_compare_fn cmp);

#endif // MAVNAT_ARRAYLIST_SORT_SEQ_H