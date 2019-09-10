#include <iostream>
#include "fsw/coroutine.h"
#include "fsw/fsw.h"

using namespace fsw;
using namespace std;

int main(int argc, char const *argv[])
{
    fsw_event_init();

    while (true)
    {
        Coroutine::create([](void *arg)
        {
            Coroutine *co = Coroutine::get_current();
            int  cid = co->get_cid();
            cout << cid << endl;
            Coroutine::sleep(0.5);
            cout << cid << endl;
        });

        fsw_event_wait();
    }
    
    return 0;
}
