#include <iostream>
#include <zmq.hpp>
#include "execute.h"
#include "WorkingNode.h"
#include "ZmqStream.h"

using namespace std;

WorkingNode::WorkingNode(int nodeId, int parentPort, int leftChildPort, int rightChildPort)
{
	_nodeId = nodeId;
	_parentPort = parentPort;
	_leftChildPort = leftChildPort;
	_rightChildPort = rightChildPort;
	_zmqParentStream.init(parentPort, ZmqStreamDirection::Up);
	_zmqLeftChildStream.init(leftChildPort, ZmqStreamDirection::Down);
	_zmqRightChildStream.init(rightChildPort, ZmqStreamDirection::Down);
	_leftChildNodeIds.clear();
	_rightChildNodeIds.clear();
}

void WorkingNode::Run()
{
	for (;;)
	{
		string request = _zmqParentStream.receive();
		cout << "WorkingNode - Info: Request =  '" << request << "'" << endl;

		string command = readString(request);
		string response = "";

		if (command == "create")
		{
			int nodeId = readInt(request);
			int extraPort1 = readInt(request);
			int extraPort2 = readInt(request);
			//cout << "node(" << this->nodeId << "): " << command << " " << nodeId << " " << extraPort1 << " " << extraPort2 << endl;
			response = createNode(nodeId, extraPort1, extraPort2);
		}
		else if (command == "exec")
		{
			int nodeId = readInt(request);
			string whereString = readString(request);
			string whatString = readString(request);
			//cout << "node(" << this->nodeId << "): " << command << " " << nodeId << "'" << whereString << "' '" << whatString << "'" << endl;
			response = executeOnNode(nodeId, whereString, whatString);
		}
		else if (command == "ping")
		{
			int nodeId = readInt(request);
			//cout << "node(" << this->nodeId << "): " << command << " " << nodeId << endl;
			response = pingNode(nodeId);
		}
		else if (command == "kill")
		{
			int nodeId = readInt(request);
			//cout << "node(" << this->nodeId << "): " << command << " " << nodeId << endl;
			response = killNode(nodeId);
			//break;
		}
		else if (command == "stat")
		{
			int nodeId = readInt(request);
			//cout << "node(" << _nodeId << ", " << getppid() << "): " << command << " " << nodeId << endl;
			response = getStatisticsFromNode(nodeId);
		}
		else
		{
			cout << "node(" << _nodeId << "): " << command << endl;
			response = "Error: Uknown command '" + command + "'";
		}
		_zmqParentStream.send(response);
	}
}

bool WorkingNode::existsChildNodeId(const int nodeId)
{
	return existsLeftChildNodeId(nodeId) || existsRightChildNodeId(nodeId);
}

bool WorkingNode::existsLeftChildNodeId(const int nodeId)
{
	return find(_leftChildNodeIds.begin(), _leftChildNodeIds.end(), nodeId) != _leftChildNodeIds.end();
}

bool WorkingNode::existsRightChildNodeId(const int nodeId)
{
	return find(_rightChildNodeIds.begin(), _rightChildNodeIds.end(), nodeId) != _rightChildNodeIds.end();
}

int WorkingNode::runNodeApplication(const int nodeId, const int port, const int extraPort1, const int extraPort2)
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

