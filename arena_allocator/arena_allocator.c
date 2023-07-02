#include "arena_allocator.h"
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

struct arena {
  unsigned char* memory;
  size_t memory_length;
  size_t offset;
  size_t prev_offset;
};

#define DEFAULT_ARENA_INIT_SIZE 1024*1024
#define DEFAULT_MEMORY_ALIGNMENT (2*sizeof(void*))

void arena_init(arena* allocator) {
  allocator->memory = (unsigned char*)malloc(DEFAULT_ARENA_INIT_SIZE);
  allocator->memory_length = DEFAULT_ARENA_INIT_SIZE;
  allocator->offset = 0;
  allocator->prev_offset = 0;
}

bool is_power_of_two(uintptr_t x) {
	return (x & (x-1)) == 0;
}

uintptr_t align_forward(uintptr_t ptr, size_t align) {
  uintptr_t p, a, modulo;

  if (!is_power_of_two(align)) {
    return NULL;
  }

  p = ptr;
	a = (uintptr_t)align;
	// Same as (p % a) but faster as 'a' is a power of two
	modulo = p & (a-1);

	if (modulo != 0) {
		// If 'p' address is not aligned, push the address to the
		// next value which is aligned
		p += a - modulo;
	}

	return p;
}

void* arena_alloc_align(arena* allocator, size_t memory_size, size_t align) {
  // Align offset forward to specified alignment
  uintptr_t ptr = (uintptr_t)allocator->memory + (uintptr_t)allocator->offset;
  uintptr_t align_offset = align_forward(ptr, align);
  // Change to relative offset
  align_offset -= (uintptr_t)allocator->memory;

  // Check to see if memory has enough space
  // if memory doesn't have enough space
  if (align_offset + memory_size > allocator->memory_length) {
    return NULL;
  }

  void* allocation_ptr = &allocator->memory[align_offset];
  allocator->prev_offset = align_offset;
  allocator->offset = align_offset + memory_size;

  // set memory to 0
  memset(allocation_ptr, 0, memory_size);
  return allocation_ptr;
}

void* arena_alloc(arena* allocator, size_t memory_size) {
  return arena_alloc_align(allocator, memory_size, DEFAULT_MEMORY_ALIGNMENT);
}