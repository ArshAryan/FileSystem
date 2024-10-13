#ifndef BITMAP_H
#define BITMAP_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    unsigned int *bits;
    int size;
} bitmap_t;

static inline int bitmap_is_set(unsigned int *bits, int i) {
    return bits[i / 32] & (1 << (31 - (i % 32)));
}

static inline void bitmap_set(unsigned int *bits, int i) {
    bits[i / 32] |= (1 << (31 - (i % 32)));
}

static inline void bitmap_clear(unsigned int *bits, int i) {
    bits[i / 32] &= ~(1 << (31 - (i % 32)));
}

#endif