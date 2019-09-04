#include <iostream>
#include "fsw/coroutine.h"
#include "fsw/socket.h"
#include "fsw/coroutine_socket.h"
#include "fsw/log.h"

using namespace fsw;
using namespace std;
using fsw::coroutine::Socket;

char response_str[] = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nConnection: close\r\nContent-Length: 11\r\n\r\nhello world\r\n";

struct co_param
{
    int port;
};

int main(int argc, char const *argv[])
{
    co_param param = {80};

    Coroutine::create([](void *param)
    {
        co_param *_param = (co_param *)param;
        int port = _param->port;
        char ip[] = "127.0.0.1";

        Socket sock(FSW_SOCK_TCP);
        sock.bind(FSW_SOCK_TCP, ip, port);
        sock.listen();
    
        while (true)
        {
            int connfd = sock.accept();

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
    }, &param);

    Coroutine::scheduler();

    return 0;
}