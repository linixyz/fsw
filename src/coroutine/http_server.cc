#include "coroutine_http_server.h"
#include "socket.h"
#include "log.h"
#include "coroutine.h"

using fsw::coroutine::HttpServer;
using fsw::coroutine::Socket;
using fsw::Coroutine;

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

        // Coroutine::create(handler, (void *)conn);
    }
    return true;
}

bool HttpServer::on_accept()
{

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