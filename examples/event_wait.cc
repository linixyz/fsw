#include <iostream>
#include "fsw/coroutine.h"
#include "fsw/fsw.h"

using namespace fsw;
using namespace std;

int main(int argc, char const *argv[])
{
    fsw_event_init();
    fsw_event_wait();
    
    return 0;
}
