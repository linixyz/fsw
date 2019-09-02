#include "error.h"
#include "log.h"

const char* fsw_strerror(int code)
{
    switch (code)
    {
    case FSW_ERROR_SESSION_CLOSED_BY_SERVER:
        return "Session closed by server";
        break;
    case FSW_ERROR_SESSION_CLOSED_BY_CLIENT:
        return "Session closed by client";
        break;
    default:
        snprintf(fsw_error, sizeof(fsw_error), "Unknown error: %d", code);
        return fsw_error;
        break;
    }
}