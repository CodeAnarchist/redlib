#include "checksums.h"
//TODO NEON might help, compiler uses it with -O3 for len >= 64
//also a full asm implementation without NEON might be faster
static uint16_t checksum_calc(uint64_t sum, const uint8_t *p, size_t len) {
    uint64_t words = 0;
    bool odd = ((uintptr_t)p & 1) != 0;
    if (odd && len) {
        sum += p[0] << 8;
        p++;
        len--;
    }

    while (len >= 2 && ((uintptr_t)p & 7)) {
        words += be16(rd_be16(p));
        p += 2;
        len -= 2;
    }

    while (len >= 16) {
        uint64_t w0 = *(const uint64_t*)p;
        uint64_t w1 = *(const uint64_t*)(p + 8);
        words += (w0 & 0xFFFF) + ((w0 >> 16) & 0xFFFF) + ((w0 >> 32) & 0xFFFF) + (w0 >> 48);
        words += (w1 & 0xFFFF) + ((w1 >> 16) & 0xFFFF) + ((w1 >> 32) & 0xFFFF) + (w1 >> 48);
        p += 16;
        len -= 16;
    }
    while (len >= 8) {
        uint64_t v = *(const uint64_t*)p;
        words += (v & 0xFFFF) + ((v >> 16) & 0xFFFF) + ((v >> 32) & 0xFFFF) + (v >> 48);
        p += 8;
        len -= 8;
    }
    while (len >= 2) {
        words += be16(rd_be16(p));
        p += 2;
        len -= 2;
    }
    if (len) words += be16(p[0] << 8);
    
    while (words >> 16) words = (words & 0xFFFF) + (words >> 16);
    uint16_t data = be16(words);
    sum += odd ? bswap16(data) : data;

    while (sum >> 16) sum = (sum & 0xFFFF) + (sum >> 16);
    return (uint16_t)~sum;
}

uint16_t checksum16(const void *data, size_t len) {
    return checksum_calc(0, data, len);
}

uint16_t checksum16_pipv4(uint32_t src_ip,
                        uint32_t dst_ip,
                        uint8_t  protocol,
                        const uint8_t *payload,
                        uint16_t length)
{
    uint64_t sum = 0;

    sum += (src_ip >> 16) & 0xFFFF;
    sum +=  src_ip & 0xFFFF;
    sum += (dst_ip >> 16) & 0xFFFF;
    sum +=  dst_ip & 0xFFFF;
    sum += protocol;
    sum += length;

    return checksum_calc(sum, payload, length);
}

uint16_t checksum16_pipv6(const uint8_t src_ip[16],
                          const uint8_t dst_ip[16],
                          uint8_t protocol,
                          const uint8_t *payload,
                          uint32_t length)
{
    uint64_t sum = 0;
    for (int i=0;i<16;i+=2) {
        sum += rd_be16(src_ip + i);
        sum += rd_be16(dst_ip + i);
    }
    sum += (length >> 16) & 0xFFFF;
    sum += length & 0xFFFF;
    sum += protocol;
    return checksum_calc(sum, payload, length);
}