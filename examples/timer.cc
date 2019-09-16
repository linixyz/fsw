#include <iostream>
#include "fsw/timer.h"

using fsw::TimerManager;
using fsw::Timer;

void print_num(void *arg)
{
    std::cout << (uintptr_t)arg << std::endl;
}

int main(int argc, char const *argv[])
{
    TimerManager timer_manager;

    timer_manager.add_timer(1 * Timer::SECOND, print_num, (void*)(uintptr_t)1);
    timer_manager.add_timer(2 * Timer::SECOND, print_num, (void*)(uintptr_t)2);
    timer_manager.add_timer(3 * Timer::SECOND, print_num, (void*)(uintptr_t)3);
    timer_manager.add_timer(4 * Timer::SECOND, print_num, (void*)(uintptr_t)4);

    while (timer_manager.get_next_timeout() > 0)
    {
        timer_manager.run_timers();
    }

    return 0;
}
