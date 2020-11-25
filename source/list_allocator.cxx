#include "internal/list_allocator.hxx"
#include "internal/utils.hxx"

namespace smem {

list_allocator::list_allocator(void *base, size_t size)
    : allocator(base, size), free_blocks_(nullptr) {
  static_assert(sizeof(header) >= sizeof(block));
  debug::_assert(size > sizeof(block));
  free_blocks_ = reinterpret_cast<block *>(base_);
  free_blocks_->size = size;
  free_blocks_->next = nullptr;
}

list_allocator::~list_allocator() { free_blocks_->next = nullptr; }

void *list_allocator::alloc(size_t size, int align) {
  debug::_assert(size != 0 && align != 0);

  // makre sure '*(int*)(malloc(1)) = 1' would not crash others
  size = math::align_size(size, sizeof(void *));

  block *prev = nullptr;
  block *free = free_blocks_;

  while (free != nullptr) {
    int adj =
        math::align_forwrad_adjust_with_header(free, align, sizeof(header));
    size_t body_size = size + adj;

    if (free->size < body_size) {
      prev = free;
      free = free->next;
      continue;
    }

    // give them this block
    if (free->size - body_size <= sizeof(header)) {
      body_size = free_blocks_->size;
      if (!prev)
        free_blocks_ = free->next;
      else
        prev->next = free->next;
    }
    // split this block and create a small one
    else {
      block *next = reinterpret_cast<block *>(math::add(free, body_size));
      next->size = free->size - body_size;
      next->next = free->next;
      if (!prev)
        free_blocks_ = next;
      else
        prev->next = next;
    }

    void *res = math::add(free, adj);
    header *h = reinterpret_cast<header *>(math::sub(res, sizeof(header)));
    h->size = body_size;
    h->adj = adj;
    used_memory_ += body_size;
    num_allocations_++;

    debug::_assert(math::align_forward_adjust(res, align) == 0);
    return res;
  }
  return nullptr;
}

void list_allocator::free(void *p) {
  debug::_assert(p != nullptr);
  header *h = reinterpret_cast<header *>(math::sub(p, sizeof(header)));

  int block_size = h->size;
  void *block_begin = reinterpret_cast<void *>(math::sub(p, h->adj));
  void *block_end =
      reinterpret_cast<void *>(math::add(block_begin, block_size));
  block *prev = nullptr;
  block *free = free_blocks_;

  while (free != nullptr) {
    if (math::sub(free, block_end) >= 0) break;
    prev = free;
    free = free->next;
  }
  if (prev == nullptr) {
    prev = reinterpret_cast<block *>(block_begin);
    prev->size = block_size;
    prev->next = free_blocks_;
    free_blocks_ = prev;
  } else if (math::eq(math::add(prev, prev->size), block_begin)) {
    prev->size += block_size;
  } else {
    block *tmp = reinterpret_cast<block *>(block_begin);
    tmp->size = block_size;
    tmp->next = prev->next;
    prev->next = tmp;
    prev = tmp;
  }

  if (free != nullptr && math::eq(free, block_end)) {
    prev->size += free->size;
    prev->next = free->next;
  }

  num_allocations_--;
  used_memory_ -= block_size;
}

bool list_allocator::is_owner(void* p) {
  void* base_begin = get_base();
  void* base_end = math::add(base_begin, get_size());
  return (math::sub(base_begin, p) <= 0) && (math::sub(p, base_end) <= 0);
}

size_t list_allocator::calculate_obj_size(size_t n) {
  n = math::align_size(n, sizeof(void*));
  return n + math::align_forwrad_adjust_with_header(nullptr, sizeof(void*), sizeof(header));
}

}  // namespace smem
