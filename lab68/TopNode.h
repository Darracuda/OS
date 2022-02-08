#pragma once
#include <string>
#include <vector>
#include "IpPortDealer.h"
#include "ZmqStream.h"

using namespace std;

class TopNode
{
private:
	IpPortDealer _ipPortDealer;
	ZmqStream _zmqStream;
	int _port;
	vector<int> _childNodeIds;

public:
	TopNode();
	virtual ~TopNode();
	void Run();

private:
	string createNode(const int nodeId, const int extraPort1, const int extraPort2);
	string executeOnNode(const int nodeId, const string& whereString, const string& whatString);
	string pingNode(const int nodeId);
	string killNode(const int nodeId);
	string getStatisticsFromNode(const int nodeId);

	bool existsChildNodeId(const int nodeId);
	int runNodeApplication(const int nodeId, const int port, const int extraPort1, const int extraPort2);

    string readString(string& input);
    int readInt(string& input);
};

