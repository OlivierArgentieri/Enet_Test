#pragma once
#include <string>
#include "ENet.h"

class EClient
{
public :
	std::string name;

	
private:
	ENet* host;

	void Init();
	void Setup();
	void Connect();
	
};

