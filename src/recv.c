#include "../include/recv.h"

int recv_cmd(struct sockaddr_in *src, socklen_t src_len, struct sockaddr_in *dst, socklen_t dst_len, char *filename)
{
  FILE *output = fopen(filename, "wb");
  if (output == NULL)
  {
    return -1;
  }

  printf("receiving...\n");
  struct odon_conn conn = {0};
  if (odon_init(&conn, src, src_len, dst, dst_len) < 0)
  {
    fclose(output);
    odon_free(&conn);
    return -1;
  }

  if (odon_recv(&conn, output) < 0)
  {
    fclose(output);
    odon_free(&conn);
    return -1;
  }

  fclose(output);
  odon_free(&conn);
  return 0;
}
