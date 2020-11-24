#pragma once

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

void* smem_alloc(size_t n);

void smem_free(void* ptr);

typedef struct {
  void* (*malloc)(size_t);
  void (*free)(void*);
} smem_alloctor_t;

void smem_set_alloctor(smem_alloctor_t alloctor);

typedef enum {
    SMEM_MODE_LINE,
    SMEM_MODE_LIST,
    SMEM_MODE_STACK,
    SMEM_MODE_POOL,
} smem_mode_t;

void smem_set_alloctor_mode(smem_mode_t global_mode);

#ifdef __cplusplus
}
#endif
