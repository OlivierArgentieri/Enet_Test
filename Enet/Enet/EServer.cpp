#include "EServer.h"
#include <cstdio>
#include <string>
#include "Utils.h"
#include "EPacketData.h"
#include <iostream>


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
	address.port = PORT;
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
			RegisterPeer(event.peer);
			/* Store any relevant client information here. */
			//event.peer->data = (void*)"Client information";
			break;

		case ENET_EVENT_TYPE_RECEIVE:
			ReceiveData(event);
			break;

		case ENET_EVENT_TYPE_DISCONNECT:
			UnRegisterPeer(event.peer);
			break;
		}
	}
}

void EServer::CleanUp()
{
	enet_host_destroy(host);
	host = nullptr;
}

void EServer::RegisterPeer(ENetPeer* _peer)
{
	if (_peer == nullptr) return;

	std::string _name = "Client" + std::to_string(clientDatas.size());
	EClientData* _client = new EClientData(_peer, _name.c_str());
	_client->peer->data = _client;
	
	
	clientDatas.push_back(_client);

	printf("A new client %s connected from %x:%u.\n",
		_client->name.c_str(),
		_peer->address.host,
		_peer->address.port);

}
void EServer::UnRegisterPeer(ENetPeer* _peer)
{
	if (_peer == nullptr) return;
	EClientData* _client = static_cast<EClientData*>(_peer->data);

	delete _client;
	
	_peer = nullptr;
}

void EServer::ReceiveData(const ENetEvent& _event)
{
	MyEnet::ENet::ReceiveData(_event);

	if (IsAllPlayersAreReady())
	{
		std::cout << "all ready" << std::endl;
		BroadcastPacket(true, "allReady");
	}
}

void EServer::Disconnect()
{
}

void EServer::BroadcastPacket(bool _reliable, const char* _dataStr)
{
	EPacketData _packetData;
	_packetData.SetJsonContent(_dataStr);

	// Create a reliable packet of content "_dataStr" 
	unsigned int _dataSize = 0;
	void* _data = _packetData.Serialize(_dataSize);
	// Create a reliable packet of content "_dataStr" 
	ENetPacket* packet = enet_packet_create(_data, _dataSize, _reliable ? ENET_PACKET_FLAG_RELIABLE : 0);

	enet_host_broadcast(host, 0, packet);

	enet_host_flush(host);
}

void EServer::SendPacket(bool _reliable, const char* _dataStr, ENetPeer *_peer, ENetHost *_host) const
{
	// Create a reliable packet of content "_dataStr" 
	ENetPacket* packet = enet_packet_create(_dataStr, strlen(_dataStr) + 1, _reliable ? ENET_PACKET_FLAG_RELIABLE : 0);

	/* Send the packet to the peer over channel id 0. /
	/ One could also broadcast the packet by         /
	/ enet_host_broadcast (host, 0, packet);         /*/
	enet_peer_send(_peer, 0, packet);

	/* One could just use enet_host_service() instead. */
	enet_host_flush(_host);
	// enet_host_service()
}

enet_uint32 EServer::RegisterClient(ENetEvent _event)
{
	if (_event.packet == nullptr) return 0;
	
	std::string _clientData = (char*)_event.packet->data;
	std::string _clientIP = Utils::Utils::HexaDumpReverseToIP(_event.peer->address.host);
	
	std::vector<std::string> _splitData = Utils::Utils::SplitString(_clientData, DELIMITER);

	if (_splitData.empty())
	{
		printf("ERROR on ADD clients \n");
		return 0;
	}

	std::string _name = _splitData[1];
	enet_uint32 _id = _event.peer->connectID;

	
	EServerClientData _client = EServerClientData
	{
		_id,
		_clientIP,
		_name
	};

	clients[_id] = _client;
	return _id;
}

void EServer::UnRegisterClient(ENetEvent _event)
{
	if (_event.packet == nullptr) return;

	std::string _clientData = (char*)_event.packet->data;
	std::vector<std::string> _splitData = Utils::Utils::SplitString(_clientData, DELIMITER);

	if (_splitData.empty())
	{
		printf("ERROR on remove client");
		return;
	}
	
	enet_uint32 _id = _event.peer->connectID;

	if (clients.empty()) return;

	clients.erase(_id);
}


void EServer::ShowConnectedUser()
{
	if(clientDatas.empty())
	{
		printf("0 Clients Connected \n");
		return;
	}
	
	printf("Connected Clients: \n");

	for (EClientData* _client : clientDatas)
	{
		printf(" with name : %s \n", _client->name.c_str());
	}
}

void EServer::SendTokenToClient(enet_uint32 _clientID)
{
	if (_clientID == 0) return;

	
	EServerClientData _client = clients[_clientID];
	
	std::string _token = "TOKEN:"+std::to_string(_client.id);
	
	BroadcastPacket(true, _token.c_str());
}

bool EServer::IsAllPlayersAreReady()
{
	for (EClientData* _client : clientDatas)
	{
		if (!_client->isReady)
			return false;
	}
	return true;
}