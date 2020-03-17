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

static int callback(struct nl_msg *msg, void *arg) {
  for (int i = 0; i < 256; i++) {
    printf("%c", ((char *)msg)[i]);
  }
  printf("\n");
  return 0;
}

int main(int argc, const char *argv[]) {
  if (argc != 4) {
    printf("Usage:\n");
    printf("  %s DEVNAME LOCALADDR REMOTEADDR\n", argv[0]);
    return 1;
  }

  struct nl_sock *sk;
  struct nl_msg *msg;
  struct nlattr *linkinfo, *infodata;
  struct ifinfomsg ifi = {
    .ifi_family = AF_UNSPEC,
    .ifi_type = 0,
    .ifi_index = 0,
    .ifi_flags = 0,
    .ifi_change = 0,
  };

  struct in6_addr local_addr, remote_addr;

  if (inet_pton(AF_INET6, argv[2], &local_addr) != 1) {
    printf("LOCALADDR is not correct\n");
    return 1;
  }

  if (inet_pton(AF_INET6, argv[3], &remote_addr) != 1) {
    printf("LOCALADDR is not correct\n");
    return 1;
  }

  struct nl_data *local_addr_data, *remote_addr_data;
  local_addr_data = nl_data_alloc(&local_addr, sizeof(local_addr));
  remote_addr_data = nl_data_alloc(&remote_addr, sizeof(remote_addr));

  sk = nl_socket_alloc();

  nl_socket_modify_cb(sk, NL_CB_INVALID, NL_CB_DEBUG, callback, NULL);
  nl_socket_modify_cb(sk, NL_CB_VALID, NL_CB_DEBUG, callback, NULL);
  nl_socket_modify_cb(sk, NL_CB_FINISH, NL_CB_DEBUG, callback, NULL);

  nl_connect(sk, NETLINK_ROUTE);

  if (!(msg = nlmsg_alloc_simple(RTM_NEWLINK, NLM_F_CREATE))) return 1;

  if (nlmsg_append(msg, &ifi, sizeof(ifi), NLMSG_ALIGNTO) < 0) return 1;

  NLA_PUT_STRING(msg, IFLA_IFNAME, argv[1]);
  if (!(linkinfo = nla_nest_start(msg, IFLA_LINKINFO))) return 1;
    NLA_PUT_STRING(msg, IFLA_INFO_KIND, "etherip");
    if (!(infodata = nla_nest_start(msg, IFLA_INFO_DATA))) return 1;
      NLA_PUT_DATA(msg, IFLA_ETHERIP_LOCAL_ADDR, local_addr_data);
      NLA_PUT_DATA(msg, IFLA_ETHERIP_REMOTE_ADDR, remote_addr_data);
    nla_nest_end(msg, infodata);
  nla_nest_end(msg, linkinfo);

  nl_send_auto(sk, msg);

  nl_recvmsgs_default(sk);

  nl_socket_free(sk);

nla_put_failure:
  return 1;
}
