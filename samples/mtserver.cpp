#include <server.hpp>
#include <request.hpp>

#include <iostream>
#include <thread>
#include <vector>

using namespace std;

static const string SOCKET = "ipc://mtserver.ipc";
static const string INPROC = "inproc://mtserver";

class MTWorker : public zmsgr::server::Worker
{
public:
    MTWorker() {}
    virtual ~MTWorker() {}
    virtual string OnRequest(const string &data)
    {
        return string(data + " Done!");
    }
};

void server()
{
    zmsgr::server::Router router;
    router.Bind(SOCKET, INPROC);
    for (int i = 0; i < 3; i++) {
        router.AddWorker(new MTWorker());
    }
    router.Start();
    this_thread::sleep_for(1s);
    router.Stop();
}

void client(string id)
{
    zmsgr::ReqSocket req;
    req.Connect(SOCKET);

    if (req.Send(id))
        cout << "Client " << id << ": Sent " << id << endl;
    else
        cout << "Client " << id << ": Send Failed!" << endl;

    string reply;
    if (req.Recv(&reply))
        cout << "Client " << id << ": Recieved " << reply << endl;
    else
        cout << "Client " << id << ": Recv Failed!" << endl;
}

int main(int argc, const char *argv[])
{
    thread s(server);
    this_thread::sleep_for(1ms);

    vector<thread> tg;
    for (auto id : {"A", "B", "C", "D"}) {
        tg.emplace_back(thread(client, id));
    }

    for (auto &c : tg) {
        if (c.joinable())
            c.join();
    }
    s.join();
    return 0;
}
