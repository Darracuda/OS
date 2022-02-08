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

		string command = readString(request);
		string response = "";

		if (command == "create")
		{
			int nodeId = readInt(request);
			int extraPort1 = readInt(request);
			int extraPort2 = readInt(request);
			response = createNode(nodeId, extraPort1, extraPort2);
		}
		else if (command == "exec")
		{
			int nodeId = readInt(request);
			string whereString = readString(request);
			string whatString = readString(request);
			response = executeOnNode(nodeId, whereString, whatString);
		}
		else if (command == "ping")
		{
			int nodeId = readInt(request);
			response = pingNode(nodeId);
		}
		else if (command == "kill")
		{
			int nodeId = readInt(request);
			response = killNode(nodeId);
		}
		else if (command == "stat")
		{
			int nodeId = readInt(request);
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
	if (existsChildNodeId(nodeId)) // the nodeId is already in use
		return "Error: Already exists";

	string response;

	if (_leftChildNodeIds.size() <= _rightChildNodeIds.size())    // left side is shorter
	{
		if (_leftChildNodeIds.empty())  // no child nodes under the current node on the left yet
		{
			int processId = runNodeApplication(nodeId, _leftChildPort, extraPort1, extraPort2);
			response = "Ok: " + to_string(processId);
		}
		else // child node on the left is already created, so pass command to it
		{
			string request = "create " + to_string(nodeId) + " " + to_string(extraPort1) + " " + to_string(extraPort2);
			_zmqLeftChildStream.send(request);
			response = _zmqLeftChildStream.receive();
		}
		_leftChildNodeIds.push_back(nodeId);
	}
	else    // right side is shorter
	{
		if (_rightChildNodeIds.empty())  // no child nodes under the current node on the right yet
		{
			int processId = runNodeApplication(nodeId, _rightChildPort, extraPort1, extraPort2);
			response = "Ok: " + to_string(processId);
		}
		else // child node on the right is already created, so pass command to it
		{
			string request = "create " + to_string(nodeId) + " " + to_string(extraPort1) + " " + to_string(extraPort2);
			_zmqRightChildStream.send(request);
			response = _zmqRightChildStream.receive();
		}
		_rightChildNodeIds.push_back(nodeId);
	}
	return response;
}

string WorkingNode::executeOnNode(const int nodeId, const string& whereString, const string& whatString)
{
	if (_nodeId == nodeId)  // self
	{
		vector<size_t> positions = getPositions(whereString, whatString);
		string response = "";
		if (positions.size() <= 0)
			response = "Ok: " + to_string(nodeId) + ": -1";
		else
		{
			for (int i = 0; i < positions.size(); i++)
				response = response + ":" + to_string(positions[i]);
		}
		response = "Ok: " + to_string(_nodeId) + response;
		return response;
	}

	if (existsLeftChildNodeId(nodeId)) // the nodeId is found on left
	{
		string request = "exec " + to_string(nodeId) + " " + whereString + " " + whatString;
		_zmqLeftChildStream.send(request);
		string response = _zmqLeftChildStream.receive();
		return response;
	}

	if (existsRightChildNodeId(nodeId)) // the nodeId is found on right
	{
		string request = "exec " + to_string(nodeId) + " " + whereString + " " + whatString;
		_zmqRightChildStream.send(request);
		string response = _zmqRightChildStream.receive();
		return response;
	}

	return "Error: Not found";
}

string WorkingNode::pingNode(const int nodeId)
{
	if (_nodeId == nodeId)  // self
	{
		return "OK: 1";
	}

	if (existsLeftChildNodeId(nodeId)) // the nodeId is found on left
	{
		string request = "ping " + to_string(nodeId);
		_zmqLeftChildStream.send(request);
		string response = _zmqLeftChildStream.receive();
		return response;
	}

	if (existsRightChildNodeId(nodeId)) // the nodeId is found on right
	{
		string request = "ping " + to_string(nodeId);
		_zmqRightChildStream.send(request);
		string response = _zmqRightChildStream.receive();
		return response;
	}

	return "Ok: 0";
}

string WorkingNode::killNode(const int nodeId)
{
	if (_nodeId == nodeId)  // self
	{
		return "OK ('" + to_string(_nodeId) + "')";
	}

	if (existsLeftChildNodeId(nodeId)) // the nodeId is found on left
	{
		string request = "kill " + to_string(nodeId);
		_zmqLeftChildStream.send(request);
		string response = _zmqLeftChildStream.receive();
		return response;
	}

	if (existsRightChildNodeId(nodeId)) // the nodeId is found on right
	{
		string request = "kill " + to_string(nodeId);
		_zmqRightChildStream.send(request);
		string response = _zmqRightChildStream.receive();
		return response;
	}

	return "Error: Not found";
}

string WorkingNode::getStatisticsFromNode(const int nodeId)
{
	if (_nodeId == nodeId)  // self
	{
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
		string request = "stat " + to_string(nodeId);
		_zmqLeftChildStream.send(request);
		string response = _zmqLeftChildStream.receive();
		return response;
	}

	if (existsRightChildNodeId(nodeId)) // the nodeId is found on right
	{
		string request = "stat " + to_string(nodeId);
		_zmqRightChildStream.send(request);
		string response = _zmqRightChildStream.receive();
		return response;
	}

	return "Error: Not found";
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

vector<size_t> WorkingNode::getPositions(const string& whereString, const string& whatString)
{
	vector<size_t> positions;

	for (size_t beginPosition = 0, foundAtPosition = 0;
		beginPosition < whereString.length(); beginPosition = foundAtPosition + 1)
	{
		foundAtPosition = whereString.find(whatString, beginPosition);
		if (foundAtPosition == string::npos)
			break;
		positions.push_back(foundAtPosition);
	}

	return positions;
}
