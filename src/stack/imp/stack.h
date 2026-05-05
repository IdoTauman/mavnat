#pragma once

#include "../../arraylist/imp/arraylist.h"

/**
 * In this library, a Stack is a functional alias for an ArrayList.
 * Operations are restricted to the "top" (end) of the list.
 */
typedef ArrayList Stack;

Stack* mavnat_stack_init(Arena *arena, size_t initial_capacity);
void   mavnat_stack_push(Stack *stack, void *item);
void*  mavnat_stack_pop(Stack *stack);
void*  mavnat_stack_peek(const Stack *stack);
bool   mavnat_stack_is_empty(const Stack *stack);
size_t mavnat_stack_size(const Stack *stack);