string WorkingNode::createNode(const int nodeId, const int extraPort1, const int extraPort2)
{
	cout << "WorkingNode('" << _nodeId << "') - Info: Create node '" << nodeId << "'" << endl;

	if (existsChildNodeId(nodeId)) // the nodeId is already in use
		return "WorkingNode ('" + to_string(_nodeId) + "') - Error: The node '" + to_string(nodeId) + "' was already created earlier";

	string response = "NMNM";

	if (_leftChildNodeIds.size() <= _rightChildNodeIds.size())    // left side is shorter
	{
		//cout << "WorkingNode('" << _nodeId << "') - Info: Left if shorter" << endl;
		if (_leftChildNodeIds.empty())  // no child nodes under the current node on the left yet
		{
			int processId = runNodeApplication(nodeId, _leftChildPort, extraPort1, extraPort2);
			response = "WorkingNode('" + to_string(_nodeId) + "') - Info: The node '" + to_string(nodeId) + "' on the left was successfully created by WorkingNode. The processId = " + to_string(processId);
		}
		else // child node on the left is already created, so pass command to it
		{
			string request = "create " + to_string(nodeId)
				+ " " + to_string(extraPort1)
				+ " " + to_string(extraPort2);
			_zmqLeftChildStream.send(request);
			//cout << "WorkingNode - Info: Command to create node '" << nodeId << "' on the left was successfully passed to node '" << leftChildNodeIds[0] << "'" << endl;
			response = _zmqLeftChildStream.receive();
			//cout << "WorkingNode - Info: Response =  '" << response << "'" << endl;
		}
		_leftChildNodeIds.push_back(nodeId);
	}
	else    // right side is shorter
	{
		//cout << "WorkingNode('" << _nodeId << "') - Info: Right if shorter" << endl;
		if (_rightChildNodeIds.empty())  // no child nodes under the current node on the right yet
		{
			int processId = runNodeApplication(nodeId, _rightChildPort, extraPort1, extraPort2);
			response = "WorkingNode('" + to_string(_nodeId) + "') - Info: The node '" + to_string(nodeId) + "' on the right was successfully created by WorkingNode. The processId = " + to_string(processId);
		}
		else // child node on the right is already created, so pass command to it
		{
			string request = "create " + to_string(nodeId)
				+ " " + to_string(extraPort1)
				+ " " + to_string(extraPort2);
			_zmqRightChildStream.send(request);
			//cout << "WorkingNode - Info: Command to create node '" << nodeId << "' on the right was successfully passed to node '" << leftChildNodeIds[0] << "'" << endl;
			response = _zmqRightChildStream.receive();
			//cout << "WorkingNode - Info: Response =  '" << response << "'" << endl;
		}
		_rightChildNodeIds.push_back(nodeId);
	}
	return response;
}

string WorkingNode::executeOnNode(const int nodeId, const string& whereString, const string& whatString)
{
	return "ddd-exec";
}

string WorkingNode::pingNode(const int nodeId)
{
	return "ddd-ping";
}

string WorkingNode::killNode(const int nodeId)
{
	return "ddd-kill";
}

string WorkingNode::getStatisticsFromNode(const int nodeId)
{
	if (_nodeId == nodeId)  // self
	{
		cout << "WorkingNode('" << _nodeId << "') - Info: get self stat from  '" << nodeId << "'" << endl;

		string statistics = "WorkingNode('" + to_string(_nodeId) + "'):"
			+ " parent port = " + to_string(_parentPort)
			+ ", left port = " + to_string(_leftChildPort)
			+ ", right port = " + to_string(_rightChildPort)
			+ ", left child node ids =";
		if (_leftChildNodeIds.size() <= 0)
			statistics = statistics + " <empty>";
		else
		{
			for (int i = 0; i < _leftChildNodeIds.size(); i++)
			{
				statistics = statistics + " " + to_string(_leftChildNodeIds[i]);
			}
		}
		statistics = statistics + ", right child node ids =";
		if (_rightChildNodeIds.size() <= 0)
			statistics = statistics + " <empty>";
		else
		{
			for (int i = 0; i < _rightChildNodeIds.size(); i++)
			{
				statistics = statistics + " " + to_string(_rightChildNodeIds[i]);
			}
		}
		return statistics;
	}

	if (existsLeftChildNodeId(nodeId)) // the nodeId is found on left
	{
		cout << "WorkingNode('" << _nodeId << "') - Info: get left stat from  '" << nodeId << "'" << endl;
		string request = "stat " + to_string(nodeId);
		_zmqLeftChildStream.send(request);
		string response = _zmqLeftChildStream.receive();
		return response;
	}

	if (existsRightChildNodeId(nodeId)) // the nodeId is found on right
	{
		cout << "WorkingNode('" << _nodeId << "')- Info: get right stat from  '" << nodeId << "'" << endl;
		string request = "stat " + to_string(nodeId);
		_zmqRightChildStream.send(request);
		string response = _zmqRightChildStream.receive();
		return response;
	}

	return "WorkingNode('" + to_string(_nodeId) + "') - Error: The node '" + to_string(nodeId) + "' cannot be found";
}

string WorkingNode::readString(string& input)
{
	size_t pos = input.find(' ');
	if (pos == string::npos)
	{
		string result = input;
		input = "";
		return result;
	}
	else
	{
		string result = input.substr(0, pos);
		input = input.substr(pos + 1);
		return result;
	}
}

int WorkingNode::readInt(string& input)
{
	string str = readString(input);
	int result = stoi(str);
	return result;
}
