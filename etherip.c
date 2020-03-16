#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <net/protocol.h>

#define DRIVER_AUTHOR "Ryoga Saito <contact@proelbtn.com>"
#define DRIVER_DESCRIPTION "EtherIP implementation for Linux Kernel"

#define IPPROTO_ETHERIP 97

int etherip_rcv(struct sk_buff *skb)
{
  kfree_skb(skb);
  return 0;
}

int etherip_v4_err(struct sk_buff *skb, __u32 info)
{
  return 0;
}

int	etherip_v6_err(struct sk_buff *skb,
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

static int __init etherip_init(void)
{
  printk(KERN_INFO "EtherIP loaded\n");

  if (inet_add_protocol(&etherip_v4_protocol, IPPROTO_ETHERIP) < 0)
    return -EAGAIN;

  if (inet6_add_protocol(&etherip_v6_protocol, IPPROTO_ETHERIP) < 0)
    return -EAGAIN;

  return 0;
}

static void __exit etherip_exit(void)
{
  inet6_del_protocol(&etherip_v6_protocol, IPPROTO_ETHERIP);
  inet_del_protocol(&etherip_v4_protocol, IPPROTO_ETHERIP);

  printk(KERN_INFO "EtherIP unloaded");
}

module_init(etherip_init);
module_exit(etherip_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESCRIPTION);
