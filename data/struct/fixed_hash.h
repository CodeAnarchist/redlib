#pragma once
#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    uint32_t key;
    uint16_t value;
    uint16_t next;
} fixed_hash_entry_t;

typedef struct {
    fixed_hash_entry_t* entries;
    uint16_t* buckets;
    uint32_t capacity;
    uint32_t max_size;
    uint32_t count;
    uint32_t size;
    uint16_t free_head;
    uint8_t bucket_shift;
} fixed_hash_t;

bool fixed_hash_init(fixed_hash_t* map, fixed_hash_entry_t* entries, uint16_t* buckets, uint32_t capacity, uint32_t max_size);
bool fixed_hash_find(const fixed_hash_t* map, uint32_t key, uint16_t* value);
uint16_t* fixed_hash_find_value_ptr(fixed_hash_t* map, uint32_t key);
bool fixed_hash_put(fixed_hash_t* map, uint32_t key, uint16_t value);
bool fixed_hash_remove(fixed_hash_t* map, uint32_t key);

#ifdef __cplusplus
}
#endif
