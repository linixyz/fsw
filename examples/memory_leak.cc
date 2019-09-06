#include <iostream>
#include "fsw/coroutine.h"

using namespace fsw;
using namespace std;

int main(int argc, char const *argv[])
{
    while (true)
    {
        Coroutine::create([](void *arg)
        {
            Coroutine::create([](void *arg)
            {
                int cid;
                Coroutine *co = Coroutine::get_current();
                cid = co->get_cid();
                cout << cid << endl;
            });
            int cid;
            Coroutine *co = Coroutine::get_current();
            cid = co->get_cid();
            cout << cid << endl;
        });
    }

    return 0;
}
