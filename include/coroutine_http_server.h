#ifndef COROUTINE_HTTP_SERVER_H
#define COROUTINE_HTTP_SERVER_H

#include "fsw.h"
#include "coroutine.h"
#include "coroutine_socket.h"

using namespace std;

typedef void (*handle_func_t)(void*);

namespace fsw { namespace coroutine {
class HttpServer
{
private:
    Socket *socket;
    map<string, handle_func_t> handlers;
    bool running;
    
public:
    HttpServer(char *host, int port);
    ~HttpServer();
    bool start();
    bool on_accept(Socket *conn);
    bool shutdown();
    void set_handler(string pattern, handle_func_t fn);
    handle_func_t get_handler(string pattern);
};
}
}

#endif	/* COROUTINE_HTTP_SERVER_H */