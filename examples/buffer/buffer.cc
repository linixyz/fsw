#include "fsw/buffer.h"

using namespace std;
using fsw::Buffer;

int main(int argc, char const *argv[])
{
    char src_buffer[] = "aa";
    Buffer *buffer1 = new Buffer(1024);
    Buffer *buffer2 = new Buffer(1024);

    buffer1->append(src_buffer, strlen(src_buffer));
    buffer1->append(src_buffer, strlen(src_buffer));
    cout << buffer1->c_buffer() << endl;
    // buffer2->append(buffer1);
    // buffer2->append(buffer1);
    // cout << buffer1->c_buffer() << endl;
    return 0;
}
