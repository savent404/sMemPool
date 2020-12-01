#pragma once

#include <smempool.h>

#include <functional>
#include <memory>
#include <new>
#include <utility>

#include "internal/allocator.hxx"
#include "internal/utils.hxx"

namespace smem {

struct pool_item {
 public:
  using item_type = allocator;
  using reference_type = item_type&;
  using pointer_type = std::unique_ptr<item_type>;

  explicit pool_item(pointer_type ptr);

  bool add(pool_item* item);
  bool remove();
  bool isEmpty();
  reference_type get();

  pool_item(const pool_item&) = delete;
  pool_item& operator=(const pool_item&) = delete;

 public:
  pool_item* next;
  pool_item* prev;

 private:
  pointer_type item_;
};

template <typename _Tp>
struct pool : public allocator {
 public:
  using malloc_ptr = std::function<void*(size_t)>;
  using free_ptr = std::function<void(void*)>;
  using allocator_ptr = std::unique_ptr<_Tp>;

  pool(malloc_ptr m, free_ptr f, size_t default_size, size_t default_num = 0)
      : allocator(nullptr, default_size * default_num),
        head_(std::unique_ptr<_Tp>(nullptr)),
        malloc_(m),
        free_(f),
        size_(default_size),
        num_(default_num) {
    for (size_t i = 0; i < default_num; i++) {
      add_item(default_size);
    }
  }
  pool(malloc_ptr m, free_ptr f, size_t default_size, size_t default_num,
       size_t slab_size)
      : allocator(nullptr, default_size * default_num),
        head_(std::unique_ptr<_Tp>(nullptr)),
        malloc_(m),
        free_(f),
        size_(default_size),
        slab_size_(slab_size),
        num_(default_num) {
    for (size_t i = 0; i < default_num; i++) {
      add_item(default_size);
    }
  }

  ~pool() {
    pool_item* head = &head_;
    pool_item* item = head->prev;
    pool_item* next;
    while (item != head) {
      next = item->next;
      remove_item(item);
      item = next;
    }
  }

  void* alloc(size_t n, int align) override {
    pool_item* item = find_alloc_item(n);
    if (item) {
      return do_alloc(item, n);
    }
    // need a new pool item
    size_t item_size = n * 2 > size_ ? n * 2 : size_;
    item = add_item(item_size);
    return do_alloc(item, n);
  }

  void free(void* p) override {
    auto item = find_owner_item(p);
    if (item) do_free(item, p);
  }

  bool is_owner(void* p) override { return find_owner_item(p) != nullptr; }

  pool(const pool&) = delete;
  pool& operator=(const pool&) = delete;

 protected:
  allocator_ptr new_item(size_t size);
  pool_item* add_item(size_t size) {
    pool_item* item = reinterpret_cast<pool_item*>(malloc_(sizeof(pool_item)));
    new (item) pool_item(new_item(size));
    head_.add(item);
    return item;
  }
  bool remove_item(pool_item* it) { return it->remove(); }
  pool_item* find_alloc_item(size_t n) {
    pool_item* item = head_.next;
    while (item && item != &head_) {
      if (!item->isEmpty() && item->get().can_alloc(n)) {
        return item;
      }
      item = item->next;
    }
    return nullptr;
  }
  pool_item* find_owner_item(void* p) {
    pool_item* item = head_.next;
    while (item && item != &head_) {
      if (!item->isEmpty() && item->get().is_owner(p)) {
        return item;
      }
      item = item->next;
    }
    return nullptr;
  }
  size_t calculate_obj_size(size_t n) override { return 0; }

 private:
  void* do_alloc(pool_item* item, size_t n) {
    auto item_used_size = item->get().get_used_memory();
    auto item_used_num = item->get().get_num_allocations();
    auto res = item->get().alloc(n);
    used_memory_ += item->get().get_used_memory() - item_used_size;
    num_allocations_ += item->get().get_num_allocations() - item_used_num;
    return res;
  }
  void do_free(pool_item* item, void* ptr) {
    auto item_used_size = item->get().get_used_memory();
    auto item_used_num = item->get().get_num_allocations();
    item->get().free(ptr);
    used_memory_ += item->get().get_used_memory() - item_used_size;
    num_allocations_ += item->get().get_num_allocations() - item_used_num;
  }
  pool_item head_;
  malloc_ptr malloc_;
  free_ptr free_;
  size_t size_;
  size_t slab_size_;
  size_t num_;
};

}  // namespace smem

namespace smem {}  // namespace smem
