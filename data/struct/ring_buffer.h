#pragma once
#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

//TODO: review allocs & C
typedef struct CRingBuffer {
    void* buffer;
    uint64_t capacity;
    uint64_t element_size;
    uint64_t head;
    uint64_t tail;
    int32_t full;
} CRingBuffer;

void     cring_init(CRingBuffer* rb, void* storage, uint64_t capacity, uint64_t elem_size);
int32_t  cring_push(CRingBuffer* rb, const void* item);
int32_t  cring_pop(CRingBuffer* rb, void* out);
int32_t  cring_is_empty(const CRingBuffer* rb);
int32_t  cring_is_full(const CRingBuffer* rb);
void     cring_clear(CRingBuffer* rb);
uint64_t cring_capacity(const CRingBuffer* rb);
int32_t cring_remove(CRingBuffer* rb, uint64_t index, void* out);

static inline __attribute__((always_inline)) uint64_t cring_size(const CRingBuffer* rb) {
    if (!rb || !rb->buffer || !rb->capacity) return 0;
    if (rb->full) return rb->capacity;
    if (rb->head >= rb->tail) return rb->head - rb->tail;
    return rb->capacity - (rb->tail - rb->head);
}

static inline __attribute__((always_inline)) void* cring_push_slot(CRingBuffer* rb) {
    if (!rb || !rb->buffer || !rb->capacity || rb->full) return 0;
    void* slot = (uint8_t*)rb->buffer + rb->head * rb->element_size;
    rb->head++;
    if (rb->head == rb->capacity) rb->head = 0;
    rb->full = rb->head == rb->tail;
    return slot;
}

static inline __attribute__((always_inline)) void* cring_front(const CRingBuffer* rb) {
    if (!rb || !rb->buffer || (!rb->full && rb->head == rb->tail)) return NULL;
    return (uint8_t*)rb->buffer + rb->tail * rb->element_size;
}

static inline __attribute__((always_inline)) void* cring_at(const CRingBuffer* rb, uint64_t index) {
    if (!rb || !rb->buffer || index >= cring_size(rb)) return NULL;
    uint64_t pos = rb->tail + index;
    if (pos >= rb->capacity) pos -= rb->capacity;
    return (uint8_t*)rb->buffer + pos * rb->element_size;
}

static inline __attribute__((always_inline)) int32_t cring_drop(CRingBuffer* rb) {
    if (!rb || (!rb->full && rb->head == rb->tail)) return 0;
    rb->tail++;
    if (rb->tail == rb->capacity) rb->tail = 0;
    rb->full = 0;
    return 1;
}

#ifdef __cplusplus
}
#endif
