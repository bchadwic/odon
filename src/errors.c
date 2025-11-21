#include "../include/errors.h"

const char *error_string(enum odon_error_code code)
{
    switch (code)
    {
    case ODON_ERR_NO_CMD:
        return "";
    }
    return "";
}