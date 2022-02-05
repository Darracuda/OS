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
		socket.bind(address);
	}
	else if (direction == ZmqStreamDirection::Down)
	{
		socket = zmq::socket_t(context, zmq::socket_type::req);
		socket.connect(address);
	}
}

void ZmqStream::send(const string& str)
{
	zmq::message_t outmsg(str);
	socket.send(outmsg, zmq::send_flags::none);
}

string ZmqStream::receive()
{
	zmq::message_t inmsg;
    zmq::recv_result_t result = socket.recv(inmsg, zmq::recv_flags::none);
	string str = inmsg.to_string();
	return str;
}
