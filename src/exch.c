#include "../include/exch.h"
#include "../include/fmt.h"

struct odon_addr_exch *odon_exchaddrs_init(void)
{
  struct odon_addr_exch *exch = NULL;
  struct odon_addr_exch **tail = &exch;

  struct ifaddrs *ifaddr = NULL;
  if (getifaddrs(&ifaddr) == -1)
  {
    return NULL;
  }

  for (struct ifaddrs *ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next)
  {
    // if the address is not ipv4, is not up or is a loopback, skip
    if (ifa->ifa_addr == NULL ||
        ifa->ifa_addr->sa_family != AF_INET ||
        !(ifa->ifa_flags & IFF_UP) ||
        ifa->ifa_flags & IFF_LOOPBACK)
    {
      continue;
    }

    struct sockaddr_in *addr = (struct sockaddr_in *)ifa->ifa_addr;
    struct odon_addr_exch *ex = malloc(sizeof(struct odon_addr_exch));
    if (ex == NULL)
    {
      freeifaddrs(ifaddr);
      return NULL;
    }

    ex->next = NULL;
    ex->type = IPV4_LOCAL_AREA;
    memcpy(ex->conn_data, &addr->sin_addr.s_addr, IPV4_LOCAL_AREA);

    *tail = ex;
    tail = &ex->next;
  }

  freeifaddrs(ifaddr);
  return exch;
}

extern void odon_exchaddrs_free(struct odon_addr_exch *exch)
{
  while (exch)
  {
    struct odon_addr_exch *next = exch->next;
    free(exch);
    exch = next;
  }
  return;
}

extern uint32_t odon_exch_srcaddr(void)
{
  uint32_t src_addr = 0;

  struct odon_addr_exch *exch = odon_exchaddrs_init();
  for (struct odon_addr_exch *curr = exch; curr != NULL; curr = curr->next)
  {
    char encoded[MAX_EXCH_ENCODED_LENGTH];
    fmt_conn_base64url_encode(curr->type, curr->conn_data, encoded);
    printf("%s", encoded);

    src_addr = fmt_conn_ipv4(curr->conn_data);
  }
  odon_exchaddrs_free(exch);

  return src_addr;
}

extern uint32_t odon_exch_dstaddr(void)
{
  char peer[MAX_EXCH_ENCODED_LENGTH * 10];
  printf("\n  \\__peer: ");
  fgets(peer, (int)MAX_EXCH_ENCODED_LENGTH * 10, stdin);

  // strip newline char
  char *nl = strchr(peer, '\n');
  if (nl)
  {
    *nl = '\0';
  }

  char *p = peer;
  char *start;
  size_t len;

  // currently only parsing one section
  if (!fmt_conn_splitnext(&p, &start, &len) || len != IPV4_BASE64_URL_LENGTH)
  {
    return 0;
  }

  enum exch_type type = IPV4_LOCAL_AREA;
  uint8_t conn_data[MAX_EXCH_DATA_LENGTH];
  fmt_conn_base64url_decode(type, start, conn_data);

  return fmt_conn_ipv4(conn_data);
}
