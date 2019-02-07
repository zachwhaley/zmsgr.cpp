#ifndef __SERVER_HPP__
#define __SERVER_HPP__

#include <zmq.hpp>

#include <functional>
#include <memory>
#include <string>
#include <thread>
#include <vector>

namespace zmsgr::server {

class Router
{
///////////////
// Functions //
///////////////
public:
    // inproc must be a 'inproc://' socket
    void Bind(const std::string& socket, const std::string& inproc);
    void AddWorker(std::function<std::string(const std::string&)>&& worker);
    void Start();
    void Stop();

////////////////
// Attributes //
////////////////
private:
    zmq::context_t _zmqctx {zmq::context_t()};
    zmq::socket_t _router {zmq::socket_t(_zmqctx, ZMQ_ROUTER)};
    zmq::socket_t _dealer {zmq::socket_t(_zmqctx, ZMQ_DEALER)};
    zmq::socket_t _control {zmq::socket_t(_zmqctx, ZMQ_SUB)};
    std::string _worker_sock;
    std::string _control_sock;
    std::thread _server_thread;
    std::vector<std::thread> _worker_threads;
};

} // namespace zmsgr::server

#endif /* __SERVER_HPP__ */
