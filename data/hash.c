#include "data/hash.h"

u64 hash_map_fnv1a64(const void* data, u64 len){
    const u8* bytes = (const u8*)data;
    u64 h = 0xcbf29ce484222325ULL;
    for (u64 i = 0; i < len; i++) {
        h^= (u64)bytes[i];
        h*= 0x100000001b3ULL; 
    }
    return h;
}
