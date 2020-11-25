
#include <stdlib.h>
#include <string.h>

#include <new>

#include "internal/slab_allocator.hxx"

static smem::slab_allocator *slab = nullptr;

static void init() {
  slab = reinterpret_cast<smem::slab_allocator *>(
      malloc(sizeof(smem::slab_allocator)));
  void *base = malloc(1024);
  slab = new (slab) smem::slab_allocator(base, 1024, sizeof(void *), 16);
}

static void *_malloc_(size_t n) { return slab->alloc(n, sizeof(void *)); }
static void _free_(void *ptr) { slab->free(ptr); }

static void case_simple_test() {
  for (int i = 0; i < 100; i++) {
    auto ptr = _malloc_(sizeof(int));
    memset(ptr, 0, sizeof(int));
    _free_(ptr);
  }
}

static void case_recursive_test(int endpoint = 63) {
    if (!endpoint) return;
    auto ptr = _malloc_(16);
    memset(ptr, 0, 16);
    case_recursive_test(endpoint - 1);
    _free_(ptr);
}

int main() {
  init();
  case_simple_test();
  case_recursive_test();
  return 0;
}
