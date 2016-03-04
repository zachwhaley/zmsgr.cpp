#include "reply.hpp"

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
    zmq::message_t msg(data.size());
    memcpy(msg.data(), data.data(), data.size());
    return m_socket.send(msg);
}

bool
RepSocket::Recv(string *data)
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
