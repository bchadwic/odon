#ifndef ERRORS_H
#define ERRORS_H

enum odon_error_code
{
    ODON_OK = 0,

    // starting with a big number so we don't interfere with std codes
    ODON_ERR_NO_CMD = -0xFFFF,
};

// returns the string associated with the error
const char *error_string(enum odon_error_code code);

#endif