#include "../include/send.h"

int send_cmd(int argc, char *argv[])
{
  if (argc < 1)
  {
    return ERR_NO_POS_ARG;
  }
  const char *filename = *argv;

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