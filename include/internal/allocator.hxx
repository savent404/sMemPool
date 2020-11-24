#pragma once

#include <smempool.h>
#include <stddef.h>

namespace smem {

template <typename _T>
struct allocator {
 public:
  explict allocator(smem_alloctor_t allocator);

  void* alloc(size_t n);
  void free(void* n);
};
}  // namespace smem
