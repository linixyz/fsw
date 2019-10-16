#include "fsw/coroutine_http.h"
#include "fsw/coroutine_http_server.h"
#include "fsw/coroutine.h"

using fsw::Coroutine;
using fsw::coroutine::http::Request;
using fsw::coroutine::http::Response;
using fsw::coroutine::http::Server;
using fsw::coroutine::Socket;

void handler(Request *request, Response *response)
{
    std::string response_body = "hello world";

    response->header["Content-Type"] = "text/html";
    response->header["Connection"] = "close";
    response->header["Content-Length"] = std::to_string(response_body.length());
    response->end(response_body);
    return;
}

int main(int argc, char const *argv[])
{
    fsw_event_init();

    Coroutine::create([](void *arg)
    {
        char ip[] = "127.0.0.1";

        Server *serv = new Server(ip, 80);
        serv->set_handler(string("/index"), handler);
        serv->start();
    });

    fsw_event_wait();

    return 0;
}