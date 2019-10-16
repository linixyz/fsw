#include "fsw/buffer.h"
#include "fsw/fsw.h"

using namespace std;
using fsw::Buffer;

int main(int argc, char const *argv[])
{
    char src_buffer[] = "aa";
    Buffer *buffer1 = new Buffer(1024);
    Buffer *buffer2 = new Buffer(1024);

    buffer1->append(src_buffer, strlen(src_buffer));
    buffer1->append(src_buffer, strlen(src_buffer));
    assert(strcmp(buffer1->c_buffer(), "aaaa") == 0);
    assert(buffer1->length() == 4);
    buffer2->append(buffer1);
    buffer2->append(buffer1);
    assert(strcmp(buffer2->c_buffer(), "aaaaaaaa") == 0);
    assert(buffer2->length() == 8);

    Buffer *buffer3 = new Buffer(1);
    buffer3->append(src_buffer, strlen(src_buffer));
    return 0;
}
