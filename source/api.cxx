#include <smempool.h>

#include <stdlib.h>
static smem_alloctor_t global_allocator = {.malloc = malloc, .free = free};
static smem_mode_t global_mode = SMEM_MODE_LINE;

#ifdef __cplusplus
extern "C" {
#endif

void* smem_alloc(size_t n) { return global_allocator.malloc(n); }

void smem_free(void* ptr) { global_allocator.free(ptr); }

void smem_set_alloctor(smem_alloctor_t _alloc) {
  global_allocator.malloc = _alloc.malloc;
  global_allocator.free = _alloc.free;
}

void smem_set_alloctor_mode(smem_mode_t global_mode) {

}

#ifdef __cplusplus
}
#endif
