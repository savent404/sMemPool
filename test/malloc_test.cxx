#include <smempool.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>

#include <assert.h>

static void continue_alloc_test(void) {
  void* ptr;
  for (int n = 1; n <= 124; n++) {
    ptr = smem_alloc(n);
    assert(ptr);
    memset(ptr, 0, sizeof(n));
  }
}

static void case_free_merge() {
  auto ptr = smem_alloc(1024);
  smem_free(ptr);

  assert(ptr == smem_alloc(1024));
  smem_free(ptr);
}

static void case_free_cant_merge() {
  auto ptr1 = smem_alloc(1024);
  auto ptr2 = smem_alloc(1024);

  smem_free(ptr1);
  smem_free(ptr2);

  assert(ptr1 == smem_alloc(1024));
  smem_free(ptr1);
}

// check last block behavior, and return nullptr behavior
static void case_recursive_alloc(int size = 1) {
  auto ptr = smem_alloc(size);
  if (!ptr) return;
  case_recursive_alloc(size);
  smem_free(ptr);
}

static void case_skip_small_block() {
  auto ptr1 = smem_alloc(1);
  auto ptr2 = smem_alloc(1024);
  smem_free(ptr1);
  case_recursive_alloc(32);
  smem_free(ptr2);
}

int main() {
  smem_init();
  case_free_merge();
  case_free_cant_merge();
  case_skip_small_block();
  case_recursive_alloc();
  continue_alloc_test();
  return 0;
}
