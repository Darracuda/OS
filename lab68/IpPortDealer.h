#pragma once

class IpPortDealer
{
private:
	static const int minPort = 30000;
	static const int maxPort = 50000;

	int currentPort;

public:
	IpPortDealer();
	virtual ~IpPortDealer();
	int GetPort();
};

