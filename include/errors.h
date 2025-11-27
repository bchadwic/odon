#pragma once

#include <stdio.h>
#include <string.h>

enum odon_error
{
  OK = 0,
  ERR_NO_CMD = -0xFFFF,
  ERR_INVALID_CMD,
  ERR_NO_POS_ARG,
  ERR_TOO_MANY_ARGS,
  ERR_INVALID_EXCH_STR,
};

int odon_perror(const char *bin, int err);