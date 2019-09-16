#ifndef TIMER_H
#define TIMER_H

#include "fsw.h"

typedef void (*timer_func_t)(void*);

namespace fsw
{
class TimerManager;

class Timer
{
friend class TimerManager;
friend class Compare_Timer_Pointer;
public:
    static const uint64_t MILLI_SECOND;
    static const uint64_t SECOND;
    Timer(uint64_t _timeout, timer_func_t _callback, void *_private_data, TimerManager *_timer_manager);
    static uint64_t get_current_ms();
    friend bool operator <(const Timer& t1, const Timer& t2) 
    {
        return t1.exec_msec > t2.exec_msec; 
    }
    
private:
    uint64_t timeout = 0;
    uint64_t exec_msec = 0;
    timer_func_t callback;
    void *private_data;
    TimerManager *timer_manager = nullptr;
};

class Compare_Timer_Pointer
{
public:
    bool operator () (Timer* &timer1, Timer* &timer2) const
    {
        return timer1->exec_msec > timer2->exec_msec;
    }
};

class TimerManager
{
public:
    TimerManager();
    ~TimerManager();
    void add_timer(int64_t _timeout, timer_func_t _callback, void *_private_data);
    void run_timers();
private:
    std::priority_queue<Timer*, std::vector<Timer*>, Compare_Timer_Pointer> timers;
};
}

#endif	/* TIMER_H */
