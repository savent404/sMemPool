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
    for (int i = 0; i < default_num; i++) {
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
    pool_item* item = head_.next;
    void* res = nullptr;

    while (item && item != &head_) {
      if (item->isEmpty()) continue;
      if (item->get().get_last_size() >= n) {
        res = item->get().alloc(n);
        if (res) return res;
      }
      item = item->next;
    }

    // need a new pool item
    size_t item_size = n > size_ ? n * 2 : size_;
    item = add_item(item_size);
    res = item->get().alloc(n);
    return res;
  }
  void free(void* p) override {
    pool_item* item = head_.next;
    void* base = nullptr;
    void* end = nullptr;
    while (item && item != &head_) {
      if (!item->isEmpty()) {
        base = item->get().get_base();
        end = math::add(base, item->get().get_size());

        // in range
        if (math::sub(p, base) >= 0 && math::sub(p, end) <= 0) {
          item->get().free(p);
          return;
        }
      }
      item = item->next;
    }
    debug::_assert(false);
  }
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

 private:
  pool_item head_;
  malloc_ptr malloc_;
  free_ptr free_;
  size_t size_;
  size_t num_;
};

}  // namespace smem

namespace smem {}  // namespace smem
