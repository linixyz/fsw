#include "coroutine.h"
#include "log.h"
#include "timer.h"

using fsw::Coroutine;
using fsw::timer_manager;
using fsw::Timer;

Coroutine* Coroutine::current = nullptr;
long Coroutine::last_cid = 0;
std::unordered_map<long, Coroutine*> Coroutine::coroutines;
size_t Coroutine::stack_size = DEFAULT_C_STACK_SIZE;

void Coroutine::execute_defer_tasks()
{
    if (defer_tasks) {
        defer_task *defer_task;
        while(!defer_tasks->empty())
        {
            defer_task = defer_tasks->top();
            defer_tasks->pop();
            defer_task->fn(defer_task->args);
        }
        delete defer_tasks;
        defer_tasks = nullptr;
    }
}

long Coroutine::run()
{
    long cid = this->cid;
    origin = current;
    current = this;
    ctx.swap_in();
    if (ctx.is_end())
    {
        execute_defer_tasks();
        fswTrace("coroutine[%ld] end", cid);
        current = origin;
        coroutines.erase(cid);
        delete this;
    }
    return cid;
}

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
        execute_defer_tasks();
        fswTrace("coroutine[%ld] end", cid);
        current = origin;
        coroutines.erase(cid);
        delete this;
    }
}

void Coroutine::defer(coroutine_func_t _fn, void* _args)
{
    if (defer_tasks == nullptr)
    {
        defer_tasks = new std::stack<defer_task *>;
    }
    defer_task *task = new defer_task();
    task->fn = _fn;
    task->args = _args;
    defer_tasks->push(task);
}

static void sleep_timeout(void *param)
{
    ((Coroutine *) param)->resume();
}

int Coroutine::sleep(double seconds)
{
    Coroutine *co = Coroutine::get_current();
    fswTrace("coroutine[%ld] sleep", co->cid);

    timer_manager.add_timer(seconds * Timer::SECOND, sleep_timeout, (void*)co);
    co->yield();
    return 0;
}
