#pragma once

#include "internal/allocator.hxx"

namespace smem {
struct list_allocator : public allocator {
 public:
  list_allocator(void* base, size_t size);
  ~list_allocator();

  void* alloc(size_t size, int align) override;
  void free(void* p) override;

  list_allocator(const list_allocator&) = delete;
  list_allocator& operator=(const list_allocator&) = delete;

 private:
  struct header {
    size_t size;
    int adj;
  };
  struct block {
    size_t size;
    block* next;
  };
  block* free_blocks_;
};
}  // namespace smem
