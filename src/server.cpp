#include "server.hpp"

#include "zmsgr.hpp"

#include <array>

using namespace std;

namespace zmsgr::server {

void
Router::Bind(const string& socket, const string& inproc)
{
    _worker_sock = inproc;
    _control_sock = inproc + "_ctrl";

    _router.bind(socket.c_str());
    _dealer.bind(_worker_sock.c_str());
    _control.connect(_control_sock.c_str());
    _control.setsockopt(ZMQ_SUBSCRIBE, "", 0);
}

void
Router::AddWorker(std::function<std::string(const std::string&)>&& worker)
{
    // Start a thread for the worker
    _worker_threads.emplace_back(
    thread([this, worker]() {
        zmq::socket_t socket {zmq::socket_t(_zmqctx, ZMQ_REP)};
        socket.connect(_worker_sock.c_str());
        zmq::socket_t control {zmq::socket_t(_zmqctx, ZMQ_SUB)};
        control.connect(_control_sock.c_str());
        control.setsockopt(ZMQ_SUBSCRIBE, "", 0);

        array<zmq::pollitem_t, 2> items = {{
            { static_cast<void*>(control), 0, ZMQ_POLLIN, 0 },
            { static_cast<void*>(socket), 0, ZMQ_POLLIN, 0 }
        }};
        while (true) {
            // Poll socke<zmq::socket_t>t for message
            zmq::poll(&items[0], items.size(), 0);
            if (items[0].revents & ZMQ_POLLIN) {
                break;
            }
            if (items[1].revents & ZMQ_POLLIN) {
                string request;
                RecvStr(socket, &request);
                SendStr(socket, worker(request));
            }
            this_thread::yield();
        }
    })
    );
}

void
Router::Start()
{
    _server_thread = thread([this]() {
        zmq::proxy_steerable(static_cast<void*>(_router), static_cast<void*>(_dealer), nullptr, static_cast<void*>(_control));
    });
}

void
Router::Stop()
{
    static const string s_term {"TERMINATE"};

    // Stop server
    zmq::socket_t control(_zmqctx, ZMQ_PUB);
    control.bind(_control_sock.c_str());
    SendStr(control, s_term);
    _server_thread.join();

    // Stop workers
    for (auto &t : _worker_threads) {
        if (t.joinable()) {
            t.join();
        }
    }
}

} // namespace zmsgr::server
