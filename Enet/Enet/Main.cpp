// Enet.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "enet/enet.h"
#include "Enet.h"
#include "EClient.h"
#include "EServer.h"

int main()
{
	EObject* _networkLayer = nullptr;
	EClient* _client = nullptr;
	EServer* _server = nullptr;

	
	//return MyEnet::ENet::Main();
	
	char choice;
	std::cin >> choice;

	switch (choice)
	{
	case '1':
		_networkLayer = _client = new EClient("test");
		_client->Connect("127.0.0.1", 1234);
		break;

	case '2':
		_networkLayer = _server = new EServer();
		break;

	default:
		std::cout << "Unknow choice";
		break;
	}


	std::cout << "ESC key to exit\n";

	if (_client != nullptr)
	{
		std::cout << "Press S to send a message to the server\n";
		std::cout << "Press Q to disconnect Client\n";
	}
	else
	{
		std::cout << "Press S to send a message to all the clients\n";
		std::cout << "Press L to view list of clients connected \n";
	}

	bool bContinue = true;
	do
	{
		_networkLayer->Tick();

		int lastKey = getch_noblock();
		if (lastKey < 0)
			continue;
		lastKey = toupper(lastKey);

		switch (lastKey)
		{
			
		case VK_ESCAPE:
			bContinue = false;
			break;

		case 'S':
			if (_client != nullptr)
			{
				const char* myMsg = "UnMessageDeTest";
				printf("Sending %s to the server.\n", myMsg);
				_client->SendPacket(false, myMsg);
			}
			else
			{
				const char* myMsg = "UnMessageDeTestBroadcast";
				printf("Sending %s to all the clients.\n", myMsg);

				_server->BroadcastPacket(false, myMsg);
			}
			break;

		case 'Q':
			if (_client->IsConnected())
			{
				_networkLayer->Disconnect();
				std::cout << "Press R to reconnect the Client to the server\n";
			}
			break;

		case 'R':
			if (_client != nullptr)
			{
				_client->Connect("localhost", 1234);
				std::cout << "Press Q to disconnect Client\n";
			}

		case 'L':
			if (_server != nullptr)
			{
				_server->ShowConnectedUser();
			}
			break;
		}
	} while (bContinue);


	delete _networkLayer;
	
	return 0;
}
