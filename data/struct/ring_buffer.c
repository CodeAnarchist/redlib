#include "ring_buffer.h"
#include "std/memory.h"

void cring_init(CRingBuffer* rb, void* storage, uint64_t capacity, uint64_t elem_size) {
    rb->buffer = storage;
    rb->capacity = capacity;
    rb->element_size = elem_size;
    rb->head = rb->tail = 0;
    rb->full = 0;
}

uint64_t cring_capacity(const CRingBuffer* rb) {
    return rb->capacity;
}

int32_t cring_push(CRingBuffer* rb, const void* item) {
    if (!item) return 0;
    void* slot = cring_push_slot(rb);
    if (!slot) return 0;
    memcpy(slot, item, rb->element_size);
    return 1;
}

int32_t cring_pop(CRingBuffer* rb, void* out) {
    if (!out) return 0;
    void* src = cring_front(rb);
    if (!src) return 0;
    memcpy(out, src, rb->element_size);
    return cring_drop(rb);
}

int32_t cring_is_empty(const CRingBuffer* rb) {
    return (!rb || (!rb->full && rb->head == rb->tail));
}

int32_t cring_is_full(const CRingBuffer* rb) {
    return rb && rb->full;
}

void cring_clear(CRingBuffer* rb) {
    if (!rb) return;
    rb->head = rb->tail = 0;
    rb->full = 0;
}

int32_t cring_remove(CRingBuffer* rb, uint64_t index, void* out) {
    uint64_t size = cring_size(rb);
    if (!rb || index >= size) return 0;
    uint64_t pos = rb->tail + index;
    if (pos >= rb->capacity) pos -= rb->capacity;
    uint8_t* buf = (uint8_t*)rb->buffer;
    uint64_t width = rb->element_size;
    if (out) memcpy(out, buf + pos * width, width); 
    uint64_t remaining = size - index - 1;
    uint64_t first = rb->capacity - pos - 1;
    if (first > remaining) first = remaining;
    if (first) memmove(buf + pos * width, buf + (pos + 1) * width, first * width);
    remaining -= first;
    if (remaining) { 
        memcpy(buf + (rb->capacity-1) * width, buf, width);
        remaining--;
        if (remaining) memmove(buf, buf + width, remaining * width);
    }
    if (rb->head) rb->head = rb->head-1;
    else rb->head = rb->capacity - 1;
    rb->full = 0;
    return 1;
}
