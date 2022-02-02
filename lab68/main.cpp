// OS, Lab 6-8, Diana Kolpakova
// variant 4-4-2, top node

#include <iostream>
#include <string>
#include <vector>
#include "TopNode.h"

using namespace std;

int main()
{
	cout << "OS Lab 6-8 (c) Diana Kolpakova, variant 4-4-2" << endl;

	string command;
	int nodeId;
	string whereString;
	string whatString;
	TopNode* pNode = new TopNode();

	for (;;)
	{
		cout << ">";
		cin >> command;

		if (command == "create")
		{
			cin >> nodeId;
			cout << "main: " << command << " " << nodeId << endl;
			pNode->createNode(nodeId);
		}
		else if (command == "exec")
		{
			cin >> nodeId;
			cin >> whereString;
			cin >> whatString;
			cout << "main: " << command << " " << nodeId << "'" << whereString << "' '" << whatString << "'" << endl;
			pNode->executeOnNode(nodeId, whereString, whatString);
		}
		else if (command == "ping")
		{
			cin >> nodeId;
			cout << "main: " << command << " " << nodeId << endl;
			pNode->pingNode(nodeId);
		}
		else if (command == "quit" || command == "exit")
		{
			cout << "main: " << command << endl;
			cout << "Bye" << endl;
			break;
		}
		//else if (command == "test")
		//{
		//	cout << "main: " << command << endl;
		//	int processId = execute("node", "6 10 13");
		//	cout << "main: " << "processId = " << processId << endl;
		//}
		else
		{
			cout << "Error: Uknown command '" << command << "'" << endl;
			cout << "Known command are:" << endl;
			cout << "create <nodeid> - Create a new node" << endl;
			cout << "exec <nodeid> <string_where> <string_what> - Execute a task on the node with parameters" << endl;
			cout << "ping <nodeid> - Ping tbe node" << endl;
			cout << "quit or exit - Exit the prohram" << endl;
		}
	}

	delete pNode;
}

