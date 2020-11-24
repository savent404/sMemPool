#include "internal/allocator.hxx"
#include "internal/utils.hxx"

namespace smem {

allocator::allocator(void* base, size_t size)
    : base_(base), size_(size), used_memory_(0), num_allocations_(0) {}
allocator::allocator(allocator&& m)
    : base_(m.base_),
      size_(m.size_),
      used_memory_(m.used_memory_),
      num_allocations_(m.num_allocations_) {
  debug::_assert(!m.get_num_allocations() && !m.get_used_memory());
  m.base_ = nullptr;
  m.size_ = 0;
  m.used_memory_ = 0;
  m.num_allocations_ = 0;
}
allocator::~allocator() {
  debug::_assert(!get_num_allocations() && !get_used_memory());
}
}  // namespace smem
