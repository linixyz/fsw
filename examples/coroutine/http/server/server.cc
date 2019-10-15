#include "fsw/coroutine_http.h"
#include "fsw/coroutine_http_server.h"
#include "fsw/coroutine.h"

using fsw::Coroutine;
using fsw::coroutine::http::Request;
using fsw::coroutine::http::Response;
using fsw::coroutine::http::Server;
using fsw::coroutine::Socket;

char response_str[] = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nConnection: close\r\nContent-Length: 11\r\n\r\nhello world\r\n";

void handler(Request *request, Response *response)
{
    /**
     * print header
     */
    for(auto elem : request->header)
    {
        std::cout << elem.first << ": " << elem.second << endl;
    }

    std::cout << "version: " << request->version << endl;
    std::cout << "method: " << request->method << endl;
    std::cout << "body: " << request->body << endl;

    std::string response_body = "fsw server";
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