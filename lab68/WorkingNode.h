#pragma once
#include <string>
#include <vector>
#include "ZmqStream.h"

using namespace std;

class WorkingNode
{
private:
	int _nodeId;
	ZmqStream _zmqParentStream;
	ZmqStream _zmqLeftChildStream;
	ZmqStream _zmqRightChildStream;
	int _parentPort;
	int _leftChildPort;
	int _rightChildPort;
	vector<int> _leftChildNodeIds;
	vector<int> _rightChildNodeIds;

public:
	WorkingNode(int nodeId, int parentPort, int leftChildPort, int rightChildPort);
	void Run();

private:
	string createNode(const int nodeId, const int extraPort1, const int extraPort2);
	string executeOnNode(const int nodeId, const string& whereString, const string& whatString);
	string pingNode(const int nodeId);
	string killNode(const int nodeId);
	string getStatisticsFromNode(const int nodeId);

	bool existsChildNodeId(const int nodeId);
	bool existsLeftChildNodeId(const int nodeId);
	bool existsRightChildNodeId(const int nodeId);
	int runNodeApplication(const int nodeId, const int port, const int extraPort1, const int extraPort2);

	string readString(string& input);
	int readInt(string& input);

	vector<size_t> getPositions(const string& whereString, const string& whatString);
};