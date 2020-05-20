// Enet.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "enet/enet.h"
#include "Enet.h"

int main()
{
	ENet *_networkLayer = new ENet();

	if (!_networkLayer->Initialize())
		return EXIT_FAILURE;

	_networkLayer->SetupServer();


	delete _networkLayer;
	return 0;
}
