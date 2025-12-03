#include "../include/send.h"
#include <string.h>

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
  char peer[MAX_EXCH_ENCODED_LENGTH * 10];
  prompt_peer(peer, sizeof(peer));

  char *p = peer;
  char *start;
  size_t len;

  if (!fmt_conn_splitnext(&p, &start, &len))
  {
    return ERR_INVALID_CMD;
  }

  if (len != IPV4_BASE64_URL_LENGTH)
  {
    return ERR_INVALID_EXCH_STR;
  }

  enum exch_type type = IPV4_LOCAL_AREA;
  uint8_t conn_data[MAX_EXCH_DATA_LENGTH];
  fmt_conn_base64url_decode(type, start, conn_data);

  char plaintext[MAX_EXCH_PLAINTEXT_LENGTH];
  fmt_conn_plaintext(type, conn_data, plaintext);

  printf("peer ip: %s\n", plaintext);
  printf("raw: %d\n", fmt_conn_uint32(conn_data));

  struct sockaddr_in src = {
      .sin_family = AF_INET,
      .sin_addr.s_addr = htonl(INADDR_LOOPBACK),
      .sin_port = htons(52888),
  };

  struct sockaddr_in dst = {
      .sin_family = AF_INET,
      .sin_addr.s_addr = htonl(INADDR_LOOPBACK),
      .sin_port = htons(52887),
  };

  socklen_t src_len = sizeof(src);
  socklen_t dst_len = sizeof(dst);

  FILE *input = fopen(filename, "rb");
  if (input == NULL)
  {
    return -1;
  }

  printf("sending...\n");
  struct odon_conn conn = {0};
  if (odon_init(&conn, &src, src_len, &dst, dst_len) < 0)
  {
    fclose(input);
    odon_free(&conn);
    return -1;
  }

  if (odon_send(&conn, input) < 0)
  {
    fclose(input);
    odon_free(&conn);
    return -1;
  }

  fclose(input);
  odon_free(&conn);
  return 0;
}