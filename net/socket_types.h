#pragma once
#include "types.h"
#include "net/network_types.h"

#ifdef __cplusplus
extern "C" {
#endif

#define SOCK_OK              0
#define SOCK_ERR_INVAL      -1
#define SOCK_ERR_BOUND      -2
#define SOCK_ERR_NOT_BOUND  -3
#define SOCK_ERR_PERM       -4
#define SOCK_ERR_NO_PORT    -5
#define SOCK_ERR_SYS        -6
#define SOCK_ERR_PROTO      -7
#define SOCK_ERR_STATE      -8
#define SOCK_ERR_DNS        -9
#define SOCK_ERR_UNSUP      -10
#define SOCK_ERR_WOULDBLOCK -11

typedef enum {
    BIND_L3 = 0,
    BIND_L2 = 1,
    BIND_IP = 2,
    BIND_ANY = 3,
    BIND_ANY4 = 4,
    BIND_ANY6 = 5
} SockBindKind;

typedef enum {
    SOCK_OPT_DEBUG = 1u << 0,
    SOCK_OPT_KEEPALIVE = 1u << 1,
    SOCK_OPT_BUF_SIZE = 1u << 2,
    SOCK_OPT_DONTFRAG = 1u << 3,
    SOCK_OPT_TTL = 1u << 4,
    SOCK_OPT_MCAST_JOIN = 1u << 5,
    SOCK_OPT_RECV_TIMEOUT = 1u << 6,
    SOCK_OPT_SEND_TIMEOUT = 1u << 7,
    SOCK_OPT_SEND_BUF_SIZE = 1u << 8,
    SOCK_OPT_TCP_NO_DELAY = 1u << 9,
    SOCK_OPT_KEEPALIVE_INTERVAL = 1u << 10,
    SOCK_OPT_MCAST_LEAVE = 1u << 11,
    SOCK_OPT_BROADCAST_ALLOWED = 1u << 12,
} SockOptFlags;

typedef enum{
    SOCKET_SPECIAL_NONE = 0,
    SOCKET_SPECIAL_RAW = 1,
    SOCKET_SPECIAL_SET = 2,
    SOCKET_SPECIAL_PACKET = 3
} SocketSpecialKind;

typedef enum {
    SOCK_DBG_LOW = 0,
    SOCK_DBG_MEDIUM = 1,
    SOCK_DBG_ALL = 2
} SockDebugLevel;

typedef struct SocketExtraOptions {
    uint32_t flags;
    SockDebugLevel debug_level;
    uint32_t buf_size;
    uint32_t keepalive_ms;
    uint32_t recv_timeout_ms;
    uint32_t send_timeout_ms;
    uint32_t send_buf_size;
    uint8_t ttl;
    SocketSpecialKind special_kind; //TODO with PROTO_NONE select RAW SET or L2 packet sockets here
    uint8_t mcast_count;
    const net_l4_endpoint* mcast_groups;
} SocketExtraOptions;

typedef struct SockBindSpec{
    SockBindKind kind;
    ip_version_t ver;
    uint8_t l3_id;
    uint8_t ifindex;
    uint8_t ip[16];
} SockBindSpec;

#ifdef __cplusplus
}
#endif
