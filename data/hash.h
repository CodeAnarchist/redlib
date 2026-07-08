#pragma once
#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

uint64_t hash_map_fnv1a64(const void* data, uint64_t len);

#ifdef __cplusplus
}
#endif
