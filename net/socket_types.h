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
#define SOCK_ERR_CONN_REFUSED -15
#define SOCK_ERR_CONN_RESET -16
#define SOCK_ERR_TIMED_OUT -17
#define SOCK_ERR_TIME_EXCEEDED -18

typedef enum {
    BIND_L3 = 0,
    BIND_L2 = 1,
    BIND_IP = 2,
    BIND_ANY = 3,
    BIND_ANY4 = 4,
    BIND_ANY6 = 5
} SockBindKind;

typedef enum : uint64_t {
    SOCK_OPT_DEBUG = 1ULL << 0,
    SOCK_OPT_KEEPALIVE = 1ULL << 1,
    SOCK_OPT_BUF_SIZE = 1ULL << 2,
    SOCK_OPT_DONTFRAG = 1ULL << 3,
    SOCK_OPT_TTL = 1ULL << 4,
    SOCK_OPT_MCAST_JOIN = 1ULL << 5,
    SOCK_OPT_RECV_TIMEOUT = 1ULL << 6,
    SOCK_OPT_SEND_TIMEOUT = 1ULL << 7,
    SOCK_OPT_SEND_BUF_SIZE = 1ULL << 8,
    SOCK_OPT_TCP_NO_DELAY = 1ULL << 9,
    //SOCK_OPT_KEEPALIVE_INTERVAL = 1ULL << 10,
    SOCK_OPT_MCAST_LEAVE = 1ULL << 11,
    SOCK_OPT_BROADCAST_ALLOWED = 1ULL << 12,
    SOCK_OPT_SPECIAL = 1ULL << 14,
    SOCK_OPT_FILTER = 1ULL << 13,
    SOCK_OPT_NONBLOCK = 1ULL << 15,
    SOCK_OPT_DONTROUTE = 1ULL << 16,
    SOCK_OPT_REUSEADDR = 1ULL << 17,
    SOCK_OPT_REUSEPORT = 1ULL << 18,
    //SOCK_OPT_RECV_LOWAT = 1u << 19, //TODO low water mark
    SOCK_OPT_TCP_MAXSEG = 1ULL << 20,
    SOCK_OPT_LINGER = 1ULL << 21,
    SOCK_OPT_TCP_SACK = 1ULL << 22,
    SOCK_OPT_TCP_DSACK = 1ULL << 23,
    //SOCK_OPT_TCP_PACING = 1u << 24,
    //SOCK_OPT_MAX_PACING_RATE = 1u << 25,
    SOCK_OPT_PACKET_TRUNK = 1ULL << 26,
    SOCK_OPT_TCP_KEEPIDLE = 1ULL << 27,
    SOCK_OPT_TCP_KEEPINTVL = 1ULL << 28,
    SOCK_OPT_TCP_KEEPCNT = 1ULL << 29,
    SOCK_OPT_TCP_USER_TIMEOUT = 1ULL << 30, //TODO edge case zero window
    SOCK_OPT_TCP_QUICKACK = 1ULL << 31,
    SOCK_OPT_TCP_CORK = 1ULL << 32,
    SOCK_OPT_TCP_SYNCNT = 1ULL << 33,
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
    //SOCK_GET_OPT_KEEPALIVE_INTERVAL,
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
    SOCK_GET_OPT_PACKET_TRUNK,
    //SOCK_GET_OPT_TCP_PACING,
    SOCK_GET_TCP_CWND,
    SOCK_GET_TCP_SSTHRESH,
    SOCK_GET_TCP_RTO_MS,
    SOCK_GET_IP_PATH_MTU,
    //SOCK_GET_TCP_INFO,
    SOCK_GET_OPT_TCP_KEEPIDLE,
    SOCK_GET_OPT_TCP_KEEPINTVL,
    SOCK_GET_OPT_TCP_KEEPCNT,
    SOCK_GET_OPT_TCP_USER_TIMEOUT,
    SOCK_GET_OPT_TCP_QUICKACK,
    SOCK_GET_OPT_TCP_CORK,
    SOCK_GET_LAST_ICMP_ERR,//0 none, otherwise (ipver << 16) | (type << 8) | errcode
    SOCK_GET_OPT_TCP_SYNCNT,
    SOCK_GET_ERROR,
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
    SockOptFlags flags;
    SockDebugLevel debug_level;
    uint32_t buf_size;
    uint32_t recv_timeout_ms;
    uint32_t send_timeout_ms;
    uint32_t send_buf_size;
    //uint32_t recv_lowat;
    uint32_t tcp_maxseg;
    uint32_t tcp_keepidle;
    uint32_t tcp_keepintvl;
    uint32_t tcp_keepcnt;
    uint32_t tcp_user_timeout_ms;
    uint32_t tcp_syncnt;
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
    l3_id_t l3_id;
    uint8_t ifindex;
    uint8_t ip[16];
} SockBindSpec;

#ifdef __cplusplus
}
#endif
