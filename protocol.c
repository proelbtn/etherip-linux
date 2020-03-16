#include "protocol.h"

#include <net/protocol.h>

static int etherip_rcv(struct sk_buff *skb)
{
  kfree_skb(skb);
  return 0;
}

static int etherip_v4_err(struct sk_buff *skb, __u32 info)
{
  return 0;
}

static int etherip_v6_err(struct sk_buff *skb,
              struct inet6_skb_parm *opt,
              u8 type, u8 code, int offset,
              __be32 info)
{
  return 0;
}

static const struct net_protocol etherip_v4_protocol = {
  .handler = etherip_rcv,
  .err_handler = etherip_v4_err,
  .no_policy = 1,
  .netns_ok = 1,
  .icmp_strict_tag_validation = 1
};

static const struct inet6_protocol etherip_v6_protocol = {
  .handler = etherip_rcv,
  .err_handler = etherip_v6_err,
  .flags = INET6_PROTO_NOPOLICY | INET6_PROTO_FINAL
};

int etherip_protocol_init(void)
{
  int ret = 0;

  ret = inet_add_protocol(&etherip_v4_protocol, IPPROTO_ETHERIP);
  if (ret)
    goto out1;

  ret = inet6_add_protocol(&etherip_v6_protocol, IPPROTO_ETHERIP);
  if (ret)
    goto out2;
  
  return ret;

out2:
  inet_del_protocol(&etherip_v4_protocol, IPPROTO_ETHERIP);
out1:
  return ret;
}

void etherip_protocol_exit(void)
{
  inet6_del_protocol(&etherip_v6_protocol, IPPROTO_ETHERIP);
  inet_del_protocol(&etherip_v4_protocol, IPPROTO_ETHERIP);
}
