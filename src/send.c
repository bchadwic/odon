#include "../include/send.h"

int send_cmd(struct sockaddr_in *src,
             socklen_t src_len,
             struct sockaddr_in *dst,
             socklen_t dst_len,
             char *filename)
{
  FILE *input = fopen(filename, "rb");
  if (input == NULL)
  {
    return -1;
  }

  printf("sending...\n");
  struct odon_conn conn = {0};
  if (odon_init(&conn, src, src_len, dst, dst_len) < 0)
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