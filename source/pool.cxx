#include "internal/pool.hxx"
#include "internal/list_allocator.hxx"

namespace smem {

pool_item::pool_item(pointer_type ptr)
    : next(this), prev(this), item_(std::move(ptr)) {}

bool pool_item::add(pool_item* item) {
  item->next = this->next;
  item->prev = this;
  this->next->prev = item;
  this->next = item;
}

bool pool_item::remove() {
  this->prev->next = this->next;
  this->next->prev = this->prev;
  this->next = nullptr;
  this->prev = nullptr;
}

bool pool_item::isEmpty() { return item_.get() == nullptr; }

pool_item::reference_type pool_item::get() { return *item_; }

template <>
pool<list_allocator>::allocator_ptr pool<list_allocator>::new_item(size_t s) {
  list_allocator* p =
      reinterpret_cast<list_allocator*>(malloc_(sizeof(list_allocator)));
  void* base = malloc_(s);
  new (p) list_allocator(base, s);
  return std::unique_ptr<list_allocator>(p);
}

}  // namespace smem
