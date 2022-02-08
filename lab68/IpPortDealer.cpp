#include <stdexcept>
#include "IpPortDealer.h"

using namespace std;

IpPortDealer::IpPortDealer()
{
    currentPort = minPort;
}

IpPortDealer::~IpPortDealer()
{
}

int IpPortDealer::getNextPort()
{
    if (currentPort >= maxPort)
        throw runtime_error("IpPortDealer: no free IP ports");
    int port = currentPort;
    currentPort ++;
    return port;
}
