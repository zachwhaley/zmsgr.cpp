#ifndef __ZMSGR_HPP__
#define __ZMSGR_HPP__

#include <zmq.hpp>

#include <string>

namespace zmsgr {

inline bool
SendStr(zmq::socket_t& socket, const std::string& data)
{
    zmq::message_t msg {data.size()};
    memcpy(msg.data(), data.data(), data.size());
    return socket.send(msg);
}

inline bool
RecvStr(zmq::socket_t& socket, std::string* data)
{
    zmq::message_t msg;
    bool recieved = socket.recv(&msg);
    if (recieved) {
        data->assign(static_cast<char *>(msg.data()), msg.size());
    }
    return recieved;
}

} // namespace zmsgr

#endif /* __ZMSGR_HPP__ */
