#ifndef COROUTINE_HTTP_H
#define COROUTINE_HTTP_H

#include "fsw.h"
#include "http_parser.h"

namespace fsw { namespace coroutine { namespace http {
struct http_request
{
    int version;
    char *path;
    uint32_t path_len;
    const char *ext;
    uint32_t ext_len;
    uint8_t post_form_urlencoded;
    size_t body_length;
};

struct ctx
{
    void *private_data;
    http_parser parser;
    http_request request;
};
}
}
}

#endif	/* COROUTINE_HTTP_H */