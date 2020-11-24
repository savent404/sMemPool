#include <smempool.h>
#include "internal/list_allocator.hxx"
#include "internal/pool_allocator.hxx"
#include "internal/proxy_allocator.hxx"
#include "internal/utils.hxx"

static smem::allocator* global = nullptr;
#ifdef __cplusplus
extern "C" {
#endif

__attribute__((weak)) void smem_init(void) {
  size_t size = 1024 * 10;  // 10K
  char* buffer = new char[size];
  global = new smem::list_allocator(buffer, size);
}

void* smem_alloc(size_t n) {
  if (!global) {
    return nullptr;
  }
  auto res = global->alloc(n);
  if (!res) smem::debug::_assert(global->get_last_size() <= n + 32);
  return res;
}

void smem_free(void* ptr) {
  if (global) {
    global->free(ptr);
  }
}

void smem_set_alloctor(smem_alloctor_t _alloc) {}

void smem_set_alloctor_mode(smem_mode_t global_mode) {}

#ifdef __cplusplus
}
#endif
