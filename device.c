#include "device.h"

#include <linux/if_ether.h>
#include <linux/if_arp.h>
#include <linux/in6.h>
#include <linux/init.h>
#include <linux/ipv6.h>
#include <linux/random.h>
#include <linux/socket.h>
#include <net/rtnetlink.h>

static int etherip_open(struct net_device *dev)
{
  return 0;
}

static int etherip_stop(struct net_device *dev)
{
  return 0;
}

static netdev_tx_t etherip_xmit(struct sk_buff *skb, struct net_device *dev)
{
  kfree_skb(skb);
  return NETDEV_TX_OK;
}

static const struct net_device_ops etherip_netdev_ops = {
	.ndo_open		= etherip_open,
	.ndo_stop		= etherip_stop,
	.ndo_start_xmit		= etherip_xmit,

  .ndo_set_mac_address = eth_mac_addr,
};

static size_t etherip_nl_getsize(const struct net_device *dev)
{
	return 0
    + nla_total_size(MAX_ADDR_LEN)  /* IFLA_ETHERIP_LOCAL_ADDR */
    + nla_total_size(MAX_ADDR_LEN); /* IFLA_ETHERIP_REMOTE_ADDR */
}

static const struct nla_policy etherip_nl_policy[IFLA_ETHERIP_MAX + 1] = {
  [IFLA_ETHERIP_LOCAL_ADDR] = { .type = NLA_BINARY, .len = MAX_ADDR_LEN },
  [IFLA_ETHERIP_REMOTE_ADDR] = { .type = NLA_BINARY, .len = MAX_ADDR_LEN },
};

static int etherip_validate(struct nlattr *tb[], struct nlattr *data[],
              struct netlink_ext_ack *extack)
{
  if (!data || !data[IFLA_ETHERIP_LOCAL_ADDR] || !data[IFLA_ETHERIP_REMOTE_ADDR]) {
    NL_SET_ERR_MSG(extack, "You must specify both local addr and remote addr");
    return -EINVAL;
  }

  return 0;
}

static int etherip_newlink(struct net *src_net, struct net_device *dev,
              struct nlattr *tb[], struct nlattr *data[],
              struct netlink_ext_ack *extack)
{
  int ret;
  struct etherip_device *ei = netdev_priv(dev);
  struct sockaddr sa;

  ei->net = src_net;

  dev->netdev_ops = &etherip_netdev_ops;
  dev->needed_headroom = sizeof(struct ipv6hdr) + sizeof(struct etherip_header);

  memcpy(&ei->local_addr, nla_data(data[IFLA_ETHERIP_LOCAL_ADDR]), sizeof(struct in6_addr));
  memcpy(&ei->remote_addr, nla_data(data[IFLA_ETHERIP_REMOTE_ADDR]), sizeof(struct in6_addr));

  ret = register_netdevice(dev);
  if (ret < 0) goto out1;

  sa.sa_family = ARPHRD_ETHER;
  sa.sa_data[0] = 0x72;
  prandom_bytes(sa.sa_data + 1, ETH_ALEN - 1);

  ret = dev_set_mac_address(dev, &sa, NULL);
  if (ret < 0) goto out1;

  return ret;

out1:
  return ret;
}

static void etherip_setup(struct net_device *dev)
{
  struct etherip_device *ei = netdev_priv(dev);

  dev->type = ARPHRD_ETHER;
  dev->addr_len = ETH_ALEN;
  dev->mtu = ETH_DATA_LEN - sizeof(struct ipv6hdr) - sizeof(struct etherip_header);
  dev->min_mtu = 64; // TODO: fix
  dev->max_mtu = dev->mtu;
  memset(dev->broadcast, 0xff, ETH_ALEN);

  dev->flags = IFF_BROADCAST | IFF_MULTICAST;

  memset(ei, 0, sizeof(*ei));
}

static struct rtnl_link_ops etherip_link_ops __read_mostly = {
  .kind = KBUILD_MODNAME,
  .priv_size = sizeof(struct etherip_device),

  .get_size = etherip_nl_getsize,
  .policy = etherip_nl_policy,
  .validate = etherip_validate,

  .setup = etherip_setup,
  .newlink = etherip_newlink,

  .maxtype = IFLA_ETHERIP_MAX
};

int __init etherip_device_init(void) {
  int ret;

  ret = rtnl_link_register(&etherip_link_ops);
  if (ret) goto out1;

  return ret;

out1:
  return ret;
}

void etherip_device_exit(void) {
  rtnl_link_unregister(&etherip_link_ops);
}

