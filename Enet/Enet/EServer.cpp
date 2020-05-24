#include "EServer.h"
#include <cstdio>
#include <string>
#include "Utils.h"


EServer::EServer()
{
	clients = std::map<std::string, EServerClientData>();
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
	//MyEnet::ENet::Tick(this);

	ENetEvent event;
	std::string packet_data;
	std::string _delimiter = ":";
	/* Wait up to 1000 milliseconds for an event. */
	if (enet_host_service(host, &event, 1000) > 0)
	{
		switch (event.type)
		{
		case ENET_EVENT_TYPE_CONNECT:
			printf("A new client connected from %x:%u\n",
				event.peer->address.host,
				event.peer->address.port);

			/* Store any relevant client information here. */
			//event.peer->data = (void*)"Client information";
			break;

		case ENET_EVENT_TYPE_RECEIVE:
			printf("A packet of length %u containing %s was received from %s on channel %u.\n",
				event.packet->dataLength,
				event.packet->data,
				event.peer->data,
				event.channelID);

			packet_data = (char*)event.packet->data;
			if (packet_data.substr(0, packet_data.find(_delimiter)) == "CNX")
				RegisterClient(packet_data);

			if (packet_data.substr(0, packet_data.find(_delimiter)) == "DCNX")
				UnRegisterClient(packet_data);

			
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

void EServer::RegisterClient(std::string _clientData)
{
	printf("Add new client");

	std::vector<std::string> _splitData = Utils::Utils::SplitString(_clientData, ':');

	if (_splitData.empty())
	{
		printf("ERROR on ADD clients");
		return;
	}
	
	std::string _name = _splitData[1];
	std::string _id = _splitData[2];
	EServerClientData _client = EServerClientData
	{
		_id,
		_name
	};

	clients[_id] = _client;
}

void EServer::UnRegisterClient(std::string _clientData)
{
	std::vector<std::string> _splitData = Utils::Utils::SplitString(_clientData, ':');

	if (_splitData.empty())
	{
		printf("ERROR on remove client");
		return;
	}
	printf("remove client");
	std::string _id = _splitData[2];

	if (clients.empty()) return;

	clients.erase(_id);
}


void EServer::ShowConnectedUser()
{
	if(clients.empty())
	{
		printf("0 Clients Connected \n");
		return;
	}
	
	printf("Connected Clients: \n");

	for (std::pair<std::string, EServerClientData> _client : clients)
	{
		printf("%s with id : %s \n", clients[_client.first].name.c_str(), clients[_client.first].id.c_str());
	}
}