// OS, Lab 6-8, Diana Kolpakova
// variant 4-4-2, working node

#include <iostream>
#include <string>
#include <vector>
#include "WorkingNode.h"

using namespace std;

// argument 0 - full path to the current program
// argument 1 - this node id
// argument 2 - IP port to communicate to parent node
// argument 3 - IP port to communicate to child nodes

int main(int argc, char* argv[])
{
	cout << "node: " << argc << " arguments" << endl;

	if (argc != 5)
	{
		cout << "Error: Invalid numer of arguments" << endl;
		return -1;
	}

	int nodeId = -1;
	int parentPort = -1;
	int leftChildPort = -1;
	int rightChildPort = -1;
	try
	{
		nodeId = stoi(argv[1]);
		parentPort = stoi(argv[2]);
		leftChildPort = stoi(argv[3]);
		rightChildPort = stoi(argv[4]);
	}
	catch (exception&)
	{
		cout << "Error: Invalid values in arguments" << endl;
		return -1;
	}

	cout << "nodeId: " << nodeId << endl;
	cout << "parentPort: " << parentPort << endl;
	cout << "leftChildPort: " << leftChildPort << endl;
	cout << "rightChildPort: " << rightChildPort << endl;

	WorkingNode* pNode = new WorkingNode(nodeId, parentPort, leftChildPort, rightChildPort);
	pNode->Run();
	delete pNode;

	cout << "node: exit" << endl;
}
