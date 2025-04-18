#include "arena.h"
#include <stdalign.h>
#include <stdio.h>

int main(void) {
    Arena arena = arena_init();
    for (int i = 0; i < 10000; i++) {
        size_t *ptr = arena_alloc(&arena, sizeof(size_t), alignof(size_t));
        *ptr = 48;
        printf("%zu", *ptr);
    }
    arena_reset(&arena);
}
