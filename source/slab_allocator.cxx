#include <assert.h>

#include "internal/slab_allocator.hxx"
#include "internal/utils.hxx"

namespace smem {

slab_allocator::slab_allocator(void* mem, size_t size, int obj_alig,
                               size_t obj_size)
    : allocator(mem, size),
      obj_alig_(obj_alig),
      obj_size_(obj_size),
      obj_num_(0),
      free_list_(nullptr) {
  debug::_assert(obj_size_ >= sizeof(void*));
  // debug::_assert(obj_alig >= obj_size);
  int adj = math::align_forward_adjust(mem, obj_alig);
  free_list_ = reinterpret_cast<void**>(math::add(mem, adj));
  obj_num_ = (size - adj) / obj_size_;
  void** p = free_list_;

  for (size_t i = 0; i < obj_num_ - 1; i++) {
    *p = math::add(reinterpret_cast<void*>(p), obj_size_);
    p = reinterpret_cast<void**>(*p);
  }
  *p = nullptr;
}

slab_allocator::~slab_allocator() { free_list_ = nullptr; }

void* slab_allocator::alloc(size_t size, int align) {
  debug::_assert(size <= obj_size_);
  debug::_assert(align == obj_alig_);

  void* p = free_list_;
  if (!p) return nullptr;
  free_list_ = reinterpret_cast<void**>(*free_list_);
  used_memory_ += obj_size_;
  num_allocations_++;
  return p;
}

void slab_allocator::free(void* p) {
  *reinterpret_cast<void**>(p) = free_list_;
  free_list_ = reinterpret_cast<void**>(p);
  used_memory_ -= obj_size_;
  num_allocations_--;
}

bool slab_allocator::is_owner(void* p) {
  void* base_begin = get_base();
  void* base_end = math::add(base_begin, get_size());
  return (math::sub(base_begin, p) <= 0) && (math::sub(p, base_end) <= 0);
}

size_t slab_allocator::calculate_obj_size(size_t n) {
  return n;
}
}  // namespace smem
