#pragma once

#include "internal/allocator.hxx"

namespace smem {
struct pool_allocator : public allocator {
 public:
  pool_allocator(void* mem, size_t size, int obj_alig, size_t obj_size);
  ~pool_allocator();

  void* alloc(size_t size, int alig) override;
  void free(void* ptr) override;

  pool_allocator(const pool_allocator&) = delete;
  pool_allocator& operator=(const pool_allocator&) = delete;

 private:
  int obj_alig_;
  int obj_size_;
  int obj_num_;
  void** free_list_;
};
}  // namespace smem
