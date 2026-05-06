// copilot, write tests for the linked list implementation in the same format as the array list tests. make sure to cover edge cases and typical use cases. use the arena for memory management. do you understand?
// yes, I understand. I will write tests for the linked list implementation covering edge cases and

#include "../../../src/util/arena.h"
#include "../../../src/linkedlist/imp/linkedlist.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

void main() {
    // Initialize the arena for memory management
    Arena *arena = mavnat_arena_init(1024);

    // Test 1: Create a linked list and add nodes
    ListNode *node1 = mavnat_listnode_init(arena, "Node 1", sizeof(void *));
    ListNode *node2 = mavnat_listnode_init(arena, "Node 2", sizeof(void *));
    ListNode *node3 = mavnat_listnode_init(arena, "Node 3", sizeof(void *));
    mavnat_listnode_set_next(node1, node2);
    mavnat_listnode_set_prev(node2, node1);
    mavnat_listnode_set_next(node2, node3);
    mavnat_listnode_set_prev(node3, node2);

    // Test 2: Verify the links between nodes
    assert(mavnat_listnode_get_next(node1) == node2);
    assert(mavnat_listnode_get_prev(node2) == node1);
    assert(mavnat_listnode_get_next(node2) == node3);
    assert(mavnat_listnode_get_prev(node3) == node2);

    // Test 3: Verify data retrieval
    assert(strcmp(mavnat_listnode_get_data(node1), "Node 1") == 0);
    assert(strcmp(mavnat_listnode_get_data(node2), "Node 2") == 0);
    assert(strcmp(mavnat_listnode_get_data(node3), "Node 3") == 0);

    // Test 4: Edge case - Single node list
    ListNode *singleNode = mavnat_listnode_init(arena, "Single Node", sizeof(void *));
    assert(mavnat_listnode_get_next(singleNode) == NULL);
    assert(mavnat_listnode_get_prev(singleNode) == NULL);
    assert(strcmp(mavnat_listnode_get_data(singleNode), "Single Node") == 0);

    printf("[PASS] Linked List Node Creation and Linking\n");
    printf("[PASS] Linked List Data Retrieval\n");
    printf("[PASS] Linked List Single Node Edge Case\n");
    // Clean up the arena
    mavnat_arena_destroy(arena);
}
