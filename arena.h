#include <stddef.h>

typedef struct Buffer Buffer;

typedef struct {
    Buffer* buffer;
    size_t offset;
} Arena;

Arena arena_init(void);

void* arena_alloc(Arena* arena, size_t size, size_t alignment);
void arena_reset(Arena* arena);
