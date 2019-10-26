#include <iostream>
#include "fsw/coroutine.h"

using namespace fsw;
using namespace std;

int main(int argc, char const *argv[])
{
    fsw_event_init();
    Coroutine::create([](void *arg)
    {
        Coroutine *co = Coroutine::get_current();
        co->defer([](void *arg)
        {
            cout << "end 1 1" << endl;
        });
        co->defer([](void *arg)
        {
            cout << "end 1 2" << endl;
        });
        Coroutine::sleep(1);
        cout << "co1" << endl;
    });

    Coroutine::create([](void *arg)
    {
        Coroutine *co = Coroutine::get_current();
        co->defer([](void *arg)
        {
            cout << "end 2 1" << endl;
        });
        co->defer([](void *arg)
        {
            cout << "end 2 2" << endl;
        });
        cout << "co2" << endl;
    });
    fsw_event_wait();

    return 0;
}
