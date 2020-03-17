#pragma once

#include <asm/byteorder.h>
#include <linux/init.h>
#include <linux/in6.h>

enum {
  IFLA_ETHERIP_UNSPEC,
  IFLA_ETHERIP_LOCAL_ADDR,
  IFLA_ETHERIP_REMOTE_ADDR,
  __IFLA_ETHERIP_MAX
};

#define IFLA_ETHERIP_MAX (__IFLA_ETHERIP_MAX - 1)

struct etherip_header {
#if defined(__LITTLE_ENDIAN_BITFIELD)
	__u8	__rsv1:4,
        version:4;
#elif defined (__BIG_ENDIAN_BITFIELD)
	__u8	version:4,
        __rsv1:4;
#else
#error	"Please fix <asm/byteorder.h>"
#endif
  __u8 __rsv2;
};

struct etherip_device {
  struct net *net;

  struct in6_addr local_addr, remote_addr;
};

int __init etherip_device_init(void);
void etherip_device_exit(void);
