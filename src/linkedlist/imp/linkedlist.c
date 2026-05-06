#include "linkedlist.h"

typedef struct ListNode {
    void *data;
    struct ListNode *prev;
    struct ListNode *next;
};

ListNode *mavnat_listnode_init(Arena *arena, void *data, size_t data_align) {
    ListNode *node = mavnat_arena_alloc(arena, sizeof(ListNode));
    node->data = mavnat_arena_alloc_aligned(arena, data_align, data_align);
    memcpy(node->data, data);
    node->prev = NULL;
    node->next = NULL;
    return node;
}

inline void *mavnat_listnode_get_data(ListNode *node) {
    return node->data;
}

inline ListNode *mavnat_listnode_get_next(ListNode *node) {
    return node->next;
}

inline void mavnat_listnode_set_next(ListNode *node, ListNode *next) {
    node->next = next;
}

inline ListNode *mavnat_listnode_get_prev(ListNode *node) {
    return node->prev;
}

inline void mavnat_listnode_set_prev(ListNode *node, ListNode *prev) {
    node->prev = prev;
}

inline bool mavnat_listnode_has_prev(ListNode *node) {
    return node->prev;
}

inline bool mavnat_listnode_has_next(ListNode *node) {
    return node->next;
}
