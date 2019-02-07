#include <reply.hpp>
#include <request.hpp>

#include <iostream>
#include <thread>

using namespace std;

static const string SOCKET = "ipc://rrsample.ipc";

void server()
{
    zmsgr::RepSocket rep;
    rep.Bind(SOCKET);

    string request;
    if (rep.Recv(&request))
        cout << "Server: Recieved " << request << endl;
    else
        cout << "Server: Recv Failed!" << endl;

    string goodbye {"Goodbye!"};
    if (rep.Send(goodbye))
        cout << "Server: Sent " << goodbye << endl;
    else
        cout << "Server: Send Failed!" << endl;
}

void client()
{
    zmsgr::ReqSocket req;
    req.Connect(SOCKET);

    string hello {"Hello!"};
    if (req.Send(hello))
        cout << "Client: Sent " << hello << endl;
    else
        cout << "Client: Send Failed!" << endl;

    string reply;
    if (req.Recv(&reply))
        cout << "Client: Recieved " << reply << endl;
    else
        cout << "Client: Recv Failed!" << endl;
}

int main(int argc, const char *argv[])
{
    thread s {server};
    this_thread::sleep_for(1ms);
    thread c {client};

    s.join();
    c.join();
    return 0;
}
