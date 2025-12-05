#include "../include/prompt.h"

void prompt_peer(char peer[], size_t n)
{
  uint8_t first = 1;
  struct odon_addr_exch *exch = odon_exchaddrs_init();
  for (struct odon_addr_exch *curr = exch; curr != NULL; curr = curr->next)
  {
    char encoded[MAX_EXCH_ENCODED_LENGTH];
    fmt_conn_base64url_encode(curr->type, curr->conn_data, encoded);

    if (first)
    {
      first = 0;
      printf("%s", encoded);
    }
    else
    {
      printf(":%s", encoded);
    }
  }
  odon_exchaddrs_free(exch);

  printf("\n  \\__peer: ");
  fgets(peer, (int)n, stdin);

  // strip newline char
  char *nl = strchr(peer, '\n');
  if (nl) {
    *nl = '\0';
  }
}
