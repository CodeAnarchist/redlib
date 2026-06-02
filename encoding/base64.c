#include "encoding/base64.h"
#include "string/string.h"

static const char base64_table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

// 	    000	001	010	011	100	101	110	111
//000	A	B	C	D	E	F	G	H
//001	I	J	K	L	M	N	O	P
//010	Q	R	S	T	U	V	W	X
//011	Y	Z	a	b	c	d	e	f
//100	g	h	i	j	k	l	m	n
//101	o	p	q	r	s	t	u	v
//110	w	x	y	z	0	1	2	3
//111	4	5	6	7	8	9	+	/

bool base64_encode_buffer(const uint8_t *input, uint32_t input_len, char *out, uint32_t out_cap, uint32_t *out_len) {
    if (out_len) *out_len = 0;
    if ((!input && input_len) || !out || !out_len) return false;

    uint64_t need = (((uint64_t)input_len + 2)/3) * 4;
    if (need + 1 > out_cap) return false;

    uint32_t ip = 0;
    uint32_t op = 0;
    while (ip < input_len) {
        uint32_t left = input_len - ip;
        uint32_t a = input[ip++];
        uint32_t b = left > 1 ? input[ip++] : 0;
        uint32_t c = left > 2 ? input[ip++] : 0;
        uint32_t n = (a << 16) | (b << 8) | c;

        out[op++] = base64_table[(n >> 18) & 63];
        out[op++] = base64_table[(n >> 12) & 63];
        out[op++] = left > 1 ? base64_table[(n >> 6) & 63] : '=';
        out[op++] = left > 2 ? base64_table[n & 63] : '=';
    }

    out[op] = 0;
    *out_len = op;
    return true;
}

bool base64_decode_buffer(const char *input, uint32_t input_len, uint8_t *out, uint32_t out_cap, uint32_t *out_len) {
    if (out_len) *out_len = 0;
    if (!input || !out || !out_len) return false;

    uint8_t q[4];
    uint32_t qlen = 0;
    uint32_t pad = 0;
    uint32_t written = 0;
    bool finished = false;

    for (uint32_t i = 0; i < input_len; i++) {
        char c = input[i];
        if (is_whitespace(c)) continue;
        if (finished) return false;

        if (c == '=') {
            if (qlen < 2) return false;
            if (++pad > 2) return false;
            q[qlen++] = 0;
        } else {
            int val = str_has_char(base64_table, 64, c);
            if (val < 0) return false;
            if (pad) return false;
            q[qlen++] = (uint8_t)val;
        }

        if (qlen < 4) continue;
        if (written >= out_cap) return false;
        out[written++] = (uint8_t)((q[0] << 2) | (q[1] >> 4));

        if (pad < 2) {
            if (written >= out_cap) return false;
            out[written++] = (uint8_t)((q[1] << 4) | (q[2] >> 2));
        } else if (q[1] & 0x0F) return false;

        if (pad < 1) {
            if (written >= out_cap) return false;
            out[written++] = (uint8_t)((q[2] << 6) | q[3]);
        } else if (q[2] & 0x03) return false;

        if (pad) finished = true;
        qlen = 0;
        pad = 0;
    }

    if (qlen) {
        if (pad || qlen == 1) return false;

        if (written >= out_cap) return false;
        out[written++] = (uint8_t)((q[0] << 2) | (q[1] >> 4));

        if (qlen == 2) {
            if (q[1] & 0x0F) return false;
        } else if (qlen == 3) {
            if (written >= out_cap) return false;
            out[written++] = (uint8_t)((q[1] << 4) | (q[2] >> 2));
            if (q[2] & 0x03) return false;
        }
    }

    *out_len = written;
    return true;
}
