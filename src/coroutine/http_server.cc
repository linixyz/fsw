#include "coroutine_http_server.h"
#include "socket.h"
#include "log.h"
#include "coroutine.h"
#include "http_parser.h"

using fsw::coroutine::HttpServer;
using fsw::coroutine::Socket;
using fsw::Coroutine;

static const http_parser_settings parser_settings =
{
    .on_message_begin = NULL,
    .on_url = NULL,
    .on_status = NULL,
    .on_header_field = NULL,
    .on_header_value = NULL,
    .on_headers_complete = NULL,
    .on_body = NULL,
    .on_message_complete = NULL,
};

HttpServer::HttpServer(char *host, int port)
{
    socket = new Socket(AF_INET, SOCK_STREAM, 0);
    if (socket->bind(FSW_SOCK_TCP, host, port) < 0)
    {
        fswWarn("Error has occurred: (errno %d) %s", errno, strerror(errno));
        return;
    }
    if (socket->listen(512) < 0)
    {
        return;
    }
}

HttpServer::~HttpServer()
{
}

bool HttpServer::start()
{
    running = true;

    while (running)
    {
        Socket* conn = socket->accept();
        if (!conn)
        {
            return false;
        }

        on_accept(conn);
        // Coroutine::create(handler, (void *)conn);
    }
    return true;
}

bool HttpServer::on_accept(Socket *conn)
{
    http_parser *parser = new http_parser();
    http_parser_init(parser, HTTP_REQUEST);
    parser->data = conn;

    size_t len = 80 * 1024;
    size_t nparsed;
    char buf[len];
    ssize_t recved;

    recved = conn->recv(buf, len);

    /* Start up / continue the parser.
    * Note we pass recved==0 to signal that EOF has been received.
    */
    nparsed = http_parser_execute(parser, &parser_settings, buf, recved);
}

bool HttpServer::shutdown()
{
    running = false;
    return true;
}

void HttpServer::set_handler(string pattern, handle_func_t fn)
{
    handlers[pattern] = fn;
}

handle_func_t HttpServer::get_handler(string pattern)
{
    for (auto i = handlers.begin(); i != handlers.end(); i++)
    {
        if (strncasecmp(i->first.c_str(), pattern.c_str(), i->first.length()) == 0)
        {
            return i->second;
        }
    }
    return nullptr;
}