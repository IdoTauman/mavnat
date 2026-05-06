#pragma once

#include "../../util/arena.h"

#include <stdbool.h>

/* Opaque handle */
typedef struct ListNode ListNode;

/* Creates a new ListNode struct */
ListNode *mavnat_listnode_init(Arena *arena, void *data, size_t data_align);

/* Gets the data pointer from a ListNode struct */
void *mavnat_listnode_get_data(ListNode *node);

/* Gets the next ListNode struct */
ListNode *mavnat_listnode_get_next(ListNode *node);

/* Sets the next ListNode struct */
void mavnat_listnode_set_next(ListNode *node, ListNode *next);

/* Gets the previous ListNode struct */
ListNode *mavnat_listnode_get_prev(ListNode *node);

/* Sets the previous ListNode struct */
void mavnat_listnode_set_prev(ListNode *node, ListNode *prev);

/* Returns if a previous node exists */
bool mavnat_listnode_has_prev(ListNode *node);

/* Returns if a next node exists */
bool mavnat_listnode_has_next(ListNode *node);
