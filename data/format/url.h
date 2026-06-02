#pragma once

#include "types.h"
#include "string/string.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    sizedptr scheme;
    sizedptr userinfo;
    sizedptr host;
    uint16_t port;
    sizedptr path;
    sizedptr query;
    sizedptr fragment;
    bool ok;
} ParsedURL;

ParsedURL parse_url(const char *buf, uint32_t len);
string url_request_path(const ParsedURL *url, const string *base_path);

#ifdef __cplusplus
}
#endif
