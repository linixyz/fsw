#ifndef COROUTINE_HTTP_H
#define COROUTINE_HTTP_H

#include "fsw.h"
#include "coroutine_socket.h"
#include "http_parser.h"

using fsw::coroutine::Socket;

namespace fsw { namespace coroutine { namespace http {

class Ctx;

class Request
{
public:
    int version;
    char *path;
    uint32_t path_len;
    const char *ext;
    uint32_t ext_len;
    uint8_t post_form_urlencoded;
    size_t body_length;
    std::map<char*, char*> header;

    Request();
    ~Request();
};

class Ctx
{
public:
    Socket *conn;
    http_parser parser;
    Request request;
    char *current_header_name;
    size_t current_header_name_len;

    Ctx(Socket *_conn);
    ~Ctx();
    size_t parse(ssize_t recved);
};
}
}
}

#endif	/* COROUTINE_HTTP_H */