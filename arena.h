#include <stddef.h>

typedef struct Buffer Buffer;

typedef struct {
    Buffer* buffer;
    size_t offset;
} Arena;

// Zero-initialize the arena.
//
// Initializing the arena does not allocate any memory.
Arena arena_init(void);

// Allocate memory in the arena.
void* arena_alloc(Arena* arena, size_t size, size_t alignment);

// Free all allocations in the arena.
void arena_reset(Arena* arena);
