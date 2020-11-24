#include <stdlib.h>
#include <assert.h>
#include "internal/pool.hxx"
#include "internal/list_allocator.hxx"

static smem::pool<smem::list_allocator> pool(malloc, free, 1024, 1);

void * _malloc_(size_t n) {
    return pool.alloc(n, sizeof(void*));
}

void _free_(void* p) {
    pool.free(p);
}

static void case_malloc_free() {
    auto ptr = _malloc_(1);
    assert(ptr);
    _free_(ptr);
}

static void case_malloc_many(int times = 1024) {
    if (!times) return;
    auto ptr = _malloc_(512);
    case_malloc_many(times - 1);
    _free_(ptr);
}

static void case_check_range() {
    auto ptr = _malloc_(1);
    assert(pool.belong(ptr));
    _free_(ptr);

    ptr = malloc(1);
    assert(pool.belong(ptr) == false);
    free(ptr);
}

int main() {
    case_malloc_free();
    case_malloc_many();
    case_check_range();
    return 0;
}