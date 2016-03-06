#ifndef __ZMSGR_REQUEST_HPP__
#define __ZMSGR_REQUEST_HPP__

#include <zmq.hpp>

#include <memory>
#include <string>

namespace zmsgr {

class ReqSocket
{
///////////////
// Functions //
///////////////
public:
    void Connect(const std::string &socket);
    bool Send(const std::string &data);
    bool Resend(const std::string &data);
    bool Recv(std::string *data, int timeout_ms = -1);

////////////////
// Attributes //
////////////////
private:
    zmq::context_t m_zmqctx = zmq::context_t();
    std::unique_ptr<zmq::socket_t> m_socket = std::make_unique<zmq::socket_t>(m_zmqctx, ZMQ_REQ);
    std::string m_request_sock;
};

} // namespace zmsgr

#endif /* __ZMSGR_REQUEST_HPP__ */
