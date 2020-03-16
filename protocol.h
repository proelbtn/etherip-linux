#pragma once

#include <linux/init.h>

#define IPPROTO_ETHERIP 97

int __init etherip_protocol_init(void);
void etherip_protocol_exit(void);
