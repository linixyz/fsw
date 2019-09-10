#include "coroutine.h"
#include "log.h"

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
    assert(current == this);
    fswTrace("coroutine[%ld] yield", cid);
    current = origin;
    ctx.swap_out();
}

void Coroutine::resume()
{
    assert(current != this);
    fswTrace("coroutine[%ld] resume", cid);
    origin = current;
    current = this;
    ctx.swap_in();
    if (ctx.is_end())
    {
        fswTrace("coroutine[%ld] end", cid);
        current = origin;
        coroutines.erase(cid);
        delete this;
    }
}

static void sleep_timeout(uv_timer_t *timer)
{
    fswTrace("coroutine[%ld] sleep timeout", ((Coroutine *) timer->data)->get_cid());
    ((Coroutine *) timer->data)->resume();
}

static void fsw_timer_close(uv_handle_t *handle)
{
	delete handle;
    handle = nullptr;
}

int Coroutine::sleep(double seconds)
{
    uv_timer_t *timer;
    Coroutine *co = Coroutine::get_current();
    fswTrace("coroutine[%ld] sleep", co->cid);

    try
    {
        timer = new uv_timer_t();
        fswTrace("coroutine[%ld] new timer[%p]", co->get_cid(), timer);
    }
    catch(const std::bad_alloc& e)
    {
        fswError("%s", e.what());
    }
    
    timer->data = co;
    uv_timer_init(uv_default_loop(), timer);
    uv_timer_start(timer, sleep_timeout, seconds * 1000, 0);
    uv_run(uv_default_loop(), UV_RUN_DEFAULT);
   
    co->yield();
    fswTrace("coroutine[%ld] free timer[%p]", co->get_cid(), timer);
    uv_timer_stop(timer);
    uv_close((uv_handle_t *)timer, fsw_timer_close);
    return 0;
}
