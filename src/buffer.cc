#include "buffer.h"
#include "log.h"

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

    if (_length + length > _size)
    {
        fswError("buffer capacity is not enough");
    }

    while (cursor < end)
    {
        _buffer[_length++] = src_buffer[cursor++];
    }
    _buffer[_length] = 0;
}

void Buffer::append(std::string str)
{
    off_t end = str.length();
    off_t cursor = 0;
    const char *src_buffer = str.c_str();

    if (_length + str.length() > _size)
    {
        fswError("buffer capacity is not enough");
    }

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

    if (_length + buffer->length() > _size)
    {
        fswError("buffer capacity is not enough");
    }

    while (cursor < end)
    {
        _buffer[_length++] = src_buffer[cursor++];
    }
    _buffer[_length] = 0;
}

void Buffer::clear()
{
    for (size_t i = 0; i < _length; i++)
    {
        _buffer[_length] = 0;
    }
    _length = 0;
}