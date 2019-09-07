/**
 * non-coroutine: 
 * ab -c 100 -n 10000 http://127.0.0.1/
 * Requests per second:    11946.56 [#/sec] (mean)
 * Requests per second:    11310.96 [#/sec] (mean)
 * Requests per second:    9484.07 [#/sec] (mean)
 */

#include <iostream>
#include "fsw/coroutine.h"
#include "fsw/socket.h"
#include "fsw/log.h"

using namespace fsw;
using namespace std;

char response_str[] = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nConnection: close\r\nContent-Length: 11\r\n\r\nhello world\r\n";

int main(int argc, char const *argv[])
{
    int sock;
    int connfd;
    char ip[] = "127.0.0.1";

    sock = fswSocket_create(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        fswWarn("fswSocket_create error");
        return -1;
    }
    if (fswSocket_bind(sock, FSW_SOCK_TCP, ip, 80) < 0)
    {
        fswWarn("fswSocket_bind error");
        return -1;
    }
    if (fswSocket_listen(sock) < 0)
    {
        fswWarn("fswSocket_listen error");
        return -1;
    }

    while (true)
    {
        connfd = fswSocket_accept(sock);
        if (connfd < 0)
        {
            fswWarn("fswSocket_accept error");
            return -1;
        }

        Coroutine::create([](void *_sock)
        {
            int connfd = (intptr_t)_sock;
            int ret;
            char buf[1024] = {0};

            fswDebug("connfd[%d] recv.", connfd);
            ret = fswSocket_recv(connfd, buf, sizeof(buf) - 1, 0);
            
            if (ret < 0)
            {
                fswWarn("Error has occurred: (errno %d) %s", errno, strerror(errno));
            }
            fswDebug("connfd[%d] recv success.", connfd);
            
            buf[ret] = 0;
            fswDebug("connfd[%d] send.", connfd);
            ret = fswSocket_send(connfd, response_str, sizeof(response_str) - 1, 0);
            if (ret < 0)
            {
                fswWarn("Error has occurred: (errno %d) %s", errno, strerror(errno));
            }
            fswDebug("connfd[%d] send success.", connfd);
            close(connfd);

        }, (void *)(intptr_t)connfd);
    }

    return 0;
}