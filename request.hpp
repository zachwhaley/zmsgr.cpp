#ifndef __ZMSGR_REQUEST_HPP__
#define __ZMSGR_REQUEST_HPP__

#include <zmq.hpp>

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
    bool Recv(std::string *data);

////////////////
// Attributes //
////////////////
private:
    zmq::context_t m_context = zmq::context_t();
    zmq::socket_t m_socket = zmq::socket_t(m_context, ZMQ_REQ);
};

} // namespace zmsgr

#endif /* __ZMSGR_REQUEST_HPP__ */
