#ifndef __SERVER_HPP__
#define __SERVER_HPP__

#include <zmq.hpp>

#include <list>
#include <string>
#include <thread>

namespace zmsgr {
namespace server {

class Worker
{
///////////////
// Functions //
///////////////
public:
    virtual ~Worker() {}
    virtual std::string OnRequest(const std::string &data) = 0;
};

class Router
{
///////////////
// Functions //
///////////////
public:
    // inproc must be a 'inproc://' socket
    void Bind(const std::string &socket, const std::string &inproc);
    void AddWorker(Worker *worker);
    void Start();
    void Stop();

////////////////
// Attributes //
////////////////
private:
    zmq::context_t m_zmqctx = zmq::context_t();
    zmq::socket_t m_router = zmq::socket_t(m_zmqctx, ZMQ_ROUTER);
    zmq::socket_t m_dealer = zmq::socket_t(m_zmqctx, ZMQ_DEALER);
    zmq::socket_t m_control = zmq::socket_t(m_zmqctx, ZMQ_SUB);
    std::string m_worker_sock;
    std::string m_control_sock;
    std::thread m_server_thread;
    std::list<std::thread> m_worker_threads;
};

} // namespace server
} // namespace zmsgr

#endif /* __SERVER_HPP__ */
