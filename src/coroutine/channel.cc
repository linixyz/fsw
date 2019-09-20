#include "coroutine_channel.h"

using fsw::coroutine::Channel;

Channel::Channel(size_t _capacity):
    capacity(_capacity)
{
}

Channel::~Channel()
{
}