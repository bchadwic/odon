#include "../include/recv.h"

static int run(FILE *output);

int recv_cmd(int argc, char *argv[])
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
  FILE *output = fopen(filename, "wb");
  if (output == NULL)
  {
    return errno;
  }

  int res = run(output);
  fclose(output);
  return res;
}

int run(FILE *output)
{
  uint32_t src_addr = odon_exch_srcaddr();
  if (src_addr == 0)
  {
    return ERR_INVALID_SRC_ADDR;
  }

  uint32_t dst_addr = odon_exch_dstaddr();
  if (dst_addr == 0)
  {
    return ERR_INVALID_EXCH_STR;
  }

  struct sockaddr_in src = {
      .sin_family = AF_INET,
      .sin_addr.s_addr = htonl(INADDR_ANY),
      .sin_port = htons(52887),
  };

  struct sockaddr_in dst = {
      .sin_family = AF_INET,
      .sin_addr.s_addr = htonl(dst_addr),
      .sin_port = htons(52888),
  };

  socklen_t src_len = sizeof(src);
  socklen_t dst_len = sizeof(dst);

  printf("receiving...\n");
  struct odon_conn conn = {0};
  if (odon_init(&conn, &src, src_len, &dst, dst_len) < 0)
  {
    odon_free(&conn);
    return -1;
  }

  if (odon_recv(&conn, output) < 0)
  {
    odon_free(&conn);
    return -1;
  }

  odon_free(&conn);
  return 0;
}