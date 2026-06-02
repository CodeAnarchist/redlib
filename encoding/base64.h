#pragma once

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

bool base64_encode_buffer(const uint8_t *input, uint32_t input_len, char *out, uint32_t out_cap, uint32_t *out_len);
bool base64_decode_buffer(const char *input, uint32_t input_len, uint8_t *out, uint32_t out_cap, uint32_t *out_len);

#ifdef __cplusplus
}
#endif
