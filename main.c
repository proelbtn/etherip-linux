#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>

#include "device.h"
#include "protocol.h"

#define DRIVER_LICENSE "GPL"
#define DRIVER_AUTHOR "Ryoga Saito <contact@proelbtn.com>"
#define DRIVER_DESCRIPTION "EtherIP implementation for Linux Kernel"

static int __init etherip_init(void)
{
  int ret;

  ret = etherip_device_init();
  if (ret) goto out1;

  ret = etherip_protocol_init();
  if (ret) goto out2;

  return ret;

out2:
  etherip_device_exit();
out1:
  return ret;
}

static void __exit etherip_exit(void)
{
  etherip_protocol_exit();
  etherip_device_exit();
}

module_init(etherip_init);
module_exit(etherip_exit);

MODULE_LICENSE(DRIVER_LICENSE);
MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESCRIPTION);
