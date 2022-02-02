#include <iostream>
#include <string>
#include "TopNode.h"
#include "execute.h"

using namespace std;

TopNode::TopNode()
{
    pIpPortDealer = new IpPortDealer();
    port = pIpPortDealer->GetPort();

    childNodeId = -1;
}

TopNode::~TopNode()
{
    if (pIpPortDealer != nullptr)
        delete pIpPortDealer;
}

void TopNode::createNode(const int nodeId)
{
    cout << "Info: Create node '" << nodeId << "'" << endl;
    int extraPort1 = pIpPortDealer->GetPort();
    int extraPort2 = pIpPortDealer->GetPort();

    const int parametersLength = 4;
    const string parameters[parametersLength] =
    {
        to_string(nodeId),
        to_string(port),
        to_string(extraPort1),
        to_string(extraPort2)
    };

    if (childNodeId < 0)
    {
        // create the first child node
        int processId = execute("node", parametersLength, parameters);
        childNodeId = nodeId;
        childNodeIds.push_back(nodeId);
    }
    else if (find(childNodeIds.begin(), childNodeIds.end(), nodeId) != childNodeIds.end())
    {
        // so that nodeId is already in use
    }
    else
    {
        //int processId = pass_createNode(port, nodeId);
        childNodeIds.push_back(nodeId);
    }
}

void TopNode::executeOnNode(const int nodeId, const string& whereString, const string& whatString)
{
    cout << "Info: Execute on node '" << nodeId << "'. Find '" << whatString << "' in '" << whereString << "'" << endl;
}

void TopNode::pingNode(const int nodeId)
{
    cout << "Info: Ping node '" << nodeId << "'" << endl;
}
