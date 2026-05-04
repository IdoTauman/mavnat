#include <stdio.h>
#include <assert.h>
#include <stdint.h>
#include "../../../src/util/arena.h"
#include "../../../src/arraylist/imp/arraylist.h"

// Helper to cast int to void*
#define AS_PTR(x) ((void*)(intptr_t)(x))
// Helper to cast void* back to int
#define AS_INT(x) ((int)(intptr_t)(x))

void test_initialization(Arena *arena) {
    ArrayList *list = mavnat_arraylist_init(arena, 4);
    
    assert(mavnat_arraylist_size(list) == 0);
    assert(mavnat_arraylist_capacity(list) == 4);
    
    printf("[PASS] Initialization\n");
}

void test_push_and_get(Arena *arena) {
    ArrayList *list = mavnat_arraylist_init(arena, 4);
    
    mavnat_arraylist_push(list, AS_PTR(10));
    mavnat_arraylist_push(list, AS_PTR(20));
    mavnat_arraylist_push(list, AS_PTR(30));
    
    assert(mavnat_arraylist_size(list) == 3);
    assert(AS_INT(mavnat_arraylist_get(list, 0)) == 10);
    assert(AS_INT(mavnat_arraylist_get(list, 1)) == 20);
    assert(AS_INT(mavnat_arraylist_get(list, 2)) == 30);
    
    // REMOVED: Out of bounds test, since assert() will intentionally crash the program.
    
    printf("[PASS] Push and Get\n");
}

void test_dynamic_growth(Arena *arena) {
    ArrayList *list = mavnat_arraylist_init(arena, 2); 
    
    for (int i = 0; i < 5; i++) {
        mavnat_arraylist_push(list, AS_PTR(i * 100));
    }
    
    assert(mavnat_arraylist_size(list) == 5);
    assert(mavnat_arraylist_capacity(list) >= 5); 
    
    for (int i = 0; i < 5; i++) {
        assert(AS_INT(mavnat_arraylist_get(list, i)) == i * 100);
    }
    
    printf("[PASS] Dynamic Growth (Reallocation)\n");
}

void test_pop(Arena *arena) {
    ArrayList *list = mavnat_arraylist_init(arena, 4);
    
    mavnat_arraylist_push(list, AS_PTR(99));
    mavnat_arraylist_push(list, AS_PTR(88));
    
    assert(AS_INT(mavnat_arraylist_pop(list)) == 88);
    assert(mavnat_arraylist_size(list) == 1);
    
    assert(AS_INT(mavnat_arraylist_pop(list)) == 99);
    assert(mavnat_arraylist_size(list) == 0);
    
    // Popping an empty list is still safe and returns NULL
    assert(mavnat_arraylist_pop(list) == NULL);
    
    printf("[PASS] Pop\n");
}

void test_set_and_clear(Arena *arena) {
    ArrayList *list = mavnat_arraylist_init(arena, 4);
    
    mavnat_arraylist_push(list, AS_PTR(1));
    mavnat_arraylist_push(list, AS_PTR(2));
    
    mavnat_arraylist_set(list, 0, AS_PTR(999));
    assert(AS_INT(mavnat_arraylist_get(list, 0)) == 999);
    
    mavnat_arraylist_clear(list);
    assert(mavnat_arraylist_size(list) == 0);
    assert(mavnat_arraylist_capacity(list) == 4); 
    
    mavnat_arraylist_push(list, AS_PTR(5));
    assert(AS_INT(mavnat_arraylist_get(list, 0)) == 5);
    
    printf("[PASS] Set and Clear\n");
}

int main() {
    printf("Starting ArrayList Tests...\n");
    printf("---------------------------\n");
    
    Arena *test_arena = mavnat_arena_init(128); 
    
    test_initialization(test_arena);
    test_push_and_get(test_arena);
    test_dynamic_growth(test_arena);
    test_pop(test_arena);
    test_set_and_clear(test_arena);
    
    mavnat_arena_destroy(test_arena);
    
    printf("---------------------------\n");
    printf("ALL TESTS PASSED SUCCESSFULLY.\n");
    
    return 0;
}