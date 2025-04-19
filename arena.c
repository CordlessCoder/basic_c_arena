#include "arena.h"
#include <stdlib.h>

#define BUF_CAPACITY 4096

typedef struct Buffer {
    size_t capacity;
    struct Buffer* next;
} Buffer;

Buffer *new_buffer(size_t capacity) {
    Buffer *ptr = malloc((capacity + sizeof(Buffer) * 2 - 1) / sizeof(Buffer) * sizeof(Buffer));
    Buffer buf = {capacity, 0};
    *ptr = buf;
    return ptr;
}

void *buffer_inner(Buffer *buf) {
    return buf + 1;
}


Arena arena_init(void) {
    Arena arena = {0};
    return arena;
}

void *arena_alloc(Arena* arena, size_t size, size_t alignment) {
    if (!arena->buffer) {
        size_t bsize = size;
        if (size < BUF_CAPACITY) {
            bsize = BUF_CAPACITY;
        }
        arena->buffer = new_buffer(bsize);
        arena->offset = 0;
    }
    // Handle allocations that can't fit in a buffer
    if (size > BUF_CAPACITY && (arena->buffer->capacity - arena->offset) != size) {
        Buffer *new = new_buffer(size);
        new->next = arena->buffer->next;
        arena->buffer->next = new;
        return buffer_inner(new);
    }
    size_t start = (size_t)buffer_inner(arena->buffer) + arena->offset;
    size_t end = (size_t)buffer_inner(arena->buffer) + arena->buffer->capacity;
    size_t available = end - (start + alignment - 1) / alignment * alignment;
    if (available < size) {
        size_t bsize = size;
        if (size < BUF_CAPACITY) {
            bsize = BUF_CAPACITY;
        }
        Buffer *new = new_buffer(bsize);
        new->next = arena->buffer;
        arena->buffer = new;
        arena->offset = 0;
    }
    start = (size_t)buffer_inner(arena->buffer) + arena->offset;
    arena->offset += (start + alignment - 1) / alignment * alignment - start;
    void *ptr = (char *)buffer_inner(arena->buffer) + arena->offset;
    arena->offset += size;
    return ptr;
}
void arena_reset(Arena* arena) {
    arena->offset = 0;
    while (arena->buffer) {
        Buffer *old = arena->buffer;
        arena->buffer = old->next;
        free(old);
    }
}
