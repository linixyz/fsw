#include "coroutine_http_server.h"
#include "coroutine_http.h"
#include "socket.h"
#include "log.h"
#include "coroutine.h"
#include "http_parser.h"

using fsw::coroutine::http::Server;
using fsw::coroutine::http::Ctx;
using fsw::coroutine::Socket;
using fsw::Coroutine;

Server::Server(char *host, int port)
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

Server::~Server()
{
}

bool Server::start()
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

bool Server::on_accept(Socket *conn)
{
    ssize_t recved;
    Ctx *ctx = new Ctx(conn);

    http_parser_init(&ctx->parser, HTTP_REQUEST);

    recved = conn->recv(conn->get_read_buf(), READ_BUF_MAX_SIZE);

    /* Start up / continue the parser.
    * Note we pass recved==0 to signal that EOF has been received.
    */
    ctx->parse(recved);
    return true;
}

bool Server::shutdown()
{
    running = false;
    return true;
}

void Server::set_handler(string pattern, handle_func_t fn)
{
    handlers[pattern] = fn;
}

handle_func_t Server::get_handler(string pattern)
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