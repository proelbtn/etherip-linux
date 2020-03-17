#include <arpa/inet.h>

#include <netlink/netlink.h>
#include <netlink/socket.h>
#include <netlink/msg.h>
#include <netlink/attr.h>

enum {
  IFLA_ETHERIP_UNSPEC,
  IFLA_ETHERIP_LOCAL_ADDR,
  IFLA_ETHERIP_REMOTE_ADDR,
  __IFLA_ETHERIP_MAX
};

#define IFLA_ETHERIP_MAX (__IFLA_ETHERIP_MAX - 1)

int main(int argc, const char *argv[]) {
  if (argc != 2) {
    printf("Usage:\n");
    printf("  %s IFINDEX\n", argv[0]);
    return 1;
  }

  int ifindex;
  if ((ifindex = atoi(argv[1])) == 0) {
    printf("invalid ifindex\n");
    return 1;
  }

  struct nl_sock *sk;
  struct nl_msg *msg;
  struct nlattr *linkinfo, *infodata;
  struct ifinfomsg ifi = {
    .ifi_family = AF_UNSPEC,
    .ifi_type = 0,
    .ifi_index = ifindex,
    .ifi_flags = 0,
    .ifi_change = 0,
  };

  sk = nl_socket_alloc();

  nl_connect(sk, NETLINK_ROUTE);

  if (!(msg = nlmsg_alloc_simple(RTM_GETLINK, 0))) return 1;
  if (nlmsg_append(msg, &ifi, sizeof(ifi), NLMSG_ALIGNTO) < 0) return 1;

  nl_send_sync(sk, msg);

  nl_socket_free(sk);

nla_put_failure:
  return 1;
}
