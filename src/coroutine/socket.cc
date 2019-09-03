#include "coroutine_socket.h"
#include "coroutine.h"
#include "socket.h"

using fsw::Coroutine;
using fsw::coroutine::Socket;

Socket::Socket(int type)
{
    sockfd = fswSocket_create(type);
    if (sockfd < 0)
    {
        return;
    }
}

Socket::~Socket()
{
}

int Socket::accept()
{
    return 0;
}

ssize_t Socket::recv(void *buf, size_t len)
{
    return 0;
}

ssize_t Socket::send(const void *buf, size_t len)
{
    return 0;
}

bool Socket::wait_event(int event)
{
    return 0;
}
