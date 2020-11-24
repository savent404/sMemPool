#include <stdlib.h>
#include <string.h>
#include <iostream>
#include "internal/list_allocator.hxx"

#include <assert.h>

static char buffer[1024 * 10];
static smem::list_allocator al(buffer, 1024 * 10);
void* _alloc_(size_t n) { return al.alloc(n, sizeof(void*)); }

void _free_(void* p) { return al.free(p); }

static void continue_alloc_test(int it = 124) {
  auto ptr = _alloc_(it);
  memset(ptr, 0, it);
  if (it != 1) continue_alloc_test(it - 1);
  _free_(ptr);
}

static void case_free_merge() {
  auto ptr = _alloc_(1024);
  _free_(ptr);

  assert(ptr == _alloc_(1024));
  _free_(ptr);
}

static void case_free_cant_merge() {
  auto ptr1 = _alloc_(1024);
  auto ptr2 = _alloc_(1024);

  _free_(ptr1);
  _free_(ptr2);

  assert(ptr1 == _alloc_(1024));
  _free_(ptr1);
}

// check last block behavior, and return nullptr behavior
static void case_recursive_alloc(int size = 1) {
  auto ptr = _alloc_(size);
  if (!ptr) return;
  case_recursive_alloc(size);
  _free_(ptr);
}

static void case_skip_small_block() {
  auto ptr1 = _alloc_(1);
  auto ptr2 = _alloc_(1024);
  _free_(ptr1);
  case_recursive_alloc(32);
  _free_(ptr2);
}

int main() {
  case_free_merge();
  case_free_cant_merge();
  case_skip_small_block();
  case_recursive_alloc();
  continue_alloc_test();
  return 0;
}
