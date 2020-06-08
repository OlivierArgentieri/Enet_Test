#include "EServer.h"
#include <cstdio>
#include <string>
#include "Utils.h"
#include <iostream>
#include "PacketData.h"


EServer::EServer()
{
	Init();
	Setup();
}

void EServer::Init()
{
	//MyEnet::ENet::Initialize();
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
	ClientData* _client = new ClientData(_peer, _name.c_str());
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
	ClientData* _client = static_cast<ClientData*>(_peer->data);

	delete _client;
	
	_peer = nullptr;
}

void EServer::ReceiveData(const ENetEvent& _event)
{
	ClientData* _client = static_cast<ClientData*>(_event.peer->data);
	const char* _sender = _client ? _client->name.c_str() : "Server";

	PacketData* packetData = new PacketData();
	packetData->Deserialize(_event.packet->data, _event.packet->dataLength);

	if (packetData->IsValid())
	{
		printf("A packet of length %u containing %s was received from %s on channel %u.\n",
			_event.packet->dataLength,
			packetData->GetContent(),
			_sender,
			_event.channelID);

		// Record the packet to be able to use it later
		//PushPacket(packetData);
	}
	else
		printf("An invalid packet of length %u was received from %s on channel %u.\n",
			_event.packet->dataLength,
			_sender,
			_event.channelID);

	/* Clean up the packet now that we're done using it. */
	enet_packet_destroy(_event.packet);
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
	PacketData _packetData;
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

	/*
	ServerClientData _client = ServerClientData
	{
		_id,
		_clientIP,
		_name
	};

	clients[_id] = _client;
	return _id;*/
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
	/*
	if (clients.empty()) return;

	clients.erase(_id);*/
}


void EServer::ShowConnectedUser()
{
	if(clientDatas.empty())
	{
		printf("0 Clients Connected \n");
		return;
	}
	
	printf("Connected Clients: \n");

	for (ClientData* _client : clientDatas)
	{
		printf(" with name : %s \n", _client->name.c_str());
	}
}

bool EServer::IsAllPlayersAreReady()
{
	for (ClientData* _client : clientDatas)
	{
		if (!_client->isReady)
			return false;
	}
	return true;
}
