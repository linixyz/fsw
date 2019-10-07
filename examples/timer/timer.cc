#include <iostream>
#include "fsw/timer.h"

using fsw::Timer;
using fsw::timer_manager;

void print_num(void *arg)
{
    std::cout << (uintptr_t)arg << std::endl;
}

int main(int argc, char const *argv[])
{
    fsw_event_init();

    timer_manager.add_timer(1 * Timer::SECOND, print_num, (void*)(uintptr_t)1);
    timer_manager.add_timer(2 * Timer::SECOND, print_num, (void*)(uintptr_t)2);
    timer_manager.add_timer(3 * Timer::SECOND, print_num, (void*)(uintptr_t)3);
    timer_manager.add_timer(4 * Timer::SECOND, print_num, (void*)(uintptr_t)4);

    fsw_event_wait();

    return 0;
}
