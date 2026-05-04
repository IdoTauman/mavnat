#include <stdio.h>
#include <assert.h>
#include <stdint.h>
#include "../../../src/util/arena.h"
#include "../../../src/arraylist/imp/arraylist.h"
#include "../../../src/arraylist/alg/search_seq.h"

// Helper macros for storing raw ints in void* pointers
#define AS_PTR(x) ((void*)(intptr_t)(x))
#define AS_INT(x) ((int)(intptr_t)(x))

/* 
 * Standard comparator function for our packed pointer-integers.
 * Returns: -1 if a < b, 1 if a > b, 0 if equal.
 */
int int_ptr_cmp(const void *a, const void *b) {
    intptr_t val_a = (intptr_t)a;
    intptr_t val_b = (intptr_t)b;
    
    if (val_a < val_b) return -1;
    if (val_a > val_b) return 1;
    return 0;
}

void test_linsearch(Arena *arena) {
    ArrayList *list = mavnat_arraylist_init(arena, 8);
    
    // Test empty list
    assert(mavnat_arraylist_linsearch(list, AS_PTR(10), int_ptr_cmp) == MAVNAT_NOT_FOUND);
    
    // Populate with unsorted data: [50, 10, 40, 20, 30]
    mavnat_arraylist_push(list, AS_PTR(50));
    mavnat_arraylist_push(list, AS_PTR(10));
    mavnat_arraylist_push(list, AS_PTR(40));
    mavnat_arraylist_push(list, AS_PTR(20));
    mavnat_arraylist_push(list, AS_PTR(30));
    
    // Test finding existing elements
    assert(mavnat_arraylist_linsearch(list, AS_PTR(50), int_ptr_cmp) == 0); // First
    assert(mavnat_arraylist_linsearch(list, AS_PTR(40), int_ptr_cmp) == 2); // Middle
    assert(mavnat_arraylist_linsearch(list, AS_PTR(30), int_ptr_cmp) == 4); // Last
    
    // Test missing element
    assert(mavnat_arraylist_linsearch(list, AS_PTR(99), int_ptr_cmp) == MAVNAT_NOT_FOUND);
    
    printf("[PASS] Standard Linear Search\n");
}

void test_linsearch_avx512(Arena *arena) {
    ArrayList *list = mavnat_arraylist_init(arena, 32);

    // Test empty list
    assert(mavnat_arraylist_linsearch_avx512(list, AS_PTR(99)) == MAVNAT_NOT_FOUND);

    // Populate with 20 items to create 2 full chunks (16) + 4 tail items
    for (int i = 0; i < 20; i++) {
        mavnat_arraylist_push(list, AS_PTR(i * 10)); // 0, 10, 20...
    }

    // Test Chunk 1 Boundaries (Indices 0 - 7)
    assert(mavnat_arraylist_linsearch_avx512(list, AS_PTR(0)) == 0);   
    assert(mavnat_arraylist_linsearch_avx512(list, AS_PTR(30)) == 3);  
    assert(mavnat_arraylist_linsearch_avx512(list, AS_PTR(70)) == 7);  

    // Test Chunk 2 Boundaries (Indices 8 - 15)
    assert(mavnat_arraylist_linsearch_avx512(list, AS_PTR(80)) == 8);  
    assert(mavnat_arraylist_linsearch_avx512(list, AS_PTR(150)) == 15); 

    // Test The Tail Loop (Indices 16 - 19)
    assert(mavnat_arraylist_linsearch_avx512(list, AS_PTR(160)) == 16); 
    assert(mavnat_arraylist_linsearch_avx512(list, AS_PTR(190)) == 19); 

    // Test Not Found
    assert(mavnat_arraylist_linsearch_avx512(list, AS_PTR(999)) == MAVNAT_NOT_FOUND);

    printf("[PASS] AVX-512 Linear Search\n");
}

void test_bsearch(Arena *arena) {
    ArrayList *list = mavnat_arraylist_init(arena, 16);
    
    // 1. Test empty list
    assert(mavnat_arraylist_bsearch(list, AS_PTR(10), int_ptr_cmp) == MAVNAT_NOT_FOUND);
    
    // 2. Populate with SORTED data: [10, 20, 30, 40, 50] (Odd number of elements)
    for (int i = 1; i <= 5; i++) {
        mavnat_arraylist_push(list, AS_PTR(i * 10));
    }
    
    // Test finding every single element
    for (size_t i = 0; i < mavnat_arraylist_size(list); i++) {
        void *target = mavnat_arraylist_get(list, i);
        assert(mavnat_arraylist_bsearch(list, target, int_ptr_cmp) == i);
    }
    
    // Test missing items (Out of bounds)
    assert(mavnat_arraylist_bsearch(list, AS_PTR(5), int_ptr_cmp) == MAVNAT_NOT_FOUND);  // Too small
    assert(mavnat_arraylist_bsearch(list, AS_PTR(99), int_ptr_cmp) == MAVNAT_NOT_FOUND); // Too large
    
    // Test missing items (In between bounds)
    assert(mavnat_arraylist_bsearch(list, AS_PTR(25), int_ptr_cmp) == MAVNAT_NOT_FOUND);
    
    // 3. Test Even number of elements
    mavnat_arraylist_push(list, AS_PTR(60)); // List is now length 6
    assert(mavnat_arraylist_bsearch(list, AS_PTR(60), int_ptr_cmp) == 5);
    
    printf("[PASS] Binary Search\n");
}

int main() {
    printf("Starting Search Algorithm Tests...\n");
    printf("----------------------------------\n");
    
    Arena *test_arena = mavnat_arena_init(256); 
    
    test_linsearch(test_arena);
    test_linsearch_avx512(test_arena);
    test_bsearch(test_arena);
    
    mavnat_arena_destroy(test_arena);
    
    printf("----------------------------------\n");
    printf("ALL TESTS PASSED SUCCESSFULLY.\n");
    
    return 0;
}