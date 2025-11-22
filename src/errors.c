#include "../include/errors.h"

int odon_perror(const char *bin, int err)
{
  if (err > 0)
  {
    return fprintf(stderr, "%s: unknown error occurred\n%s\n", bin, strerror(err));
  }

  switch (err)
  {
  case ERR_NO_CMD:
    return fprintf(stderr, "%s: missing command\n", bin);
  case ERR_INVALID_CMD:
    return fprintf(stderr, "%s: invalid command\n", bin);
  case ERR_NO_POS_ARG:
    return fprintf(stderr, "%s: missing positional argument\n", bin);
  default:
    return fprintf(stderr, "%s: unknown error code: %d\n", bin, err);
  }
  return 0;
}