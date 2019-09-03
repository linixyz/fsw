#include <iostream>
#include "fsw/coroutine.h"
#include "fsw/socket.h"
#include "fsw/log.h"

using namespace fsw;
using namespace std;

char response_str[] = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nConnection: keep-alive\r\nContent-Length: 11\r\n\r\nhello world\r\n";

int main(int argc, char const *argv[])
{
    int sock;
    int connfd;
    char ip[] = "127.0.0.1";

    sock = fswSocket_create(FSW_SOCK_TCP);
    fswSocket_bind(sock, FSW_SOCK_TCP, ip, 80);
    fswSocket_listen(sock);

    while (true)
    {
        connfd = fswSocket_accept(sock);
        if (connfd < 0)
        {
            fswWarn("Error has occurred: (errno %d) %s", errno, strerror(errno));
        }

        Coroutine::create([](void *_sock)
        {
            int connfd = (intptr_t)_sock;
            while (true)
            {
                int ret;
                char buf[1024] = {0};

                fswDebug("connfd[%d] recv.", connfd);
                ret = fswSocket_recv(connfd, buf, sizeof(buf) - 1, 0);
                
                if (ret == 0)
                {
                    fswDebug("client[%d] closed.", connfd);
                    close(connfd);
                    break;
                }
                else if (ret < 0)
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
            }

        }, (void *)(intptr_t)connfd);
    }

    return 0;
}