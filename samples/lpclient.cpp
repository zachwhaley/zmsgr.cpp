#include <reply.hpp>
#include <request.hpp>

#include <iostream>
#include <thread>

using namespace std;

static const string SOCKET = "ipc://lpsample.ipc";

void server()
{
    zmsgr::RepSocket rep;
    rep.Bind(SOCKET);

    string request;
    if (rep.Recv(&request))
        cout << "Server: Recieved " << request << endl;
    else
        cout << "Server: Recv Failed!" << endl;

    string goodbye = "Goodbye!";
    if (rep.Send(goodbye))
        cout << "Server: Sent " << goodbye << endl;
    else
        cout << "Server: Send Failed!" << endl;
}

void client()
{
    zmsgr::ReqSocket req;
    req.Connect(SOCKET);

    string hello = "Hello!";
    if (req.Send(hello))
        cout << "Client: Sent " << hello << endl;
    else
        cout << "Client: Send Failed!" << endl;

    string reply;
    int retries = 3;
    do {
        if (req.Recv(&reply, 1000))
            break;
        else if (req.Resend(hello))
            cout << "Client: Resend " << hello << endl;
        else break;
    } while (--retries > 0);

    if (!reply.empty())
        cout << "Client: Recieved " << reply << endl;
    else
        cout << "Client: Recv Failed!" << endl;
}

int main(int argc, const char *argv[])
{
    // Start client before server, so that it fails to receive messages
    thread c(client);
    this_thread::sleep_for(2s);
    thread s(server);

    s.join();
    c.join();
    return 0;
}
