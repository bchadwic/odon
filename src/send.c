#include "../include/send.h"

static int run(FILE *input);

int send_cmd(int argc, char *argv[])
{
  if (argc < 1)
  {
    return ERR_NO_POS_ARG;
  }
  else if (argc > 1)
  {
    return ERR_TOO_MANY_ARGS;
  }

  const char *filename = *argv;
  FILE *input = fopen(filename, "rb");
  if (input == NULL)
  {
    return errno;
  }

  int res = run(input);
  fclose(input);
  return res;
}

static int run(FILE *input)
{
  char peer[MAX_EXCH_ENCODED_LENGTH * 10];
  prompt_peer(peer, sizeof(peer));

  char *p = peer;
  char *start;
  size_t len;

  // currently only parsing one section
  if (!fmt_conn_splitnext(&p, &start, &len) || len != IPV4_BASE64_URL_LENGTH)
  {
    return ERR_INVALID_EXCH_STR;
  }

  enum exch_type type = IPV4_LOCAL_AREA;
  uint8_t conn_data[MAX_EXCH_DATA_LENGTH];
  fmt_conn_base64url_decode(type, start, conn_data);

  struct sockaddr_in src = {
      .sin_family = AF_INET,
      .sin_addr.s_addr = htonl(INADDR_ANY),
      .sin_port = htons(52888),
  };

  uint32_t dst_addr = fmt_conn_ipv4(conn_data);
  struct sockaddr_in dst = {
      .sin_family = AF_INET,
      .sin_addr.s_addr = htonl(dst_addr),
      .sin_port = htons(52887),
  };

  socklen_t src_len = sizeof(src);
  socklen_t dst_len = sizeof(dst);

  printf("sending...\n");
  struct odon_conn conn = {0};
  if (odon_init(&conn, &src, src_len, &dst, dst_len) < 0)
  {
    odon_free(&conn);
    return -1;
  }

  if (odon_send(&conn, input) < 0)
  {
    odon_free(&conn);
    return -1;
  }

  odon_free(&conn);
  return 0;
}