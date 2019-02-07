#include "request.hpp"

#include "zmsgr.hpp"

using namespace std;

namespace zmsgr {

void
ReqSocket::Connect(const string& socket)
{
    _request_sock = socket;
    _socket->connect(_request_sock.c_str());
    //  Configure socket to not wait at close time
    int linger = 0;
    _socket->setsockopt(ZMQ_LINGER, &linger, sizeof(linger));
}

bool
ReqSocket::Resend(const string& data)
{
    _socket.reset(new zmq::socket_t(_zmqctx, ZMQ_REQ));
    Connect(_request_sock);
    return Send(data);
}

bool
ReqSocket::Send(const string& data)
{
    return SendStr(*_socket, data);
}

bool
ReqSocket::Recv(string* data, int timeout_ms)
{
    zmq::pollitem_t item {static_cast<void*>(*_socket), 0, ZMQ_POLLIN, 0};
    zmq::poll(&item, 1, timeout_ms);
    if (item.revents & ZMQ_POLLIN) {
        return RecvStr(*_socket, data);
    }
    return false;
}

} // namespace zmsgr
