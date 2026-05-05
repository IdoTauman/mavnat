#pragma once

#include "../../arraylist/imp/arraylist.h"
#include <stdbool.h>

/**
 * In this library, a Stack is a functional alias for an ArrayList.
 * Operations are restricted to the "top" (end) of the list.
 */
typedef ArrayList Stack;

static inline Stack* mavnat_stack_init(Arena *arena, size_t initial_capacity);
static inline void   mavnat_stack_push(Stack *stack, void *item);
static inline void*  mavnat_stack_pop(Stack *stack);
static inline void*  mavnat_stack_peek(const Stack *stack);
static inline bool   mavnat_stack_is_empty(const Stack *stack);
static inline size_t mavnat_stack_size(const Stack *stack);