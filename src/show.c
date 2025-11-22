#include "../include/show.h"

int show_cmd(int argc, char *argv[])
{
  (void)argc;
  (void)argv;

  struct odon_addr_exch *exch = odon_exchaddrs_init();
  for (struct odon_addr_exch *curr = exch; curr != NULL; curr = curr->next)
  {
    char encoded[MAX_EXCH_ENCODED_LENGTH];
    fmt_conn_base64url(curr->type, curr->conn_data, encoded);

    char plaintext[MAX_EXCH_PLAINTEXT_LENGTH];
    fmt_conn_plaintext(curr->type, curr->conn_data, plaintext);

    printf("%s - %s\n", encoded, plaintext);
  }
  odon_exchaddrs_free(exch);
  return 0;
}