#ifndef MEMORY_ALLOCATORS__ARENA_ALLOCATOR_H
#define MEMORY_ALLOCATORS__ARENA_ALLOCATOR_H

/// @brief Arena Allocator Struct
typedef struct arena arena;

void arena_init(arena* allocator);

void* arena_alloc(arena* allocator, size_t memory_size);

void arena_increase_memory(arena* allocator);
void arena_resize_to(arena* allocator, size_t memory_size);

void arena_free(arena* allocator);

#endif
