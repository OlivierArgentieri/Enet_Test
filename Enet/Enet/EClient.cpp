#include "ENet.h"
#include "EClient.h"
#include <iostream>
#include "Utils.h"
#include "EPacketData.h"

ENetHost* EClient::GetHost() const
{
	return host;
}

ENetPeer* EClient::GetPeer() const
{
	return peer;
}

EClient::EClient(const char* _id)
{
	token = 0;
	name = _id;
	Init();
	Setup();
}

EClient::~EClient()
{
	CleanUp();
}

void EClient::Init()
{
	MyEnet::ENet::Initialize();
}

void EClient::Setup()
{
	host = enet_host_create(NULL /* create a client host */,
		1 /* only allow 1 outgoing connection */,
		2 /* allow up 2 channels to be used, 0 and 1 */,
		0 /* assume any amount of incoming bandwidth */,
		0 /* assume any amount of outgoing bandwidth */);
	if (host == NULL)
	{
		fprintf(stderr, "An error occurred while trying to create an ENet client host.\n");
		exit(EXIT_FAILURE);
	}
}

void EClient::Connect(const char* _hostname, int _port, int _timeout)
{
	ENetAddress address;
	ENetEvent event = ENetEvent();

	std::string _cnxStr = "CNX:" + name + ":" + "testID";
	
	/* Connect to some.server.net:1234. */
	enet_address_set_host(&address, _hostname);
	address.port = _port;

	// create packet of user data
	event.packet = enet_packet_create("test", strlen("test") + 1, true? ENET_PACKET_FLAG_RELIABLE : 0);
	//event.peer->data = (void*)"test";

	/* Initiate the connection, allocating the two channels 0 and 1. */
	peer = enet_host_connect(host, &address, 2, 0);
	if (peer == NULL)
	{
		fprintf(stderr, "No available peers for initiating an ENet connection.\n");
		exit(EXIT_FAILURE);
	}

	if (enet_host_service(host, &event, _timeout) > 0 && event.type == ENET_EVENT_TYPE_CONNECT)
	{
		printf("Connection to %s:%i succeeded.", _hostname, _port);
		
		SendPacket(true, _cnxStr.c_str());
	}
	else
	{
		/* had run out without any significant event. */
		enet_peer_reset(peer);
		printf("Connection to %s:%i failed.", _hostname, _port);
	}
}

void EClient::Tick()
{
	//MyEnet::ENet::Tick(this);

	ENetEvent event;
	std::string packet_data;
	/* Wait up to 1000 milliseconds for an event. */
	if (enet_host_service(host, &event, 1000) > 0)
	{
		switch (event.type)
		{
		case ENET_EVENT_TYPE_CONNECT:
		// server reserved ?
			break;

		case ENET_EVENT_TYPE_RECEIVE:
			printf("A packet of length %u containing %s was received from %s on channel %u.\n",
				event.packet->dataLength,
				event.packet->data,
				Utils::Utils::HexaDumpReverseToIP(event.peer->address.host).c_str(),
				event.channelID);

			packet_data = (char*)event.packet->data;

			if (packet_data.substr(0, packet_data.find(DELIMITER)) == "TOKEN")
				SetToken(packet_data);
			
			/* Clean up the packet now that we're done using it. */
			enet_packet_destroy(event.packet);
			break;

		case ENET_EVENT_TYPE_DISCONNECT:
			printf("%s disconnected.\n", event.peer->data);
			/* Reset the peer's client information. */
			event.peer->data = NULL;
		}
	}
}

void EClient::SetToken(std::string _strData)
{
	if (token != 0) return; // not mine
	std::vector<std::string> _datas = Utils::Utils::SplitString(_strData, DELIMITER);

	token = strtoul(_datas[1].c_str(), NULL, 10);
	
	std::cout << "RECEIVE TOKEN : " << std::to_string(token) <<  std::endl;


	
	std::cout << "SHA-1 :" << "" << std::endl;
}

void EClient::SendPacket(bool _reliable, const char* _dataStr)
{
	EPacketData _packetData;
	_packetData.SetStringContent(_dataStr);
	// Create a reliable packet of content "_dataStr" 
	unsigned int _dataSize = 0;
	void* _data = _packetData.Serialize(_dataSize);
	ENetPacket* packet = enet_packet_create(_dataStr, strlen(_dataStr) + 1, _reliable ? ENET_PACKET_FLAG_RELIABLE : 0);

	/* Send the packet to the peer over channel id 0. /
	/ One could also broadcast the packet by         /
	/ enet_host_broadcast (host, 0, packet);         /*/
	enet_peer_send(_object->GetPeer(), 0, packet);

	/* One could just use enet_host_service() instead. */
	enet_host_flush(_object->GetHost());
	// enet_host_service()
}


bool EClient::IsConnected()
{
	return peer != nullptr;
}

void EClient::CleanUp()
{
	Disconnect();
	enet_host_destroy(host);
	host = nullptr;

}

void EClient::Disconnect()
{
	std::string _dcnxStr = "DCNX:" + name + ":" + "testID";
	SendPacket(true, _dcnxStr.c_str());
}
