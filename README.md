# Fsw

fsw means from swoole but not all swoole. This project is the foundation of [study php extension](https://github.com/php-extension-research/study).

# Example

```cpp
#include <iostream>
#include "fsw/coroutine.h"

using namespace Fsw;
using namespace std;

int main(int argc, char const *argv[])
{
    Coroutine::create([](void *arg)
    {
        cout << "co1" << endl;
    });

    Coroutine::create([](void *arg)
    {
        cout << "co2" << endl;
    });
    
    return 0;
}
```

