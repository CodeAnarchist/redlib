#pragma once
#include "types.h"
#include "net/interface_types.h"
#include "net/network_types.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    NET_CTRL_OBJ_NONE = 0,
    NET_CTRL_OBJ_LINK = 1,
    NET_CTRL_OBJ_ADDR = 2,
    NET_CTRL_OBJ_ROUTE = 3,
    NET_CTRL_OBJ_NEIGH = 4,
    NET_CTRL_OBJ_FIREWALL = 5
} NetCtrlObject;

typedef enum {
    NET_CTRL_OP_GET = 1,
    NET_CTRL_OP_UPD = 2,
    NET_CTRL_OP_DEL = 3,
    NET_CTRL_OP_ADD = 4,
} NetCtrlOp;

typedef enum {
    NET_CTRL_F_REQUEST = 1 << 0,
    NET_CTRL_F_RESPONSE = 1 << 1
} NetCtrlFlags;

typedef enum {
    NET_CTRL_EXT_NONE = 0,
    NET_CTRL_EXT_IFINDEX = 1,
    NET_CTRL_EXT_IFNAME = 2,
    NET_CTRL_EXT_FLAGS = 3,
    NET_CTRL_EXT_MTU = 4,
    NET_CTRL_EXT_METRIC = 5,
    NET_CTRL_EXT_ADDRESS = 6,
    NET_CTRL_EXT_PREFIX_LEN = 7,
    NET_CTRL_EXT_GATEWAY = 8,
    NET_CTRL_EXT_PROTOCOL = 9,
    NET_CTRL_EXT_MAC = 10,
    NET_CTRL_EXT_L3_ID = 11,
    NET_CTRL_EXT_TTL_MS = 12,
    NET_CTRL_EXT_STATE = 13,
    NET_CTRL_EXT_CONFIG = 14,
    NET_CTRL_EXT_KIND = 15,
    NET_CTRL_EXT_DAD_STATE = 16,
    NET_CTRL_EXT_RULE_ID = 17,
    NET_CTRL_EXT_ACTION = 18,
    NET_CTRL_EXT_DIRECTION = 19,
    NET_CTRL_EXT_PORT_FROM = 20,
    NET_CTRL_EXT_PORT_TO = 21,
} NetCtrlExtAttr;

typedef enum {
    NET_CTRL_NEIGH_F_STATIC = 1 << 0,
    NET_CTRL_NEIGH_F_ROUTER = 1 << 1,
} NetCtrlNeighFlags;

typedef enum {
    NET_CTRL_FIREWALL_ALLOW = 1,
    NET_CTRL_FIREWALL_DENY = 2
} NetCtrlFirewallAction;

typedef enum {
    NET_CTRL_FIREWALL_IN = 1,
    NET_CTRL_FIREWALL_OUT = 2
} NetCtrlFirewallDirection;

typedef struct NetCtrlMsg {
    uint16_t object;
    uint16_t op;
    uint16_t flags;
    uint16_t reserved;
    uint32_t length;
    int32_t status;
} NetCtrlMsg;

typedef struct NetCtrlAttr {
    uint16_t ext;
    uint16_t length;
} NetCtrlAttr;

typedef struct NetCtrlLinkInfo {
    uint8_t ifindex;
    uint8_t up;
    uint8_t kind;
    uint8_t reserved;
    uint16_t metric;
    uint16_t mtu;
    uint8_t ipv4_count;
    uint8_t ipv6_count;
    char name[16];
} NetCtrlLinkInfo;

typedef struct NetCtrlPrefix {
    uint8_t ifindex;
    l3_id_t l3_id;
    uint8_t prefix_len;
    uint8_t reserved;
    net_l4_endpoint address;
    net_l4_endpoint gateway;
} NetCtrlPrefix;

typedef struct NetCtrlAddrInfo {
    NetCtrlPrefix prefix;
    int16_t config;
    uint16_t mtu;
    uint32_t epoch;
    uint8_t kind;
    uint8_t dad_state;
    uint16_t reserved;
} NetCtrlAddrInfo;

typedef struct NetCtrlRouteInfo {
    NetCtrlPrefix prefix;
    uint16_t metric;
    uint16_t reserved;
    uint32_t route_epoch;
} NetCtrlRouteInfo;

typedef struct NetCtrlNeighInfo {
    uint8_t ifindex;
    uint8_t state;
    uint16_t flags;
    uint32_t ttl_ms;
    uint32_t router_lifetime_ms;
    net_l4_endpoint address;
    uint8_t mac[6];
} NetCtrlNeighInfo;

typedef struct NetCtrlFirewallState {
    uint8_t enabled;
    uint8_t default_in;
    uint8_t default_out;
    uint8_t reserved;
    uint32_t rule_count;
} NetCtrlFirewallState;

typedef struct NetCtrlFirewallRule {
    uint32_t id;
    uint8_t action;
    uint8_t direction;
    uint8_t protocol;
    uint8_t ip_version;
    uint8_t prefix_len;
    uint8_t reserved;
    uint16_t port_from;
    uint16_t port_to;
    uint8_t address[16];
} NetCtrlFirewallRule;

#define NET_CTRL_MSG_DATA(m) ((void*)((uint8_t*)(m) + sizeof(NetCtrlMsg)))
#define NET_CTRL_MSG_CONST_DATA(m) ((const void*)((const uint8_t*)(m) + sizeof(NetCtrlMsg)))
#define NET_CTRL_MSG_PAYLOAD_LEN(m) (((m)->length >= sizeof(NetCtrlMsg)) ? ((m)->length - (uint32_t)sizeof(NetCtrlMsg)) : 0)
#define NET_CTRL_ATTR_DATA(a) ((void*)((uint8_t*)(a) + sizeof(NetCtrlAttr)))
#define NET_CTRL_ATTR_CONST_DATA(a) ((const void*)((const uint8_t*)(a) + sizeof(NetCtrlAttr)))
#define NET_CTRL_ATTR_NEXT(a) ((NetCtrlAttr*)((uint8_t*)NET_CTRL_ATTR_DATA(a) + (a)->length))
#define NET_CTRL_HAS(a, ext) (((a)->present & (1u << (ext))) != 0)

#ifdef __cplusplus
}
#endif
