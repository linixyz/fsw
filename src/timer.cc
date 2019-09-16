#include "timer.h"

using fsw::Timer;
using fsw::TimerManager;

const uint64_t Timer::MILLI_SECOND = 1;
const uint64_t Timer::SECOND = 1000;

uint64_t Timer::get_current_ms()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

Timer::Timer(uint64_t _timeout, timer_func_t _callback, void *_private_data, TimerManager *_timer_manager):
    timeout(_timeout), callback(_callback), private_data(_private_data), timer_manager(_timer_manager)
{
    exec_msec = get_current_ms() + _timeout;
}

TimerManager::TimerManager()
{
}

TimerManager::~TimerManager()
{
}

void TimerManager::add_timer(int64_t _timeout, timer_func_t _callback, void *_private_data)
{
    Timer *timer = new Timer(_timeout, _callback, _private_data, this);
    timers.push(timer);
}

void TimerManager::run_timers()
{
    uint64_t now = Timer::get_current_ms();
    if (!timers.empty())
    {
        while (true)
        {
            Timer *t = timers.top();
            if (now < t->exec_msec)
            {
                break;
            }
            timers.pop();
            t->callback(t->private_data);
            delete t;
        }
    }
}