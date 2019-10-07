#include <iostream>
#include "fsw/coroutine.h"
#include "fsw/coroutine_channel.h"

using namespace fsw;
using namespace std;
using fsw::coroutine::Channel;

int main(int argc, char const *argv[])
{
    Channel *chan = new Channel();

    Coroutine::create([](void *arg)
    {
        void *data;
        Channel *chan = (Channel *)arg;

        std::cout << "pop start" << std::endl;
        data = chan->pop();
        std::cout << "pop ret:" << *(string *)data << std::endl;
    }, (void *)chan);

    Coroutine::create([](void *arg)
    {
        bool ret;
        string data = "hello world";
        Channel *chan = (Channel *)arg;

        std::cout << "push start" << std::endl;
        ret = chan->push(&data);
        std::cout << "push ret:" << ret << std::endl;
    }, (void *)chan);

    std::cout << "main co" << endl;
    delete chan;

    return 0;
}
