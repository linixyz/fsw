#ifndef COROUTINE_HTTP_H
#define COROUTINE_HTTP_H

#include "fsw.h"
#include "coroutine_socket.h"
#include "http_parser.h"

using fsw::coroutine::Socket;

namespace fsw { namespace coroutine { namespace http {

class Request
{
private:
    int version;
    char *path;
    uint32_t path_len;
    const char *ext;
    uint32_t ext_len;
    uint8_t post_form_urlencoded;
    size_t body_length;
public:
    Request();
};

class Ctx
{
private:
    Socket *private_data;
    http_parser parser;
    Request request;
public:
    Ctx(Socket *conn);
    ~Ctx();
};
}
}
}

#endif	/* COROUTINE_HTTP_H */