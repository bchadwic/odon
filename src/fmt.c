#include "../include/fmt.h"

static const char b64url_encode_map[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_";
static const uint8_t b64url_decode_map[256] = {
    ['A'] = 0,
    ['B'] = 1,
    ['C'] = 2,
    ['D'] = 3,
    ['E'] = 4,
    ['F'] = 5,
    ['G'] = 6,
    ['H'] = 7,
    ['I'] = 8,
    ['J'] = 9,
    ['K'] = 10,
    ['L'] = 11,
    ['M'] = 12,
    ['N'] = 13,
    ['O'] = 14,
    ['P'] = 15,
    ['Q'] = 16,
    ['R'] = 17,
    ['S'] = 18,
    ['T'] = 19,
    ['U'] = 20,
    ['V'] = 21,
    ['W'] = 22,
    ['X'] = 23,
    ['Y'] = 24,
    ['Z'] = 25,

    ['a'] = 26,
    ['b'] = 27,
    ['c'] = 28,
    ['d'] = 29,
    ['e'] = 30,
    ['f'] = 31,
    ['g'] = 32,
    ['h'] = 33,
    ['i'] = 34,
    ['j'] = 35,
    ['k'] = 36,
    ['l'] = 37,
    ['m'] = 38,
    ['n'] = 39,
    ['o'] = 40,
    ['p'] = 41,
    ['q'] = 42,
    ['r'] = 43,
    ['s'] = 44,
    ['t'] = 45,
    ['u'] = 46,
    ['v'] = 47,
    ['w'] = 48,
    ['x'] = 49,
    ['y'] = 50,
    ['z'] = 51,

    ['0'] = 52,
    ['1'] = 53,
    ['2'] = 54,
    ['3'] = 55,
    ['4'] = 56,
    ['5'] = 57,
    ['6'] = 58,
    ['7'] = 59,
    ['8'] = 60,
    ['9'] = 61,

    ['-'] = 62,
    ['_'] = 63,
};

void fmt_conn_base64url_encode(enum exch_type type,
                               uint8_t conn_data[MAX_EXCH_DATA_LENGTH],
                               char encoded[MAX_EXCH_ENCODED_LENGTH])
{

    uint8_t *p = conn_data;
    int n = (int)type;
    int offset = 0; // base64 offset

    for (int i = 0; i < n; i += 3)
    {
        uint32_t chunk = (uint32_t)(p[i] << 16);

        int rem = n - i;
        if (rem > 1)
        {
            chunk |= (uint32_t)(p[i + 1] << 8);
        }
        if (rem > 2)
        {
            chunk |= (uint32_t)(p[i + 2]);
        }

        encoded[offset++] = b64url_encode_map[(chunk >> 18) & 0x3F];
        encoded[offset++] = b64url_encode_map[(chunk >> 12) & 0x3F];
        if (rem > 1)
        {
            encoded[offset++] = b64url_encode_map[(chunk >> 6) & 0x3F];
        }
        if (rem > 2)
        {
            encoded[offset++] = b64url_encode_map[chunk & 0x3F];
        }
    }

    encoded[offset] = '\0';
}

void fmt_conn_base64url_decode(enum exch_type type,
                               char encoded[MAX_EXCH_ENCODED_LENGTH],
                               uint8_t conn_data[MAX_EXCH_DATA_LENGTH])
{
    uint8_t *p = conn_data;
    int n = (int)type;
    int offset = 0; // conn_data offset

    for (int i = 0; i < n; i += 3)
    {
        uint32_t chunk = 0;

        int rem = n - i;
        chunk |= (uint32_t)(b64url_decode_map[(unsigned char)encoded[offset++]] << 18);
        chunk |= (uint32_t)(b64url_decode_map[(unsigned char)encoded[offset++]] << 12);
        if (rem > 1)
        {
            chunk |= (uint32_t)(b64url_decode_map[(unsigned char)encoded[offset++]] << 6);
        }
        if (rem > 2)
        {
            chunk |= (uint32_t)(b64url_decode_map[(unsigned char)encoded[offset++]]);
        }

        p[i] = (uint8_t)((chunk >> 16) & 0xFF);
        if (rem > 1)
        {
            p[i + 1] = (uint8_t)((chunk >> 8) & 0xFF);
        }
        if (rem > 2)
        {
            p[i + 2] = (uint8_t)(chunk & 0xFF);
        }
    }
}

void fmt_conn_plaintext(enum exch_type type, uint8_t conn_data[MAX_EXCH_DATA_LENGTH], char plaintext[MAX_EXCH_PLAINTEXT_LENGTH])
{
    uint8_t *p = conn_data;

    if (type == IPV4_LOCAL_AREA)
    {
        snprintf(plaintext, MAX_EXCH_PLAINTEXT_LENGTH, "%u.%u.%u.%u", p[0], p[1], p[2], p[3]);
        return;
    }

    if (type == IPV4_REFLEXIVE)
    {
        int port = ((int)p[4] << 8) | p[5];
        snprintf(plaintext, MAX_EXCH_PLAINTEXT_LENGTH, "%u.%u.%u.%u:%u", p[0], p[1], p[2], p[3], port);
        return;
    }

    // unknown type
    plaintext[0] = '\0';
}

uint32_t fmt_conn_uint32(uint8_t conn_data[MAX_EXCH_DATA_LENGTH])
{
    printf("hello world\n");
    return ((uint32_t)conn_data[0]) |
           ((uint32_t)conn_data[1] << 8) |
           ((uint32_t)conn_data[2] << 16) |
           ((uint32_t)conn_data[3] << 24);
}

int fmt_conn_splitnext(char **peer, char **start, size_t *len)
{
    static const char delim = ':';
    if (**peer == '\0')
    {
        return 0;
    }

    char *p = *peer;
    *start = p;

    while (*p != '\0' && *p != delim)
    {
        p++;
    }

    *len = (size_t)(p - *start);

    if (*p != '\0')
    {
        *peer = p + 1;
    }
    else
    {
        *peer = p;
    }
    return 1;
}
