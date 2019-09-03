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

int Socket::bind(int type, char *host, int port)
{
    return fswSocket_bind(sockfd, type, host, port);
}

int Socket::listen()
{
    return fswSocket_listen(sockfd);
}

int Socket::accept()
{
    int connfd;

    connfd = fswSocket_accept(sockfd);
    if (connfd < 0 && errno == EAGAIN)
    {
        wait_event(FSW_EVENT_READ);
        connfd = fswSocket_accept(sockfd);
    }

    return connfd;
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
    long id;
    Coroutine* co;
    epoll_event *ev;

    co = Coroutine::get_current();
    id = co->get_cid();
    ev = FswG.poll.events;

    ev->events = event == FSW_EVENT_READ ? EPOLLIN : EPOLLOUT;
    ev->data.u64 = touint64(sockfd, id);
    epoll_ctl(FswG.poll.epollfd, EPOLL_CTL_ADD, sockfd, ev);

    co->yield();
    return true;
}
