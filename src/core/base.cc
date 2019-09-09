#include "fsw.h"
#include "uv.h"
#include "coroutine.h"
#include "log.h"

using fsw::Coroutine;

fswGlobal_t FswG;

int init_fswPoll()
{
    try
    {
        FswG.poll = new fswPoll_t();
    }
    catch(const std::bad_alloc& e)
    {
        fswError("%s", e.what());
    }

    FswG.poll->epollfd = epoll_create(256);
    if (FswG.poll->epollfd  < 0)
    {
        fswWarn("Error has occurred: (errno %d) %s", errno, strerror(errno));
        delete FswG.poll;
        FswG.poll = nullptr;
        return -1;
    }

    FswG.poll->ncap = FSW_EPOLL_CAP;
    try
    {
        FswG.poll->events = new epoll_event[FswG.poll->ncap](); // zero initialized
    }
    catch(const std::bad_alloc& e)
    {
        fswError("%s", e.what());
    }
    
    FswG.poll->event_num = 0;

    return 0;
}

inline int free_fswPoll()
{
    if (close(FswG.poll->epollfd) < 0)
    {
        fswWarn("Error has occurred: (errno %d) %s", errno, strerror(errno));
    }
    delete FswG.poll->events;
    FswG.poll->events = nullptr;
    delete FswG.poll;
    FswG.poll = nullptr;
    return 0;
}

int fsw_event_init()
{
    if (!FswG.poll)
    {
        init_fswPoll();
    }

    FswG.running = 1;

    return 0;
}

typedef enum
{
    UV_CLOCK_PRECISE = 0,  /* Use the highest resolution clock available. */
    UV_CLOCK_FAST = 1      /* Use the fastest clock with <= 1ms granularity. */
} uv_clocktype_t;

extern "C" void uv__run_timers(uv_loop_t* loop);
extern "C" uint64_t uv__hrtime(uv_clocktype_t type);
extern "C" int uv__next_timeout(const uv_loop_t* loop);

int fsw_event_wait()
{
    uv_loop_t *loop = uv_default_loop();

    fsw_event_init();

    while (FswG.running > 0)
    {
        int n;
        int timeout;
        epoll_event *events;

        timeout = uv__next_timeout(loop);
        events = FswG.poll->events;
        n = epoll_wait(FswG.poll->epollfd, events, FswG.poll->ncap, timeout);
        
        for (int i = 0; i < n; i++)
        {
            int fd;
            int id;
            struct epoll_event *p = &events[i];
            uint64_t u64 = p->data.u64;
            Coroutine *co;

            fromuint64(u64, &fd, &id);
            fswTrace("coroutine[%d] resume", id);
            co = Coroutine::get_by_cid(id);
            co->resume();
        }

        loop->time = uv__hrtime(UV_CLOCK_FAST) / 1000000;
        uv__run_timers(loop);

        if (uv__next_timeout(loop) < 0 && FswG.poll->event_num == 0)
        {
            FswG.running = 0;
        }
    }

    fsw_event_free();

    return 0;
}

int fsw_event_free()
{
    FswG.running = 0;
    free_fswPoll();
    return 0;
}