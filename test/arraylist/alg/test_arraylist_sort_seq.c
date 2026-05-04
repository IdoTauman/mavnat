#include <stdio.h>
#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include "../../../src/util/arena.h"
#include "../../../src/arraylist/imp/arraylist.h"
#include "../../../src/arraylist/alg/sort_seq.h"

// Helper macros for handling integers as pointers
#define AS_PTR(x) ((void*)(intptr_t)(x))
#define AS_INT(x) ((int)(intptr_t)(x))

/**
 * Comparator for integer values packed into void pointers.
 */
int int_ptr_cmp(const void *a, const void *b) {
    intptr_t val_a = (intptr_t)a;
    intptr_t val_b = (intptr_t)b;
    if (val_a < val_b) return -1;
    if (val_a > val_b) return 1;
    return 0;
}

/**
 * Validates that the list is in non-decreasing order.
 */
void assert_is_sorted(ArrayList *list, mavnat_compare_fn cmp) {
    size_t size = mavnat_arraylist_size(list);
    if (size <= 1) return;
    
    for (size_t i = 0; i < size - 1; i++) {
        void *current = mavnat_arraylist_get(list, i);
        void *next = mavnat_arraylist_get(list, i + 1);
        assert(cmp(current, next) <= 0 && "Sort order validation failed!");
    }
}

void test_edge_cases(Arena *arena) {
    // 1. Empty List
    ArrayList *empty = mavnat_arraylist_init(arena, 8);
    mavnat_arraylist_sort(empty, int_ptr_cmp);
    assert(mavnat_arraylist_size(empty) == 0);

    // 2. Single Element
    ArrayList *single = mavnat_arraylist_init(arena, 8);
    mavnat_arraylist_push(single, AS_PTR(123));
    mavnat_arraylist_sort(single, int_ptr_cmp);
    assert(AS_INT(mavnat_arraylist_get(single, 0)) == 123);

    printf("[PASS] Edge Cases (Empty/Single)\n");
}

void test_insertion_sort_explicit(Arena *arena) {
    ArrayList *list = mavnat_arraylist_init(arena, 16);
    int input[] = {40, 10, 50, 20, 30, 10}; // Includes duplicates
    
    for (int i = 0; i < 6; i++) {
        mavnat_arraylist_push(list, AS_PTR(input[i]));
    }
    
    // Test the public insertion sort specifically
    mavnat_arraylist_insertionsort(list, int_ptr_cmp);
    assert_is_sorted(list, int_ptr_cmp);
    
    printf("[PASS] Insertion Sort (Small Set/Duplicates)\n");
}

void test_sort_large_random(Arena *arena) {
    const int count = 5000;
    ArrayList *list = mavnat_arraylist_init(arena, count);
    
    srand(time(NULL));
    for (int i = 0; i < count; i++) {
        mavnat_arraylist_push(list, AS_PTR(rand() % 10000));
    }
    
    // Test the hybrid sort (QuickSort + Threshold)
    mavnat_arraylist_sort(list, int_ptr_cmp);
    assert_is_sorted(list, int_ptr_cmp);
    
    printf("[PASS] Hybrid Sort (Random: %d items)\n", count);
}

void test_pathological_patterns(Arena *arena) {
    // 1. Already Sorted
    ArrayList *sorted = mavnat_arraylist_init(arena, 128);
    for (int i = 0; i < 100; i++) mavnat_arraylist_push(sorted, AS_PTR(i));
    mavnat_arraylist_sort(sorted, int_ptr_cmp);
    assert_is_sorted(sorted, int_ptr_cmp);

    // 2. Reverse Sorted
    ArrayList *reverse = mavnat_arraylist_init(arena, 128);
    for (int i = 100; i > 0; i--) mavnat_arraylist_push(reverse, AS_PTR(i));
    mavnat_arraylist_sort(reverse, int_ptr_cmp);
    assert_is_sorted(reverse, int_ptr_cmp);

    printf("[PASS] Pathological Patterns (Sorted/Reverse)\n");
}

int main() {
    printf("Running Sequential Sort Tests...\n");
    printf("--------------------------------\n");

    // Initialize the arena for the test session
    Arena *test_arena = mavnat_arena_init(1024 * 1024); 

    test_edge_cases(test_arena);
    test_insertion_sort_explicit(test_arena);
    test_sort_large_random(test_arena);
    test_pathological_patterns(test_arena);

    // Clean up memory
    mavnat_arena_destroy(test_arena);

    printf("--------------------------------\n");
    printf("ALL SORT TESTS PASSED.\n");
    return 0;
}