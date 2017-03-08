#include "request.hpp"

#include "zmsgr.hpp"

using namespace std;

namespace zmsgr {

void
ReqSocket::Connect(const string &socket)
{
    m_request_sock = socket;
    m_socket->connect(m_request_sock.c_str());
    //  Configure socket to not wait at close time
    int linger = 0;
    m_socket->setsockopt(ZMQ_LINGER, &linger, sizeof(linger));
}

bool
ReqSocket::Resend(const string &data)
{
    m_socket.reset(new zmq::socket_t(m_zmqctx, ZMQ_REQ));
    Connect(m_request_sock);
    return Send(data);
}

bool
ReqSocket::Send(const string &data)
{
    return SendStr(*m_socket, data);
}

bool
ReqSocket::Recv(string *data, int timeout_ms)
{
    zmq::pollitem_t item = { static_cast<void*>(*m_socket), 0, ZMQ_POLLIN, 0 };
    zmq::poll(&item, 1, timeout_ms);
    if (item.revents & ZMQ_POLLIN) {
        return RecvStr(*m_socket, data);
    }
    return false;
}

} // namespace zmsgr
