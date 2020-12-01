#pragma once

#include "internal/allocator.hxx"

namespace smem {
struct slab_allocator : public allocator {
 public:
  slab_allocator(void* mem, size_t size, int obj_alig, size_t obj_size);
  ~slab_allocator();

  void* alloc(size_t size, int alig) override;
  void free(void* ptr) override;
  bool is_owner(void *p) override;
  size_t calculate_obj_size(size_t n) override;

  slab_allocator(const slab_allocator&) = delete;
  slab_allocator& operator=(const slab_allocator&) = delete;

 private:
  int obj_alig_;
  size_t obj_size_;
  int obj_num_;
  void** free_list_;
};
}  // namespace smem
