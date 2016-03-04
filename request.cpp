#include "request.hpp"

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
    zmq::message_t msg(data.size());
    memcpy(msg.data(), data.data(), data.size());
    return m_socket.send(msg);
}

bool
ReqSocket::Recv(string *data)
{
    assert(data);

    zmq::message_t msg;
    bool recieved = m_socket.recv(&msg);
    if (recieved) {
        data->assign(static_cast<char *>(msg.data()), msg.size());
    }
    return recieved;
}

} // namespace zmsgr
