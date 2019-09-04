#include "coroutine.h"

using fsw::Coroutine;

Coroutine* Coroutine::current = nullptr;
long Coroutine::last_cid = 0;
std::unordered_map<long, Coroutine*> Coroutine::coroutines;
size_t Coroutine::stack_size = DEFAULT_C_STACK_SIZE;

void* Coroutine::get_current_task()
{
    return current ? current->get_task() : nullptr;
}

void* Coroutine::get_task()
{
    return task;
}

Coroutine* Coroutine::get_current()
{
    return current;
}

void Coroutine::set_task(void *_task)
{
    task = _task;
}

long Coroutine::create(coroutine_func_t fn, void* args)
{
    return (new Coroutine(fn, args))->run();
}

void Coroutine::yield()
{
    current = origin;
    ctx.swap_out();
}

void Coroutine::resume()
{
    origin = current;
    current = this;
    ctx.swap_in();
    if (ctx.is_end())
    {
        current = origin;
        coroutines.erase(cid);
        delete this;
    }
}

static void sleep_timeout(uv_timer_t *timer)
{
    ((Coroutine *) timer->data)->resume();
}

int Coroutine::sleep(double seconds)
{
    Coroutine *co = Coroutine::get_current();

    uv_timer_t timer;
    timer.data = co;
    uv_timer_init(uv_default_loop(), &timer);
    uv_timer_start(&timer, sleep_timeout, seconds * 1000, 0);
   
    co->yield();
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

int Coroutine::scheduler()
{
    uv_loop_t *loop = uv_default_loop();

    if (!FswG.poll)
    {
        init_fswPoll();
    }

    while (loop->stop_flag == 0)
    {
        int n;
        int timeout;
        epoll_event *events;

        timeout = uv__next_timeout(loop);
        events = FswG.poll->events;
        n = epoll_wait(FswG.poll->epollfd, events, FswG.poll->ncap, timeout);
        
        for (int i = 0; i < n; i++) {
            int fd;
            int id;
            struct epoll_event *p = &events[i];
            uint64_t u64 = p->data.u64;
            Coroutine *co;

            fromuint64(u64, &fd, &id);
            co = get_by_cid(id);
            co->resume();
        }

        loop->time = uv__hrtime(UV_CLOCK_FAST) / 1000000;
        uv__run_timers(loop);

        if (uv__next_timeout(loop) < 0 && !FswG.poll)
        {
            uv_stop(loop);
        }
    }

    free_fswPoll();

    return 0;
}