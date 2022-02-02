#pragma once
#include <string>

using namespace std;

class WorkingNode
{
private:
	int nodeId;
	int parentPort;
	int leftChildPort;
	int rightChildPort;
    int leftChildNodeId;
    int rightChildNodeId;
    vector<int> leftChildNodeIds;
    vector<int> rightChildNodeIds;

public:
	WorkingNode(int nodeId, int parentPort, int leftChildPort, int rightChildPort);
	void Run();
private:
	void createNode(int nodeId);
	void executeOnNode(int nodeId, string whereString, string whatString);
	void pingNode(int nodeId);

};

