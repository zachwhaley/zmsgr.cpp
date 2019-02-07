#ifndef __ZMSGR_REPLY_HPP__
#define __ZMSGR_REPLY_HPP__

#include <zmq.hpp>

#include <string>

namespace zmsgr {

class RepSocket
{
///////////////
// Functions //
///////////////
public:
    void Bind(const std::string& socket);
    bool Send(const std::string& data);
    bool Recv(std::string* data);

////////////////
// Attributes //
////////////////
private:
    zmq::context_t _zmqctx {zmq::context_t()};
    zmq::socket_t _socket {zmq::socket_t(_zmqctx, ZMQ_REP)};

};

} // namespace zmsgr

#endif /* __ZMSGR_REPLY_HPP__ */
