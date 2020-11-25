#include <smempool.h>

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "internal/list_allocator.hxx"
#include "internal/pool.hxx"
#include "internal/slab_allocator.hxx"

static void init() {
  smem_config_t config;
  memset(&config, 0, sizeof(config));

  config.malloc = malloc;
  config.free = free;

  config.list_default_bs = 1024;
  config.slab_8B_default_bs = 256;
  config.slab_16B_default_bs = 1024;
  config.slab_32B_default_bs = 1024;
  config.slab_64B_default_bs = 1024;
  config.slab_128B_default_bs = 512;

  smem_init(&config);
}

extern smem::pool<smem::list_allocator> *list_pool;
extern smem::pool<smem::slab_allocator> *slab_pool[6];

static void case_allocator_branch_chose_(smem::allocator *allocator,
                                         size_t size) {
  auto num = allocator->get_num_allocations();
  auto ptr = smem_alloc(size);
  auto num_1 = allocator->get_num_allocations();

  assert(ptr);
  assert(num + 1 == num_1);
  assert(allocator->is_owner(ptr));
  smem_free(ptr);
}

static void case_allocator_branch_chose() {
  for (int i = 1; i < 5; i++) {
    case_allocator_branch_chose_(slab_pool[1], i);
  }
  for (int i = 5; i < 9; i++) {
    case_allocator_branch_chose_(slab_pool[1], i);
  }
  for (int i = 9; i < 17; i++) {
    case_allocator_branch_chose_(slab_pool[2], i);
  }
  for (int i = 17; i < 33; i++) {
    case_allocator_branch_chose_(slab_pool[3], i);
  }
  for (int i = 33; i < 65; i++) {
    case_allocator_branch_chose_(slab_pool[4], i);
  }
  for (int i = 65; i < 129; i++) {
    case_allocator_branch_chose_(slab_pool[5], i);
  }
  for (int i = 129; i < 2048; i++) {
    case_allocator_branch_chose_(list_pool, i);
  }
}

int main(void) {
  init();
  case_allocator_branch_chose();
  return 0;
}
