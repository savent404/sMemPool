#pragma once

#include <stddef.h>

namespace smem {

struct debug {
  static void _assert(bool t) {
    if (!t)
      while (1) {
      }
  }
};

namespace math {

inline int align_size(int size, int align) {
  return (size + align - 1) & (~(align - 1));
}

inline void* align_forward(void* addr, int align) {
  return reinterpret_cast<void*>(
      (reinterpret_cast<size_t>(addr) + (align - 1)) & (~(align - 1)));
}

inline int align_forward_adjust(const void* addr, int align) {
  int adj =
      align - (reinterpret_cast<size_t>(addr) & static_cast<size_t>(align - 1));
  return adj == align ? 0 : adj;
}

inline int align_forwrad_adjust_with_header(const void* addr, int align,
                                            int header_size) {
  int adj = align_forward_adjust(addr, align);
  int neededspace = header_size;

  if (adj < neededspace) {
    neededspace -= adj;
    adj += align * (neededspace / align);
    if (neededspace & (align - 1)) adj += align;
  }
  debug::_assert(adj < 100);
  return adj;
}

inline void* add(void* base, int offset) {
  return reinterpret_cast<void*>(reinterpret_cast<size_t>(base) + offset);
}

inline void* sub(void* base, int sub_offset) {
  return reinterpret_cast<void*>(reinterpret_cast<size_t>(base) - sub_offset);
}

inline int sub(void* addr1, void* addr2) {
  return reinterpret_cast<size_t>(addr1) - reinterpret_cast<size_t>(addr2);
}

inline bool eq(void* addr1, void* addr2) { return addr1 == addr2; }

}  // namespace math
}  // namespace smem
