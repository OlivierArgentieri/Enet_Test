// Enet.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <iostream>
#include "BoardGameServer.h"

int main()
{
	
	BoardGameServer* _gameServer = new BoardGameServer();
	_gameServer->Initialize();
	_gameServer->CreateBalls();
	
	bool bContinue = true;
	do
	{
		_gameServer->ServerLoop();


		//_networkLayer->Update();

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
			std::string myMsg = "UnMessageDeTestBroadcast";
			printf("Sending %s to all the clients.\n", myMsg);

			//_networkLayer->BroadcastPacket(false, myMsg.c_str());

			break;
		}
	} while (bContinue);

/*
	delete _networkLayer;*/
	return 0;
}
