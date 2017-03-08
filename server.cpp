#include "server.hpp"

#include "zmsgr.hpp"

#include <array>

using namespace std;

namespace zmsgr {
namespace server {

void
Router::Bind(const string &socket, const string &inproc)
{
    m_worker_sock = inproc;
    m_control_sock = inproc + "_ctrl";

    m_router.bind(socket.c_str());
    m_dealer.bind(m_worker_sock.c_str());
    m_control.connect(m_control_sock.c_str());
    m_control.setsockopt(ZMQ_SUBSCRIBE, "", 0);
}

void
Router::AddWorker(Worker *worker)
{
    // Start a thread for the worker
    m_worker_threads.emplace_back(
    thread([this, worker]() {
        zmq::socket_t socket = zmq::socket_t(m_zmqctx, ZMQ_REP);
        socket.connect(m_worker_sock.c_str());
        zmq::socket_t control = zmq::socket_t(m_zmqctx, ZMQ_SUB);
        control.connect(m_control_sock.c_str());
        control.setsockopt(ZMQ_SUBSCRIBE, "", 0);

        array<zmq::pollitem_t, 2> items = {{
            { static_cast<void*>(control), 0, ZMQ_POLLIN, 0 },
            { static_cast<void*>(socket), 0, ZMQ_POLLIN, 0 }
        }};
        while (true) {
            // Poll socket for message
            zmq::poll(&items[0], items.size(), 0);
            if (items[0].revents & ZMQ_POLLIN) {
                break;
            }
            if (items[1].revents & ZMQ_POLLIN) {
                string request;
                RecvStr(socket, &request);
                SendStr(socket, worker->OnRequest(request));
            }
            this_thread::yield();
        }
    })
    );
}

void
Router::Start()
{
    m_server_thread = thread([this]() {
        zmq::proxy_steerable(static_cast<void*>(m_router), static_cast<void*>(m_dealer), nullptr, static_cast<void*>(m_control));
    });
}

void
Router::Stop()
{
    static const string s_term = "TERMINATE";

    // Stop server
    zmq::socket_t control(m_zmqctx, ZMQ_PUB);
    control.bind(m_control_sock.c_str());
    SendStr(control, s_term);
    m_server_thread.join();

    // Stop workers
    for (auto &t : m_worker_threads) {
        if (t.joinable()) {
            t.join();
        }
    }
}

} // namespace server
} // namespace zmsgr
