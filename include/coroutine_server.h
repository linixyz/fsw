#ifndef COROUTINE_SERVER_H
#define COROUTINE_SERVER_H

#include "fsw.h"
#include "coroutine_socket.h"

typedef void (*handle_func_t)(void*);

namespace fsw { namespace coroutine {
class Server
{
private:
    Socket *socket;
    handle_func_t handler;
    
public:
    Server(char *host, int port);
    ~Server();
    bool start();
    bool shutdown();
    void handle(handle_func_t fn);
};
}
}

#endif	/* COROUTINE_SERVER_H */