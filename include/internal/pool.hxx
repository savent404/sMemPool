#pragma once

#include <smempool.h>

namespace smem {

struct pool {
 public:
  void* base_addr;
  pool* allocator;  // nullptr means alloced by general allocator
};

}  // namespace smem
