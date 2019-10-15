#include "coroutine_http.h"
#include "log.h"

using fsw::coroutine::http::Request;
using fsw::coroutine::http::Ctx;
using fsw::coroutine::Socket;

static int http_request_on_message_begin(http_parser *parser);
static int http_request_on_url(http_parser *parser, const char *at, size_t length);
static int http_request_on_status(http_parser *parser, const char *at, size_t length);
static int http_request_on_header_field(http_parser *parser, const char *at, size_t length);
static int http_request_on_header_value(http_parser *parser, const char *at, size_t length);
static int http_request_on_headers_complete(http_parser *parser);
static int http_request_on_body(http_parser *parser, const char *at, size_t length);
static int http_request_on_message_complete(http_parser *parser);

static int http_request_on_message_begin(http_parser *parser)
{
    fswTrace("http request on message begin");
    return 0;
}

static int http_request_on_url(http_parser *parser, const char *at, size_t length)
{
    Ctx *ctx = (Ctx *)parser->data;
    /**
     * because const char *at may be destroyed, so must copy to ctx->request.path
     */
    ctx->request.path = new char[length + 1]();
    memcpy(ctx->request.path, at, length);
    ctx->request.path_len = length;
    return 0;
}

static int http_request_on_status(http_parser *parser, const char *at, size_t length)
{
    fswTrace("http request on status");
    return 0;
}

static int http_request_on_header_field(http_parser *parser, const char *at, size_t length)
{
    Ctx *ctx = (Ctx *)parser->data;
    ctx->current_header_name = (char *) at;
    ctx->current_header_name_len = length;
    return 0;
}

static int http_request_on_header_value(http_parser *parser, const char *at, size_t length)
{
    Ctx *ctx = (Ctx *)parser->data;
    std::map<char *, char *> &headers = ctx->request.header;
    size_t header_len = ctx->current_header_name_len;
    char *header_name = new char[header_len + 1]();

    memcpy(header_name, ctx->current_header_name, header_len);
    for (size_t i = 0; i < header_len; i++)
    {
        header_name[i] = tolower(header_name[i]);
    }

    char *header_value = new char[length + 1]();
    memcpy(header_value, at, length);
    headers[header_name] = header_value;
    
    return 0;
}

static int http_request_on_headers_complete(http_parser *parser)
{
    Ctx *ctx = (Ctx *)parser->data;
    ctx->request.version = parser->http_major * 100 + parser->http_minor;
    return 0;
}

static int http_request_on_body(http_parser *parser, const char *at, size_t length)
{
    fswTrace("http request on body");
    return 0;
}

static int http_request_on_message_complete(http_parser *parser)
{
    fswTrace("http request on message complete");
    return 0;
}

static const http_parser_settings parser_settings =
{
    .on_message_begin = http_request_on_message_begin,
    .on_url = http_request_on_url,
    .on_status = http_request_on_status,
    .on_header_field = http_request_on_header_field,
    .on_header_value = http_request_on_header_value,
    .on_headers_complete = http_request_on_headers_complete,
    .on_body = http_request_on_body,
    .on_message_complete = http_request_on_message_complete,
};

Request::Request()
{
    
}

Request::~Request()
{
    delete[] path;

    /**
     * delete header name and header value
     */
    for (auto i = header.begin(); i != header.end(); i++)
    {
        delete[] i->first;
        delete[] i->second;
    }
}

Ctx::Ctx(Socket *_conn)
{
    conn = _conn;
    parser.data = this;
}

Ctx::~Ctx()
{
    delete conn;
}

size_t Ctx::parse(ssize_t recved)
{
    size_t nparsed;
    Socket *conn = this->conn;

    nparsed = http_parser_execute(&parser, &parser_settings, conn->get_read_buf(), recved);
    return nparsed;
}
