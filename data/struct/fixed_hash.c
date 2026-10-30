#include "fixed_hash.h"
#define FIXED_HASH_END UINT16_MAX

static uint32_t golden_hash(const fixed_hash_t* map, uint32_t key) {
    uint32_t mixed = key * 0x9e3779b1u;
    return (uint32_t)(((uint64_t)mixed) >> map->bucket_shift);
}

bool fixed_hash_init(fixed_hash_t* map, fixed_hash_entry_t* entries, uint16_t* buckets, uint32_t capacity, uint32_t max_size) {
    if (!map || !entries || !buckets || !capacity || (capacity & (capacity - 1)) || !max_size || max_size > FIXED_HASH_END) return false;
    for (uint32_t i = 0; i < capacity; i++) buckets[i] = FIXED_HASH_END;
    *map = (fixed_hash_t){.entries=entries, .buckets=buckets, .capacity=capacity, .max_size=max_size, 
        .count=0, .size=0, .free_head=FIXED_HASH_END, .bucket_shift=(uint8_t)(32 - __builtin_ctz(capacity))};
    return true;
}

bool fixed_hash_find(const fixed_hash_t* map, uint32_t key, uint16_t* value) {
    if (!map || !map->entries || !map->buckets || !value) return false;
    for (uint16_t i = map->buckets[golden_hash(map, key)]; i != FIXED_HASH_END; i = map->entries[i].next) {
        if (map->entries[i].key != key) continue;
        *value = map->entries[i].value;
        return true;
    }
    return false;
}

uint16_t* fixed_hash_find_value_ptr(fixed_hash_t* map, uint32_t key) {
    if (!map || !map->entries || !map->buckets) return NULL;
    uint32_t bucket = golden_hash(map, key);
    for (uint16_t i = map->buckets[bucket]; i != FIXED_HASH_END; i = map->entries[i].next)if (map->entries[i].key == key) return &map->entries[i].value;

    return NULL;
}

bool fixed_hash_put(fixed_hash_t* map, uint32_t key, uint16_t value) {
    if (!map || !map->entries || !map->buckets) return false;
    uint32_t bucket = golden_hash(map, key);
    for (uint16_t i = map->buckets[bucket]; i != FIXED_HASH_END; i = map->entries[i].next) {
        if (map->entries[i].key != key) continue;
        map->entries[i].value = value;
        return true;
    }
    if (map->size >= map->max_size) return false;
    uint16_t index;
    if (map->free_head != FIXED_HASH_END) {
        index = map->free_head;
        map->free_head = map->entries[index].next;
    } else index = (uint16_t)map->count++;

    map->entries[index] = (fixed_hash_entry_t){.key=key, .value=value, .next=map->buckets[bucket]};
    map->buckets[bucket] = index;
    map->size++;
    return true;
}

bool fixed_hash_remove(fixed_hash_t* map, uint32_t key) {
    if (!map || !map->entries || !map->buckets) return false;
    uint32_t bucket = golden_hash(map, key);
    uint16_t* link = &map->buckets[bucket];
    while (*link != FIXED_HASH_END) {
        uint16_t index = *link;
        fixed_hash_entry_t* entry = &map->entries[index];
        if (entry->key == key) {
            *link = entry->next;
            entry->next = map->free_head;
            map->free_head = index;
            map->size--; 
            return true;
        }
        link = &entry->next;
    }
    return false;
}
