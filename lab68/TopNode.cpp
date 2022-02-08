#include <iostream>
#include <string>
#include "TopNode.h"
#include "execute.h"
#include "ZmqStream.h"

using namespace std;

TopNode::TopNode()
{
	_port = _ipPortDealer.getNextPort();
	_zmqStream.init(_port, ZmqStreamDirection::Down);
	_childNodeIds.clear();
}

TopNode::~TopNode()
{
}

void TopNode::Run()
{
	string command;
	int nodeId;
	string whereString;
	string whatString;
	string response;

	for (;;)
	{
		cout << ">";
		cin >> command;

		if (command == "create")
		{
			cin >> nodeId;
			const int extraPort1 = _ipPortDealer.getNextPort();
			const int extraPort2 = _ipPortDealer.getNextPort();
			response = createNode(nodeId, extraPort1, extraPort2);
			cout << response << endl;
		}
		else if (command == "exec")
		{
			cin >> nodeId;
			cin >> whereString;
			cin >> whatString;
			response = executeOnNode(nodeId, whereString, whatString);
			cout << response << endl;
		}
		else if (command == "ping")
		{
			cin >> nodeId;
			response = pingNode(nodeId);
			cout << response << endl;
		}
		else if (command == "kill")
		{
			cin >> nodeId;
			response = killNode(nodeId);
			cout << response << endl;
		}
		else if (command == "stat")
		{
			cin >> nodeId;
			response = getStatisticsFromNode(nodeId);
			cout << response << endl;
		}
		else if (command == "quit" || command == "exit")
		{
			cout << "Bye" << endl;
			break;
		}
		else
		{
			cout << "Error: Uknown command '" << command << "'" << endl;
			cout << "Known command are:" << endl;
			cout << "create <nodeid> - Create a new node" << endl;
			cout << "exec <nodeid> <string_where> <string_what> - Execute a task on the node with parameters" << endl;
			cout << "ping <nodeid> - Ping the node" << endl;
			cout << "kill <nodeid> - Kill the node" << endl;
			cout << "stat <nodeid> - Get statictics from the node" << endl;
			cout << "quit or exit - Exit the program" << endl;
		}
	}
}

bool TopNode::existsChildNodeId(const int nodeId)
{
	return find(_childNodeIds.begin(), _childNodeIds.end(), nodeId) != _childNodeIds.end();
}

int TopNode::runNodeApplication(const int nodeId, const int port, const int extraPort1, const int extraPort2)
{
	const int parametersLength = 4;
	const string parameters[parametersLength] =
	{
		to_string(nodeId),
		to_string(port),
		to_string(extraPort1),
		to_string(extraPort2)
	};
	int processId = execute("node", parametersLength, parameters);
	return processId;
}

string TopNode::createNode(const int nodeId, const int extraPort1, const int extraPort2)
{
	if (existsChildNodeId(nodeId)) // the nodeId is already in use
		return "Error: Already exists";

	string response = "";
	
	if (_childNodeIds.empty())  // no child nodes under the current node yet
	{
		int processId = runNodeApplication(nodeId, _port, extraPort1, extraPort2);
		response = "Ok: " + to_string(processId);
	}
	else // child node is already created, so pass command to it
	{
		string request = "create " + to_string(nodeId)
			+ " " + to_string(extraPort1)
			+ " " + to_string(extraPort2);
		_zmqStream.send(request);
		response = _zmqStream.receive();
	}
	_childNodeIds.push_back(nodeId);
	return response;
}

string TopNode::executeOnNode(const int nodeId, const string& whereString, const string& whatString)
{
	if (!existsChildNodeId(nodeId)) // the nodeId cannot be found
		return "Error: Not found";

	string request = "exec " + to_string(nodeId)
		+ " " + whereString
		+ " " + whatString;
	_zmqStream.send(request);
	string response = _zmqStream.receive();
	return response;
}

string TopNode::pingNode(const int nodeId)
{
	if (!existsChildNodeId(nodeId)) // the nodeId cannot be found
		return "Ok: 0";

	string request = "ping " + to_string(nodeId);
	_zmqStream.send(request);
	string response = _zmqStream.receive();
	return response;
}

string TopNode::killNode(const int nodeId)
{
	if (!existsChildNodeId(nodeId)) // the nodeId cannot be found
		return "Error: Not found";

	string request = "kill " + to_string(nodeId);
	_zmqStream.send(request);
	string response = _zmqStream.receive();
	return response;
}

string TopNode::getStatisticsFromNode(const int nodeId)
{
	if (nodeId <= 0)  // self
	{
		string statistics = "Port = " + to_string(_port) + ", child node ids =";
		if (_childNodeIds.size() <= 0)
			statistics = statistics + " <empty>";
		else
		{
			for (int i = 0; i < _childNodeIds.size(); i++)
			{
				statistics = statistics + " " + to_string(_childNodeIds[i]);
			}
		}
		return statistics;
	}

	if (!existsChildNodeId(nodeId)) // the nodeId cannot be found
		return "Error: not found";

	string request = "stat " + to_string(nodeId);
	_zmqStream.send(request);
	string response = _zmqStream.receive();
	return response;
}
