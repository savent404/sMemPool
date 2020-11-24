#pragma once

#include "internal/allocator.hxx"

namespace smem {

struct proxy_allocator : public allocator {
 public:
  proxy_allocator(allocator* allocator);  // NOLINT
  ~proxy_allocator();
  void* alloc(size_t size, int alig) override;
  void free(void* p) override;

  proxy_allocator(const proxy_allocator&) = delete;
  proxy_allocator& operator=(const proxy_allocator&) = delete;

 private:
  allocator* allocator_;
};

}  // namespace smem
