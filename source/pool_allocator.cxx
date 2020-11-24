#include "internal/pool_allocator.hxx"
#include <assert.h>
#include "internal/utils.hxx"

namespace smem {

pool_allocator::pool_allocator(void* mem, size_t size, int obj_alig,
                               size_t obj_size)
    : allocator(mem, size),
      obj_alig_(0),
      obj_size_(0),
      obj_num_(0),
      free_list_(nullptr) {
  debug::_assert(obj_size_ >= sizeof(void*));
  debug::_assert(obj_alig >= obj_size);
  int adj = math::align_forward_adjust(mem, obj_alig);
  free_list_ = reinterpret_cast<void**>(math::add(mem, adj));
  obj_alig_ = obj_alig;
  obj_size_ = obj_size;
  obj_num_ = (size - adj) / obj_alig_;
  void** p = free_list_;

  for (size_t i = 0; i < obj_num_ - 1; i++) {
    *p = math::add(reinterpret_cast<void*>(p), obj_alig_);
    p = reinterpret_cast<void**>(*p);
  }
  *p = nullptr;
}

pool_allocator::~pool_allocator() { free_list_ = nullptr; }

void* pool_allocator::alloc(size_t size, int align) {
  debug::_assert(size + sizeof(void*) <= obj_size_);
  debug::_assert(align == obj_alig_);

  void* p = free_list_;
  if (!p) return nullptr;
  free_list_ = reinterpret_cast<void**>(*free_list_);
  used_memory_ += obj_size_;
  num_allocations_++;
  return p;
}

void pool_allocator::free(void* p) {
    *reinterpret_cast<void**>(p) = free_list_;
    free_list_ = reinterpret_cast<void**>(p);
    used_memory_ -= obj_size_;
    num_allocations_--;
}
}  // namespace smem
