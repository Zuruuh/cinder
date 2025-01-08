#define FNV_OFFSET 14695981039346656037UL
#define FNV_PRIME 1099511628211UL

#include "hash.h"
#include <stdint.h>

uint64_t hash_key(const char *key) {
  uint64_t hash = FNV_OFFSET;
  for (const char *p = key; *p; p++) {
    hash ^= (uint64_t)(unsigned char)(*p);
    hash *= FNV_PRIME;
  }

  return hash;
}
