#pragma once
#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

void sha256_hash(const void *data, uint32_t len, uint8_t out[32]);

#ifdef __cplusplus
}
#endif
