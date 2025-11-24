#pragma once

#include <inttypes.h>
#include "./exch.h"

#define MAX_EXCH_ENCODED_LENGTH 9
#define MAX_EXCH_PLAINTEXT_LENGTH 22

void fmt_conn_base64url_encode(enum exch_type type,
                               uint8_t conn_data[MAX_EXCH_DATA_LENGTH],
                               char encoded[MAX_EXCH_ENCODED_LENGTH]);

void fmt_conn_base64url_decode(enum exch_type type,
                               char encoded[MAX_EXCH_ENCODED_LENGTH],
                               uint8_t conn_data[MAX_EXCH_DATA_LENGTH]);

void fmt_conn_plaintext(enum exch_type type,
                        uint8_t conn_data[MAX_EXCH_DATA_LENGTH],
                        char plaintext[MAX_EXCH_PLAINTEXT_LENGTH]);

int fmt_conn_splitnext(char **peer, char **start, size_t *len);