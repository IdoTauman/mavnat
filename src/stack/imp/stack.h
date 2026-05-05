#pragma once

#include "../../arraylist/imp/arraylist.h"
#include <stdbool.h>
#include <assert.h>

/**
 * In this library, a Stack is a functional alias for an ArrayList.
 * Operations are restricted to the "top" (end) of the list.
 */
typedef ArrayList Stack;

#define mavnat_unlikely(x) __builtin_expect(!!(x), 0)

static inline Stack* mavnat_stack_init(Arena *arena, size_t initial_capacity) {
    return mavnat_arraylist_init(arena, initial_capacity);
}

static inline void mavnat_stack_push(Stack *stack, void *item) {
    mavnat_arraylist_push((ArrayList*)stack, item);
}

static inline void* mavnat_stack_pop(Stack *stack) {
    return mavnat_arraylist_pop((ArrayList*)stack);
}

static inline void* mavnat_stack_peek(const Stack *stack) {
    size_t size = stack->size;
    if (mavnat_unlikely(size == 0)) return NULL;
    void **data = stack->data;
    return data[size - 1];
}

static inline bool mavnat_stack_is_empty(const Stack *stack) {
    return stack->size == 0;
}

static inline size_t mavnat_stack_size(const Stack *stack) {
    return stack->size;
}