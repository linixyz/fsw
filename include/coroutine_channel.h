#ifndef COROUTINE_CHANNEL_H
#define COROUTINE_CHANNEL_H

#include "fsw.h"
#include "coroutine.h"

namespace fsw { namespace coroutine {
class Channel
{
public:
    enum opcode
    {
        PUSH = 1,
        POP = 2,
    };

    Channel();
    ~Channel();
    void* pop(double timeout = -1);
    bool push(void *data, double timeout = -1);

protected:
    size_t capacity = 1;
    bool closed = false;
    std::list<Coroutine*> producer_queue;
    std::list<Coroutine*> consumer_queue;
    std::queue<void*> data_queue;
};
}
}

#endif	/* COROUTINE_CHANNEL_H */