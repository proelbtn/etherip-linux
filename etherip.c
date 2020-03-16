#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

#define DRIVER_AUTHOR "Ryoga Saito <contact@proelbtn.com>"
#define DRIVER_DESCRIPTION "EtherIP implementation for Linux Kernel"

static int __init etherip_init(void)
{
  printk(KERN_INFO "EtherIP loaded\n");
  return 0;
}

static void __exit etherip_exit(void)
{
  printk(KERN_INFO "EtherIP unloaded");
}

module_init(etherip_init);
module_exit(etherip_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESCRIPTION);
