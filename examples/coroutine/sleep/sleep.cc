#include <iostream>
#include "fsw/coroutine.h"
#include "fsw/fsw.h"

using namespace fsw;
using namespace std;

int main(int argc, char const *argv[])
{
    fsw_event_init();

    Coroutine::create([](void *arg)
    {
        cout << "co1" << endl;
        Coroutine::sleep(0.3);
        cout << "co1" << endl;
    });

    Coroutine::create([](void *arg)
    {
        cout << "co2" << endl;
        Coroutine::sleep(1);
        cout << "co2" << endl;
    });

    fsw_event_wait();
    
    return 0;
}
