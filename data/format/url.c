#include "url.h"

ParsedURL parse_url(const char *buf, uint32_t len) {
    ParsedURL r = {0};
    if (!buf || !len) return r;

    uint32_t i = 0;

    if (is_alpha(buf[i])) {
        i++;
        while (i < len) {
            char c= buf[i];
            if (c == ':') {
                r.scheme.ptr = (uintptr_t)buf;
                r.scheme.size = i;
                i++;
                break;
            }
            if (c == '/' || c == '?' || c == '#') {
                i = 0;
                break;
            }
            if (!(is_alnum(c) || c == '+' || c == '-' || c == '.')) {
                i = 0;
                break;
            }
            i++;
        }
        if (i >= len) i = 0;
    }

    if (i + 1 < len && buf[i] =='/' &&buf[i + 1] == '/') {
        i += 2;
        uint32_t auth_start = i;
        uint32_t auth_end = auth_start;
        while (auth_end < len) {
            char c = buf[auth_end];
            if (c == '/' || c == '?' || c == '#') break;
            auth_end++;
        }

        if (auth_start >= auth_end) return (ParsedURL){0};

        uint32_t user_end = auth_start;
        while (user_end<auth_end && buf[user_end] != '@') user_end++;

        uint32_t host_start;
        if (user_end < auth_end) {
            r.userinfo.ptr = (uintptr_t)(buf + auth_start);
            r.userinfo.size = user_end - auth_start;
            host_start = user_end + 1;
        } else host_start = auth_start;

        if (host_start >= auth_end) return (ParsedURL){0};

        if (buf[host_start] == '[') {
            uint32_t p = host_start + 1;
            while (p < auth_end && buf[p] != ']') p++;

            if (p >= auth_end || p == host_start + 1) return (ParsedURL){0};

            r.host.ptr = (uintptr_t)(buf + host_start + 1);
            r.host.size = p -(host_start + 1);

            p++;
            if (p < auth_end && buf[p] == ':') {
                p++;
                if (p >= auth_end) return (ParsedURL){0};
                uint32_t port = 0;
                while (p < auth_end) {
                    char d = buf[p];
                    if (!is_digit(d)) return (ParsedURL){0};

                    port = port * 10 + (uint32_t)(d - '0');
                    if (port > 65535) return (ParsedURL){0};
                    p++;
                }
                r.port = (uint16_t)port;
            } else if (p != auth_end) return (ParsedURL){0};
        } else {
            uint32_t p = host_start;
            while (p < auth_end && buf[p] != ':') p++;
            if (p == host_start) return (ParsedURL){0};

            r.host.ptr = (uintptr_t)(buf + host_start);
            r.host.size = p -host_start;

            if (p < auth_end) {
                p++;
                if (p >= auth_end) return (ParsedURL){0};
                uint32_t port = 0;
                while (p < auth_end) {
                    char d = buf[p];
                    if (!is_digit(d)) return (ParsedURL){0};

                    port = port * 10 + (uint32_t)(d -'0');
                    if (port > 65535) return (ParsedURL){0};
                    p++;
                }
                r.port = (uint16_t)port;
            }
        }

        i = auth_end;
    }

    uint32_t path_start = i;
    uint32_t path_end = path_start;
    while (path_end < len) {
        char c = buf[path_end];
        if (c == '?' || c == '#') break;
        path_end++;
    }

    if (path_end > path_start) {
        r.path.ptr = (uintptr_t)(buf + path_start);
        r.path.size = path_end - path_start;
    }

    i = path_end;

    if (i < len && buf[i] == '?') {
        uint32_t qs = i + 1;
        uint32_t qe = qs;
        while (qe < len && buf[qe] != '#') qe++;
        
        if (qe > qs) {
            r.query.ptr = (uintptr_t)(buf + qs);
            r.query.size = qe - qs;
        }
        i = qe;
    }

    if (i < len && buf[i] == '#') {
        uint32_t fs = i + 1;
        if (fs < len) {
            r.fragment.ptr = (uintptr_t)(buf + fs);
            r.fragment.size = len - fs;
        }
    }

    r.ok = true;
    return r;
}

string url_request_path(const ParsedURL *url, const string *base_path) {
    string out = string_repeat('\0', 0);
    if (!url || !url->ok) return out;

    const char *path = (const char*)url->path.ptr;
    uint32_t path_len = url->path.size;
    const char *base = base_path && base_path->data ? base_path->data : 0;
    uint32_t base_len = base ? base_path->length : 0;

    if (path_len && path[0] == '/') string_append_bytes(&out, path, path_len);
    else if (path_len) {
        uint32_t base_end = base_len;
        for (uint32_t i = 0; i < base_end; i++) {
            char c = base[i];
            if (c == '?' || c == '#') {
                base_end = i;
                break;
            }
        }

        uint32_t slash = 0;
        for (uint32_t i = 0; i < base_end; i++) if (base[i] == '/') slash = i + 1;
        if (slash) string_append_bytes(&out, base, slash);
        else string_append_bytes(&out, "/", 1);
        string_append_bytes(&out, path, path_len);
    } else if (url->query.ptr && base_len) {
        uint32_t base_end = base_len;
        for (uint32_t i = 0; i < base_end; i++) {
            char c = base[i];
            if (c == '?' || c == '#') {
                base_end = i;
                break;
            }
        }

        if (base_end) string_append_bytes(&out, base, base_end);
        else string_append_bytes(&out, "/", 1);
    } else if (base_len) {
        uint32_t base_end = base_len;
        for (uint32_t i = 0; i < base_end; i++) {
            if (base[i] == '#') {
                base_end = i;
                break;
            }
        }

        if (base_end) string_append_bytes(&out, base, base_end);
        else string_append_bytes(&out, "/", 1);
    } else string_append_bytes(&out, "/", 1);

    if (url->query.ptr && url->query.size) {
        string_append_bytes(&out, "?", 1);
        string_append_bytes(&out, (const char*)url->query.ptr, url->query.size);
    }

    if (!out.length) string_append_bytes(&out, "/", 1);
    return out;
}
