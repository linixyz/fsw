#ifndef COROUTINE_SOCKET_H
#define COROUTINE_SOCKET_H

namespace Fsw { namespace Coroutine {
class Socket
{
private:
    int sockfd;
public:
    Socket(int type);
    ~Socket();
};
}
}



#endif	/* COROUTINE_SOCKET_H */