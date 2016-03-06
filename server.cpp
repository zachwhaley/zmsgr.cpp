#include "server.hpp"

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

        zmq::pollitem_t items[] = {
            { control, 0, ZMQ_POLLIN, 0 },
            { socket, 0, ZMQ_POLLIN, 0 }
        };
        while (true) {
            // Poll socket for message
            zmq::poll(&items[0], 2, 0);
            if (items[0].revents & ZMQ_POLLIN) {
                break;
            }
            if (items[1].revents & ZMQ_POLLIN) {
                zmq::message_t req;
                socket.recv(&req);
                string data(static_cast<char *>(req.data()), req.size());

                string resp = worker->OnRequest(data);

                zmq::message_t rep(resp.size());
                memcpy(rep.data(), resp.data(), resp.size());
                socket.send(rep);
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
        zmq::proxy_steerable(m_router, m_dealer, nullptr, m_control);
    });
}

void
Router::Stop()
{
    static const string s_term = "TERMINATE";

    // Stop server
    zmq::socket_t control(m_zmqctx, ZMQ_PUB);
    control.bind(m_control_sock.c_str());
    zmq::message_t term(s_term.size());
    memcpy(term.data(), s_term.data(), s_term.size());
    control.send(term);
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
