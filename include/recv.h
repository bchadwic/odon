#pragma once

#include "odon.h"

int recv_cmd(struct sockaddr_in *src,
             socklen_t src_len,
             struct sockaddr_in *dst,
             socklen_t dst_len,
             char *filename);