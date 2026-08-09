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
#define SOCK_ERR_EXIST      -12
#define SOCK_ERR_NOT_FOUND  -13
#define SOCK_ERR_NO_ROUTE   -14

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
    SOCK_OPT_SPECIAL = 1u << 14,
    SOCK_OPT_FILTER = 1u << 13,
    SOCK_OPT_NONBLOCK = 1u << 15,
    SOCK_OPT_DONTROUTE = 1u << 16,
    SOCK_OPT_REUSEADDR = 1u << 17,
    SOCK_OPT_REUSEPORT = 1u << 18,
    //SOCK_OPT_RECV_LOWAT = 1u << 19, //TODO low water mark
    SOCK_OPT_TCP_MAXSEG = 1u << 20,
    SOCK_OPT_LINGER = 1u << 21,
    SOCK_OPT_TCP_SACK = 1u << 22,
    SOCK_OPT_TCP_DSACK = 1u << 23,
    //SOCK_OPT_TCP_PACING = 1u << 24,
    //SOCK_OPT_MAX_PACING_RATE = 1u << 25,
} SockOptFlags;

typedef enum {
    SOCK_GET_PROTOCOL = 1000,
    SOCK_GET_OWNER_PID,
    SOCK_GET_SPECIAL_KIND,
    SOCK_GET_BOUND,
    SOCK_GET_CONNECTED,
    SOCK_GET_LISTENING,
    SOCK_GET_LOCAL_PORT,
    SOCK_GET_REMOTE_ENDPOINT,
    SOCK_GET_BIND_SPEC,
    SOCK_GET_LAST_RX_SPEC,
    SOCK_GET_RECV_QUEUED,
    SOCK_GET_SEND_QUEUED,
    SOCK_GET_TCP_STATE,
    SOCK_GET_TCP_MSS,
    SOCK_GET_TCP_RTT_MS,
    SOCK_GET_TCP_RETRANSMITS,
    SOCK_GET_MCAST_GROUPS,
    SOCK_GET_OPT_DEBUG,
    SOCK_GET_OPT_KEEPALIVE,
    SOCK_GET_OPT_BUF_SIZE,
    SOCK_GET_OPT_DONTFRAG,
    SOCK_GET_OPT_TTL,
    SOCK_GET_OPT_RECV_TIMEOUT,
    SOCK_GET_OPT_SEND_TIMEOUT,
    SOCK_GET_OPT_SEND_BUF_SIZE,
    SOCK_GET_OPT_TCP_NO_DELAY,
    SOCK_GET_OPT_KEEPALIVE_INTERVAL,
    SOCK_GET_OPT_BROADCAST_ALLOWED,
    SOCK_GET_OPT_FILTER,
    SOCK_GET_OPT_NONBLOCK,
    SOCK_GET_OPT_DONTROUTE,
    SOCK_GET_OPT_REUSEADDR,
    SOCK_GET_OPT_REUSEPORT,
    //SOCK_GET_OPT_RECV_LOWAT,
    SOCK_GET_OPT_TCP_MAXSEG,
    SOCK_GET_OPT_LINGER,
    SOCK_GET_OPT_TCP_SACK,
    SOCK_GET_OPT_TCP_DSACK,
    SOCK_GET_TCP_URGENT_REMAINING,
    //SOCK_GET_OPT_TCP_PACING,
    //SOCK_GET_OPT_MAX_PACING_RATE
} SocketGetOpt;

typedef enum{
    SOCKET_SPECIAL_NONE = 0,
    SOCKET_SPECIAL_RAW = 1,
    SOCKET_SPECIAL_CTRL = 2,
    SOCKET_SPECIAL_PACKET = 3
} SocketSpecialKind;

typedef enum {
    SOCK_DBG_LOW = 0,
    SOCK_DBG_MEDIUM = 1,
    SOCK_DBG_ALL = 2
} SockDebugLevel;

#define SOCKET_RAW_FILTER_MAX_RULES 8

typedef enum {
    SOCKET_RAW_FILTER_HAS_CODE = 1u << 0,
    SOCKET_RAW_FILTER_HAS_ID = 1u << 1,
    SOCKET_RAW_FILTER_HAS_SEQ = 1u << 2
} SocketRawFilterFlags;

typedef enum {
    SOCKET_PACKET_FILTER_HAS_ETHERTYPE = 1u << 0,
    SOCKET_PACKET_FILTER_HAS_MIN_LEN = 1u << 1,
    SOCKET_PACKET_FILTER_HAS_MAX_LEN = 1u << 2
} SocketPacketFilterFlags;

typedef struct SocketRawFilterRule {
    uint8_t type;
    uint8_t code;
    uint8_t flags;
    uint8_t reserved;
    uint16_t id;
    uint16_t seq;
} SocketRawFilterRule;

typedef struct SocketRawFilter {
    uint32_t count;
    SocketRawFilterRule rules[SOCKET_RAW_FILTER_MAX_RULES];
} SocketRawFilter;

typedef struct SocketLinger {
    uint32_t enabled;
    uint32_t timeout_ms;
} SocketLinger;

typedef struct SocketPacketFilter {
    uint32_t flags;
    uint16_t ethertype;
    uint16_t min_len;
    uint16_t max_len;
    uint16_t reserved;
} SocketPacketFilter;

typedef struct SocketOptions {
    uint32_t flags;
    SockDebugLevel debug_level;
    uint32_t buf_size;
    uint32_t keepalive_ms;
    uint32_t recv_timeout_ms;
    uint32_t send_timeout_ms;
    uint32_t send_buf_size;
    //uint32_t recv_lowat;
    uint32_t tcp_maxseg;
    //uint32_t max_pacing_rate;
    SocketLinger linger;
    uint8_t ttl;
    SocketSpecialKind special_kind;
    SocketRawFilter raw_filter;
    SocketPacketFilter packet_filter;
    uint8_t mcast_count;
    const net_l4_endpoint* mcast_groups;
} SocketOptions;

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
