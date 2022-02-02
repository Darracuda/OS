#pragma once
#include <string>
#include <vector>
#include "IpPortDealer.h"

using namespace std;

class TopNode
{
private:
	IpPortDealer* pIpPortDealer;
	int port;

	int childNodeId;
	vector<int> childNodeIds;

public:
	TopNode();
	virtual ~TopNode();

	void createNode(const int nodeId);
	void executeOnNode(const int nodeId, const string& whereString, const string& whatString);
	void pingNode(const int nodeId);

};

