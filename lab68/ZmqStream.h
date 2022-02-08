#pragma once
#include <string>
#include <zmq.hpp>

using namespace std;

enum class ZmqStreamDirection { Up, Down };

class ZmqStream
{
private:
    const string baseAddress = "tcp://127.0.0.1"; //"tcp://192.168.18.1";

    string address;
    zmq::context_t context;
    zmq::socket_t socket;

public:
    ZmqStream();
    virtual ~ZmqStream();
    void init(int port, ZmqStreamDirection direction);

    bool send(const string& str);
    bool receive(string& str);
};