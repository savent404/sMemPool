#include <smempool.h>

#include <new>

#include "internal/list_allocator.hxx"
#include "internal/pool.hxx"
#include "internal/proxy_allocator.hxx"
#include "internal/slab_allocator.hxx"
#include "internal/utils.hxx"

#ifdef __cplusplus
extern "C" {
#endif

static smem_config_t global_context = {.malloc = nullptr, .free = nullptr};
static smem_config_t* global = nullptr;

using list_pool_type = smem::pool<smem::list_allocator>;
using slab_pool_type = smem::pool<smem::slab_allocator>;

list_pool_type* list_pool = nullptr;
slab_pool_type* slab_pool[6] = {
    nullptr};  // [0]=4B, [1]=8B, [2]=16B, [3]=32B, [4]=64B, [5]=128B

#define MEMBER_CPY(dst_ctx, src_ctx, member) \
  { (dst_ctx)->member = (src_ctx)->member; }

void smem_init(const smem_config_t* config) {
  smem::debug::_assert(!global);
  smem::debug::_assert(config->malloc);
  smem::debug::_assert(config->free);
  smem::debug::_assert(config->list_default_bs);

  MEMBER_CPY(&global_context, config, malloc);
  MEMBER_CPY(&global_context, config, free);
  MEMBER_CPY(&global_context, config, list_default_bs);
  MEMBER_CPY(&global_context, config, slab_4B_default_bs);
  MEMBER_CPY(&global_context, config, slab_8B_default_bs);
  MEMBER_CPY(&global_context, config, slab_16B_default_bs);
  MEMBER_CPY(&global_context, config, slab_32B_default_bs);
  MEMBER_CPY(&global_context, config, slab_64B_default_bs);
  MEMBER_CPY(&global_context, config, slab_128B_default_bs);

  global = &global_context;

  if (config->list_default_bs) {
    list_pool = reinterpret_cast<list_pool_type*>(
        global->malloc(sizeof(list_pool_type)));
    new (list_pool)
        list_pool_type(config->malloc, config->free, config->list_default_bs);
  }
  if (config->slab_4B_default_bs) {
    int bs = 8;
    slab_pool[0] = reinterpret_cast<slab_pool_type*>(
        global->malloc(sizeof(slab_pool_type)));
    new (slab_pool[0]) slab_pool_type(config->malloc, config->free,
                                      config->slab_4B_default_bs, 0, bs);
  }
  if (config->slab_8B_default_bs) {
    int bs = 8;
    slab_pool[1] = reinterpret_cast<slab_pool_type*>(
        global->malloc(sizeof(slab_pool_type)));
    new (slab_pool[1]) slab_pool_type(config->malloc, config->free,
                                      config->slab_8B_default_bs, 0, bs);
  }
  if (config->slab_16B_default_bs) {
    int bs = 16;
    slab_pool[2] = reinterpret_cast<slab_pool_type*>(
        global->malloc(sizeof(slab_pool_type)));
    new (slab_pool[2]) slab_pool_type(config->malloc, config->free,
                                      config->slab_16B_default_bs, 0, bs);
  }
  if (config->slab_32B_default_bs) {
    int bs = 32;
    slab_pool[3] = reinterpret_cast<slab_pool_type*>(
        global->malloc(sizeof(slab_pool_type)));
    new (slab_pool[3]) slab_pool_type(config->malloc, config->free,
                                      config->slab_32B_default_bs, 0, bs);
  }
  if (config->slab_64B_default_bs) {
    int bs = 64;
    slab_pool[4] = reinterpret_cast<slab_pool_type*>(
        global->malloc(sizeof(slab_pool_type)));
    new (slab_pool[4]) slab_pool_type(config->malloc, config->free,
                                      config->slab_64B_default_bs, 0, bs);
  }
  if (config->slab_128B_default_bs) {
    int bs = 128;
    slab_pool[5] = reinterpret_cast<slab_pool_type*>(
        global->malloc(sizeof(slab_pool_type)));
    new (slab_pool[5]) slab_pool_type(config->malloc, config->free,
                                      config->slab_128B_default_bs, 0, bs);
  }
}

smem::allocator* find_allocator(size_t n) {
  static size_t matrix_size[6] = {4, 8, 16, 32, 64, 128};
  for (int i = 0; i < 6; i++) {
    if (n <= matrix_size[i] && slab_pool[i]) {
      return slab_pool[i];
    }
  }
  return list_pool;
}

smem::allocator* find_deallocator(void* ptr) {
  for (auto p : slab_pool) {
    if (p && p ->is_owner(ptr)) {
      return p;
    }
  }
  if (list_pool->is_owner(ptr)) return list_pool;
  return nullptr;
}

void* smem_alloc(size_t n) {
  smem::debug::_assert(global);
  auto allocator = find_allocator(n);
  smem::debug::_assert(allocator);
  return allocator->alloc(n, sizeof(void*));
}

void smem_free(void* ptr) {
  smem::debug::_assert(global);
  auto deallocator = find_deallocator(ptr);
  smem::debug::_assert(deallocator);
  deallocator->free(ptr);
}

#ifdef __cplusplus
}
#endif
