#include <smempool.h>

#include <new>

#include "internal/list_allocator.hxx"
#include "internal/pool.hxx"
#include "internal/pool_allocator.hxx"
#include "internal/proxy_allocator.hxx"
#include "internal/utils.hxx"

#ifdef __cplusplus
extern "C" {
#endif

const smem_config_t* global = nullptr;
using list_pool_type = smem::pool<smem::list_allocator>;

list_pool_type* list_pool = nullptr;

void smem_init(const smem_config_t* config) {
  smem::debug::_assert(!global);
  global = config;

  if (config->list_default_bs) {
    list_pool = reinterpret_cast<list_pool_type*>(
        global->malloc(sizeof(list_pool_type)));
    new (list_pool) list_pool_type(config->malloc, config->free, config->list_default_bs);
  }
}

void* smem_alloc(size_t n) {
  smem::debug::_assert(global);
  return list_pool->alloc(n, sizeof(void*));
}

void smem_free(void* ptr) {
  smem::debug::_assert(global);
  list_pool->free(ptr);
}

#ifdef __cplusplus
}
#endif
