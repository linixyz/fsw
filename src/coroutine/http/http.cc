#include "coroutine_http.h"

using fsw::coroutine::http::Request;
using fsw::coroutine::http::Ctx;
using fsw::coroutine::Socket;


Request::Request()
{
    
}

Ctx::Ctx(Socket *conn)
{
    private_data = conn;
}

Ctx::~Ctx()
{
    delete private_data;
}

