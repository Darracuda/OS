#include <iostream>

#include "WorkingNode.h"

using namespace std;

WorkingNode::WorkingNode(int nodeId, int parentPort, int leftChildPort, int rightChildPort)
{
	this->nodeId = nodeId;
	this->parentPort = parentPort;
	this->leftChildPort = leftChildPort;
	this->rightChildPort = rightChildPort;
}

void WorkingNode::Run()
{
	for(;;)
	{ 
		cout << "*" << endl;
	}
}
