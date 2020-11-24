#pragma once

#include <smempool.h>
#include <stddef.h>

namespace smem {

struct allocator {
 public:
  allocator(void* base, size_t size);
  allocator(allocator&& m);
  virtual ~allocator();

  virtual void* alloc(size_t size, int alig = sizeof(void*)) = 0;
  virtual void free(void* p) = 0;

  void* get_base() const { return base_; }
  size_t get_size() const { return size_; }
  size_t get_used_memory() const { return used_memory_; }
  size_t get_num_allocations() const { return num_allocations_; }
  size_t get_last_size() const { return size_ - used_memory_; }

 protected:
  void* base_;
  size_t size_;
  size_t used_memory_;
  size_t num_allocations_;
};
}  // namespace smem
