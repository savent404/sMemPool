#include <smempool.h>
#include "internal/list_allocator.hxx"
#include "internal/pool_allocator.hxx"
#include "internal/proxy_allocator.hxx"
#include "internal/utils.hxx"
#include "internal/pool.hxx"

#ifdef __cplusplus
extern "C" {
#endif

const smem_config_t *global = nullptr;

void smem_init(const smem_config_t *config) {
  smem::debug::_assert(!global);
  global = config;
}

void* smem_alloc(size_t n) {
  smem::debug::_assert(global);
  return nullptr;
}

void smem_free(void* ptr) {
  smem::debug::_assert(global);
}

#ifdef __cplusplus
}
#endif
