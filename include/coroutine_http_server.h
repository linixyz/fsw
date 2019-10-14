#ifndef COROUTINE_HTTP_SERVER_H
#define COROUTINE_HTTP_SERVER_H

#include "fsw.h"
#include "coroutine.h"
#include "coroutine_socket.h"

using namespace std;

typedef void (*handle_func_t)(void*);

namespace fsw { namespace coroutine { namespace http {
class Server
{
private:
    Socket *socket;
    map<string, handle_func_t> handlers;
    bool running;
    
public:
    Server(char *host, int port);
    ~Server();
    bool start();
    bool on_accept(Socket *conn);
    bool shutdown();
    void set_handler(string pattern, handle_func_t fn);
    handle_func_t get_handler(string pattern);
};
}
}
}

#endif	/* COROUTINE_HTTP_SERVER_H */