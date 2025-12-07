#include "../include/errors.h"

int odon_perror(const char *bin, int err)
{
  if (err > ERR_ODON_UPPER_BOUND)
  {
    return fprintf(stderr, "%s: %s\n", bin, strerror(errno));
  }

  switch (err)
  {
  case ERR_NO_CMD:
    return fprintf(stderr, "%s: missing command\n", bin);
  case ERR_INVALID_CMD:
    return fprintf(stderr, "%s: invalid command\n", bin);
  case ERR_NO_POS_ARG:
    return fprintf(stderr, "%s: missing argument\n", bin);
  case ERR_TOO_MANY_ARGS:
    return fprintf(stderr, "%s: too many arguments\n", bin);
  case ERR_INVALID_EXCH_STR:
    return fprintf(stderr, "%s: invalid exchange connection string\n", bin);
  case ERR_INVALID_SRC_ADDR:
    return fprintf(stderr, "%s: invalid source address, check network interface\n", bin);
  default:
    return fprintf(stderr, "%s: unknown error code: %d\n", bin, err);
  }
}