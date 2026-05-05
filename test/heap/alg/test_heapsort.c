#include <stdio.h>
#include <assert.h>
#include <stdint.h>
#include "../../../src/util/arena.h"
#include "../../../src/arraylist/imp/arraylist.h"
#include "../../../src/heap/alg/heapsort.h"

#define AS_PTR(x) ((void*)(intptr_t)(x))
#define AS_INT(x) ((int)(intptr_t)(x))

int int_ptr_cmp(const void *a, const void *b) {
    intptr_t val_a = (intptr_t)a;
    intptr_t val_b = (intptr_t)b;
    if (val_a < val_b) return -1;
    if (val_a > val_b) return 1;
    return 0;
}

void test_heapsort_logic(Arena *arena) {
    ArrayList *list = mavnat_arraylist_init(arena, 10);
    int inputs[] = {70, 10, 50, 20, 40, 60, 30};
    size_t n = sizeof(inputs) / sizeof(inputs[0]);

    for (size_t i = 0; i < n; i++) {
        mavnat_arraylist_push(list, AS_PTR(inputs[i]));
    }

    // Perform in-place sort
    mavnat_heapsort(list, int_ptr_cmp);

    // Verify metadata restoration
    assert(mavnat_arraylist_size(list) == n);

    // Verify ascending order: [10, 20, 30, 40, 50, 60, 70]
    intptr_t last = -1;
    for (size_t i = 0; i < n; i++) {
        intptr_t current = (intptr_t)mavnat_arraylist_get(list, i);
        assert(current >= last);
        last = current;
    }

    printf("[PASS] Heapsort In-Place Ascending\n");
}

int main() {
    printf("Starting Heapsort Algorithm Tests...\n");
    printf("-------------------------------------\n");

    Arena *test_arena = mavnat_arena_init(2048);
    test_heapsort_logic(test_arena);
    mavnat_arena_destroy(test_arena);

    printf("-------------------------------------\n");
    printf("ALL HEAPSORT TESTS PASSED.\n");
    return 0;
}