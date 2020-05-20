#include "ENet.h"
#include <iostream>
#include <cstdlib>
#include <stdio.h>

ENet::ENet()
{}

ENet::~ENet()
{
	switch (m_mode)
	{
		case Server:
			CleanUpServer();
			break;

		case Client:
			CleanUpClient();
			break;
		
		default:;
	}
}

int ENet::Initialize()
{
	if (enet_initialize() != 0)
	{
		fprintf(stderr, "An error occurred while initializing ENet.\n");
		return EXIT_FAILURE;
	}
	atexit(enet_deinitialize);
	return 1;
}

int ENet::SetupClient()
{
	ENetHost * client;
	client = enet_host_create(NULL /* create a client host */,
		1 /* only allow 1 outgoing connection */,
		2 /* allow up 2 channels to be used, 0 and 1 */,
		0 /* assume any amount of incoming bandwidth */,
		0 /* assume any amount of outgoing bandwidth */);
	if (client == NULL)
	{
		fprintf(stderr,"An error occurred while trying to create an ENet client host.\n");
		exit(EXIT_FAILURE);
	}

	m_mode = Mode::Client;

	return 1;

}

int ENet::ConnectClient()
{
	return 0;

}

void ENet::DisconnectClient()
{

}

int ENet::SetupServer()
{
	ENetAddress address;
	ENetHost * server;
	/* Bind the server to the default localhost.     */
	/* A specific host address can be specified by   */
	/* enet_address_set_host (& address, "x.x.x.x"); */
	address.host = ENET_HOST_ANY;
	/* Bind the server to port 1234. */
	address.port = 1234;
	server = enet_host_create(&address /* the address to bind the server host to */,
		32      /* allow up to 32 clients and/or outgoing connections */,
		2      /* allow up to 2 channels to be used, 0 and 1 */,
		0      /* assume any amount of incoming bandwidth */,
		0      /* assume any amount of outgoing bandwidth */);
	if (server == NULL)
	{
		fprintf(stderr, "An error occurred while trying to create an ENet server host.\n");
		exit(EXIT_FAILURE);
	}

	m_mode = Mode::Server;

	return 1;
	
}

bool ENet::IsClient()
{
	return m_mode == Mode::Client;
}

bool ENet::IsServer()
{
	return m_mode == Mode::Server;

}

bool ENet::IsClientConnected()
{
	return false;
}

void ENet::CleanUpClient()
{
	DisconnectClient();
	enet_host_destroy(m_host);
	m_mode = Mode::Unset;
	m_host = nullptr;
}

void ENet::CleanUpServer()
{
	enet_host_destroy(m_host);
	m_mode = Mode::Unset;
	m_host = nullptr;
}
