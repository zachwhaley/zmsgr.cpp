#include "reply.hpp"

#include "zmsgr.hpp"

using namespace std;

namespace zmsgr {

void
RepSocket::Bind(const string &socket)
{
    m_socket.bind(socket.c_str());
}

bool
RepSocket::Send(const string &data)
{
    return SendStr(m_socket, data);
}

bool
RepSocket::Recv(string *data)
{
    assert(data);

    return RecvStr(m_socket, data);
}

} // namespace zmsgr
