#include "ZmqStream.h"
#include <zmq.hpp>

ZmqStream::ZmqStream()
{
}

ZmqStream::~ZmqStream()
{
}

void ZmqStream::init(int port, ZmqStreamDirection direction)
{
    address = baseAddress + ":" + to_string(port);

    if (direction == ZmqStreamDirection::Up)
    {
        socket = zmq::socket_t(context, zmq::socket_type::rep);
        socket.set(zmq::sockopt::rcvtimeo, 3000);	// 3 sec
        socket.set(zmq::sockopt::sndtimeo, 3000);	// 3 sec
        socket.bind(address);
    }
    else if (direction == ZmqStreamDirection::Down)
    {
        socket = zmq::socket_t(context, zmq::socket_type::req);
        socket.set(zmq::sockopt::rcvtimeo, 3000);	// 3 sec
        socket.set(zmq::sockopt::sndtimeo, 3000);	// 3 sec
        socket.connect(address);
    }
}

bool ZmqStream::send(const string& str)
{
    try
    {
        zmq::message_t outmsg(str);
        zmq::send_result_t result = socket.send(outmsg, zmq::send_flags::none);
        return result.has_value();
    }
    catch (zmq::error_t ex)
    {
        return false;
    }
}

bool ZmqStream::receive(string& str)
{
    try
    {
        zmq::message_t inmsg;
        zmq::recv_result_t result = socket.recv(inmsg, zmq::recv_flags::none);
        str = inmsg.to_string();
        return result.has_value();
    }
    catch (zmq::error_t ex)
    {
        return false;
    }
}