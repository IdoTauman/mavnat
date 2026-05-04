#ifndef MAVNAT_ARRAYLIST_INTERNAL_H
#define MAVNAT_ARRAYLIST_INTERNAL_H

#include "arraylist.h"

/* 
 * The physical memory layout of the ArrayList.
 * This file should NEVER be installed or distributed to the end-user.
 * It is strictly for internal library use to allow direct memory 
 * access for SIMD and high-performance algorithms.
 */
struct ArrayList {
    void **data;      // 64-byte aligned for AVX-512
    size_t capacity;
    size_t size;
    Arena *arena;
};

#endif // MAVNAT_ARRAYLIST_INTERNAL_H