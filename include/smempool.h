#pragma once

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
  void* (*malloc)(size_t n);
  void (*free)(void* n);
  size_t list_default_bs;
  size_t slab_4B_default_bs;
  size_t slab_8B_default_bs;
  size_t slab_16B_default_bs;
  size_t slab_32B_default_bs;
  size_t slab_64B_default_bs;
  size_t slab_128B_default_bs;
} smem_config_t;

/**
 * @brief init smem allocator
 *
 * @param config see smem_config_t
 * @code
 * #include <stdlib.h>
 *
 * int main() {
 *  smem_config_t config;
 *  memset(&config, 0, sizeof(config));
 *  config.malloc = malloc;
 *  config.free = free;
 *  config.list_default_bs = 1024 * 10; // 10K
 *  config.list_4B_default_bs = 0; // not used
 *  config.list_8B_default_bs = 256; // 256B
 *  config.list_16_default_bs = 1024; // 1K
 *  config.list_32_default_bs = 256; // 256B
 *  config.list_64_default_bs = 0; // not used
 *  config.list_128B_default_bs = 0; // not used
 *  smem_init(&config);
 *  // ....
 * }
 * @endcode
 */
void smem_init(const smem_config_t* config);

/**
 * @brief smem alloc memory
 *
 * @param n size in bytes
 * @return void*
 */
void* smem_alloc(size_t n);

/**
 * @brief smem free memory
 *
 * @param ptr
 */
void smem_free(void* ptr);

#ifdef __cplusplus
}
#endif
