#pragma once
#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    IPV4_CFG_DISABLED = -1,
    IPV4_CFG_DHCP = 0,
    IPV4_CFG_STATIC = 1
} ipv4_cfg_t;

typedef enum {
    IPV6_DAD_NONE = 0,
    IPV6_DAD_IN_PROGRESS = 1,
    IPV6_DAD_FAILED = 2,
    IPV6_DAD_OK = 3
} ipv6_dad_state_t;

typedef enum {
    IPV6_ADDRK_GLOBAL = 0x01,
    IPV6_ADDRK_LINK_LOCAL = 0x02
} ipv6_addr_kind_t;

typedef enum {
    IPV6_CFG_DISABLE = -1,
    IPV6_CFG_STATIC = 0x01,
    IPV6_CFG_SLAAC = 0x02,
    IPV6_CFG_DHCPV6 = 0x04,
    IPV6_CFG_STATELESS = IPV6_CFG_SLAAC | IPV6_CFG_DHCPV6
} ipv6_cfg_t;

#ifdef __cplusplus
}
#endif
