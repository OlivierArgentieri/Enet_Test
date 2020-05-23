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
	ENetEvent event;
	/* Connect to some.server.net:1234. */
	enet_address_set_host(&address, _hostname);
	address.port = _port;
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
	MyEnet::ENet::Tick(this);
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
}
