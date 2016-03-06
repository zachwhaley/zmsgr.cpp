#include "request.hpp"

#include "zmsgr.hpp"

using namespace std;

namespace zmsgr {

void
ReqSocket::Connect(const string &socket)
{
    m_socket.connect(socket.c_str());
}

bool
ReqSocket::Send(const string &data)
{
    return SendStr(m_socket, data);
}

bool
ReqSocket::Recv(string *data)
{
    return RecvStr(m_socket, data);
}

} // namespace zmsgr
