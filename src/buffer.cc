#include "buffer.h"

using fsw::Buffer;

Buffer::Buffer(size_t size):
    _size(size)
{
    _length = 0;
    _buffer = new char[_size + 1]();
}

Buffer::~Buffer()
{
    delete[] _buffer;
}

size_t Buffer::length()
{
    return _length;
}

/**
 * size represents a valid character that can be stored, excluding the last \0
 */
size_t Buffer::size()
{
    return _size;
}

char* Buffer::c_buffer()
{
    return _buffer;
}

void Buffer::append(char *str, size_t length)
{
    off_t end = length;
    off_t cursor = 0;
    char *src_buffer = str;

    while (cursor < end)
    {
        _buffer[_length++] = src_buffer[cursor++];
    }
    _buffer[_length] = 0;
}

void Buffer::append(Buffer *buffer)
{
    off_t end = buffer->length();
    off_t cursor = 0;
    char *src_buffer = buffer->c_buffer();

    while (cursor < end)
    {
        _buffer[_length++] = src_buffer[cursor++];
    }
    _buffer[_length] = 0;
}