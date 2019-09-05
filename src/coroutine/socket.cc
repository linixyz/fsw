#include "coroutine_socket.h"
#include "coroutine.h"
#include "socket.h"

using fsw::Coroutine;
using fsw::coroutine::Socket;

Socket::Socket(int domain, int type, int protocol)
{
    sockfd = fswSocket_create(domain, type, protocol);
    if (sockfd < 0)
    {
        return;
    }
    fswSocket_set_nonblock(sockfd);
}

Socket::Socket(int fd)
{
    sockfd = fd;
    fswSocket_set_nonblock(sockfd);
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

    do
    {
        connfd = fswSocket_accept(sockfd);
    } while (connfd < 0 && errno == EAGAIN && wait_event(FSW_EVENT_READ));

    return connfd;
}

ssize_t Socket::recv(void *buf, size_t len)
{
    int ret;

    do
    {
        ret = fswSocket_recv(sockfd, buf, len, 0);
    } while (ret < 0 && errno == EAGAIN && wait_event(FSW_EVENT_READ));
    
    return ret;
}

ssize_t Socket::send(const void *buf, size_t len)
{
    int ret;

    do
    {
        ret = fswSocket_send(sockfd, buf, len, 0);
    } while (ret < 0 && errno == EAGAIN && wait_event(FSW_EVENT_WRITE));
    
    return ret;
}

int Socket::close()
{
    return fswSocket_close(sockfd);
}

bool Socket::wait_event(int event)
{
    long id;
    Coroutine* co;
    epoll_event *ev;

    co = Coroutine::get_current();
    id = co->get_cid();

    if (!FswG.poll)
    {
        init_fswPoll();
    }
    ev = FswG.poll->events;

    ev->events = event == FSW_EVENT_READ ? EPOLLIN : EPOLLOUT;
    ev->data.u64 = touint64(sockfd, id);
    epoll_ctl(FswG.poll->epollfd, EPOLL_CTL_ADD, sockfd, ev);

    co->yield();
    return true;
}
