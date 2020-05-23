#include "EServer.h"
#include <cstdio>


EServer::EServer()
{
	Init();
	Setup();
}

void EServer::Init()
{
	MyEnet::ENet::Initialize();
}

void EServer::Setup()
{
	ENetAddress address;
	/* Bind the server to the default localhost.     */
	/* A specific host address can be specified by   */
	/* enet_address_set_host (& address, "x.x.x.x"); */
	address.host = ENET_HOST_ANY;
	/* Bind the server to port 1234. */
	address.port = 1234;
	host = enet_host_create(&address /* the address to bind the server host to */,
		32      /* allow up to 32 clients and/or outgoing connections */,
		2      /* allow up to 2 channels to be used, 0 and 1 */,
		0      /* assume any amount of incoming bandwidth */,
		0      /* assume any amount of outgoing bandwidth */);
	if (host == NULL)
	{
		fprintf(stderr, "An error occurred while trying to create an ENet server host.\n");
		exit(EXIT_FAILURE);
	}
}

EServer::~EServer()
{
	CleanUp();
}

ENetHost* EServer::GetHost() const
{
	return host;
}

ENetPeer* EServer::GetPeer() const
{
	return peer;
}

void EServer::Tick()
{
	MyEnet::ENet::Tick(this);
}

void EServer::CleanUp()
{
	enet_host_destroy(host);
	host = nullptr;
}

void EServer::Disconnect()
{
}

void EServer::BroadcastPacket(bool _reliable, const char* _dataStr)
{
	// Create a reliable packet of content "_dataStr" 
	ENetPacket* packet = enet_packet_create(_dataStr, strlen(_dataStr) + 1, _reliable ? ENET_PACKET_FLAG_RELIABLE : 0);

	enet_host_broadcast(host, 0, packet);

	enet_host_flush(host);
}
