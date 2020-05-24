#include "ENet.h"
#include "EClient.h"

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
				event.peer->data,
				event.channelID);

			packet_data = (char*)event.packet->data;
			if (packet_data.find("DCNX") != std::string::npos)
				printf("remove client");

			else if (packet_data.find("CNX") != std::string::npos)
				printf("Add new client");


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

void EClient::SendPacket(bool _reliable, const char* _dataStr)
{
	MyEnet::ENet::SendPacket(this, _reliable, _dataStr);
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
