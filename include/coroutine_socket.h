#ifndef COROUTINE_SOCKET_H
#define COROUTINE_SOCKET_H

#include "fsw.h"

namespace Fsw { namespace Coroutine {
class Socket
{
private:
    int sockfd;
public:
    Socket(int type);
    ~Socket();
    int accept();
    ssize_t recv(void *buf, size_t len);
    ssize_t send(const void *buf, size_t len);

    bool wait_event(int event);
};
}
}



#endif	/* COROUTINE_SOCKET_H */