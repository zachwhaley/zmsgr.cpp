#include "reply.hpp"

#include "zmsgr.hpp"

using namespace std;

namespace zmsgr {

void
RepSocket::Bind(const string& socket)
{
    _socket.bind(socket.c_str());
}

bool
RepSocket::Send(const string& data)
{
    return SendStr(_socket, data);
}

bool
RepSocket::Recv(string* data)
{
    assert(data);
    return RecvStr(_socket, data);
}

} // namespace zmsgr
