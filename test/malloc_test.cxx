#include <smempool.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>

int main() {
  void* ptr;

  for (int i = 0; i < 100; i++) {
    for (int n = 1; n <= 1024; n++) {
      if (ptr = smem_alloc(n)) {
        memset(ptr, 0, sizeof(n));
      } else {
        std::cerr << "alloc" << std::endl;
        return -1;
      }
    }
  }
  return 0;
}
