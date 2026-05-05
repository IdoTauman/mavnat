#include <stdio.h>
#include <assert.h>
#include <stdint.h>
#include "../../../src/util/arena.h"
#include "../../../src/arraylist/imp/arraylist.h"
#include "../../../src/heap/imp/binaryheap.h"

#define AS_PTR(x) ((void*)(intptr_t)(x))
#define AS_INT(x) ((int)(intptr_t)(x))

int int_ptr_cmp(const void *a, const void *b) {
    intptr_t val_a = (intptr_t)a;
    intptr_t val_b = (intptr_t)b;
    if (val_a < val_b) return -1;
    if (val_a > val_b) return 1;
    return 0;
}

void test_heap_min_property(Arena *arena) {
    Heap *heap = mavnat_heap_init(arena, 8);

    // Test push and peek
    mavnat_heap_push(heap, AS_PTR(50), int_ptr_cmp);
    assert(AS_INT(mavnat_heap_peek(heap)) == 50);

    mavnat_heap_push(heap, AS_PTR(10), int_ptr_cmp);
    assert(AS_INT(mavnat_heap_peek(heap)) == 10); // 10 should sift up

    mavnat_heap_push(heap, AS_PTR(30), int_ptr_cmp);
    mavnat_heap_push(heap, AS_PTR(20), int_ptr_cmp);

    assert(mavnat_heap_size(heap) == 4);
    assert(AS_INT(mavnat_heap_peek(heap)) == 10);

    // Test extraction order (Pop)
    assert(AS_INT(mavnat_heap_pop(heap, int_ptr_cmp)) == 10);
    assert(AS_INT(mavnat_heap_pop(heap, int_ptr_cmp)) == 20);
    assert(AS_INT(mavnat_heap_pop(heap, int_ptr_cmp)) == 30);
    assert(AS_INT(mavnat_heap_pop(heap, int_ptr_cmp)) == 50);

    assert(mavnat_heap_is_empty(heap));
    printf("[PASS] Binary Min-Heap Push/Pop\n");
}

int main() {
    printf("Starting Heap Implementation Tests...\n");
    printf("-------------------------------------\n");

    Arena *test_arena = mavnat_arena_init(1024);
    test_heap_min_property(test_arena);
    mavnat_arena_destroy(test_arena);

    printf("-------------------------------------\n");
    printf("ALL HEAP TESTS PASSED.\n");
    return 0;
}