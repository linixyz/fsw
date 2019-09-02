#include <iostream>
#include "fsw/coroutine.h"

using namespace Fsw;
using namespace std;

int main(int argc, char const *argv[])
{
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

    Coroutine::scheduler();
    
    return 0;
}
