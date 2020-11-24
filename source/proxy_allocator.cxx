#include "internal/proxy_allocator.hxx"
#include "internal/utils.hxx"

namespace smem {

proxy_allocator::proxy_allocator(allocator* m)
    : allocator(m->get_base(), m->get_size()), allocator_(m) {}

void* proxy_allocator::alloc(size_t size, int align) {
  debug::_assert(size != 0);
  auto res = allocator_->alloc(size, align);
  num_allocations_ = allocator_->get_num_allocations();
  auto mem = allocator_->get_used_memory();
  used_memory_ += allocator_->get_used_memory() - mem;
  return res;
}

void proxy_allocator::free(void* p) {
  allocator_->free(p);
  num_allocations_ = allocator_->get_num_allocations();
  auto mem = allocator_->get_used_memory();
  used_memory_ += allocator_->get_used_memory() - mem;
}
}  // namespace smem
