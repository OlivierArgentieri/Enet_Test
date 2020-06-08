#include "ENet.h"
#include <iostream>
#include <cstdlib>
#include <stdio.h>
#include "EObject.h"
#include "PacketData.h"
#include "EServer.h"

MyEnet::ENet::ENet()
{}

MyEnet::ENet::~ENet()
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

int MyEnet::ENet::Initialize()
{
	if (enet_initialize() != 0)
	{
		fprintf(stderr, "An error occurred while initializing ENet.\n");
		return EXIT_FAILURE;
	}
	atexit(enet_deinitialize);
	return 1;
}

int MyEnet::ENet::SetupClient()
{
	m_host= enet_host_create(NULL /* create a client host */,
		1 /* only allow 1 outgoing connection */,
		2 /* allow up 2 channels to be used, 0 and 1 */,
		0 /* assume any amount of incoming bandwidth */,
		0 /* assume any amount of outgoing bandwidth */);
	if (m_host == NULL)
	{
		fprintf(stderr,"An error occurred while trying to create an ENet client host.\n");
		exit(EXIT_FAILURE);
	}

	m_mode = Mode::Client;

	return 1;

}

int MyEnet::ENet::ConnectClient()
{
	ENetAddress address;
	ENetEvent event;
	/* Connect to some.server.net:1234. */
	enet_address_set_host(&address, "127.0.0.1");
	address.port = PORT;
	/* Initiate the connection, allocating the two channels 0 and 1. */
	m_peer = enet_host_connect(m_host, &address, 2, 0);
	if (m_peer == NULL)
	{
		fprintf(stderr,
			"No available peers for initiating an ENet connection.\n");
		exit(EXIT_FAILURE);
	}
	/* Wait up to 5 seconds for the connection attempt to succeed. */
	if (enet_host_service(m_host, &event, 5000) > 0 &&
		event.type == ENET_EVENT_TYPE_CONNECT)
	{
		puts("Connection to some.server.net:1234 succeeded.");
	}
	else
	{
		/* Either the 5 seconds are up or a disconnect event was */
		/* received. Reset the peer in the event the 5 seconds   */
		/* had run out without any significant event.            */
		enet_peer_reset(m_peer);
		puts("Connection to some.server.net:1234 failed.");
	}
	return 1;
}

void MyEnet::ENet::DisconnectClient()
{}

void MyEnet::ENet::SendPacket(bool _reliable, const char* _dataStr) const
{
	PacketData packetData;
	packetData.SetStringContent(_dataStr);

	// Create a reliable packet of content "_dataStr" 
	unsigned int dataSize = 0;
	void* data = packetData.Serialize(dataSize);
	ENetPacket* packet = enet_packet_create(data, dataSize, _reliable ? ENET_PACKET_FLAG_RELIABLE : 0);

	/* Send the packet to the peer over channel id 0. */
	/* One could also broadcast the packet by         */
	/* enet_host_broadcast (host, 0, packet);         */
	enet_peer_send(m_peer, 0, packet);

	/* One could just use enet_host_service() instead. */
	enet_host_flush(m_host);
	// enet_host_service()
}

void MyEnet::ENet::SendPacket(bool _reliable, const char* _dataStr)
{
	PacketData _packetData;
	_packetData.SetJsonContent(_dataStr);
	// Create a reliable packet of content "_dataStr" 
	unsigned int _dataSize = 0;
	void* _data = _packetData.Serialize(_dataSize);
	// Create a reliable packet of content "_dataStr" 
	ENetPacket* packet = enet_packet_create(_data, _dataSize, _reliable ? ENET_PACKET_FLAG_RELIABLE : 0);

	/* Send the packet to the peer over channel id 0. /
	/ One could also broadcast the packet by         /
	/ enet_host_broadcast (host, 0, packet);         /*/
	enet_peer_send(m_peer, 0, packet);

	/* One could just use enet_host_service() instead. */
	enet_host_flush(m_host);
	// enet_host_service()
}

void MyEnet::ENet::BroadcastPacket(bool _reliable, const char* _dataStr)
{
}

std::list<ClientData*> MyEnet::ENet::GetClientDatas() const
{
	return clientDatas;
}


void MyEnet::ENet::BroadcastPacket(bool _reliable, const char * _dataStr) const
{
	// Create a reliable packet of content "_dataStr" 
	ENetPacket * packet = enet_packet_create(_dataStr, strlen(_dataStr) + 1, _reliable ? ENET_PACKET_FLAG_RELIABLE : 0);

	enet_host_broadcast(m_host, 0, packet);

	enet_host_flush(m_host);
}

void MyEnet::ENet::PushPacket(PacketData* _packetData)
{
	m_MessageQueue.push(_packetData);
}

int MyEnet::ENet::SetupServer()
{
	ENetAddress address;
	/* Bind the server to the default localhost.     */
	/* A specific host address can be specified by   */
	/* enet_address_set_host (& address, "x.x.x.x"); */
	address.host = ENET_HOST_ANY;
	/* Bind the server to port 1234. */
	address.port = 1234;
	m_host = enet_host_create(&address /* the address to bind the server host to */,
		32      /* allow up to 32 clients and/or outgoing connections */,
		2      /* allow up to 2 channels to be used, 0 and 1 */,
		0      /* assume any amount of incoming bandwidth */,
		0      /* assume any amount of outgoing bandwidth */);
	if (m_host == NULL)
	{
		fprintf(stderr, "An error occurred while trying to create an ENet server host.\n");
		exit(EXIT_FAILURE);
	}
	m_mode = Mode::Server;

	return 1;
}

bool MyEnet::ENet::IsClient()
{
	return m_mode == Mode::Client;
}

bool MyEnet::ENet::IsServer()
{
	return m_mode == Mode::Server;
}

bool MyEnet::ENet::IsClientConnected()
{
	return m_mode == Mode::Client && m_peer != nullptr;
}

void MyEnet::ENet::Update()
{
	ENetEvent event;
	
	/* Wait up to 1000 milliseconds for an event. */
	if (enet_host_service(m_host, &event, 1000) > 0)
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
			UnregisterPeer(event.peer);
			break;
		}
	}
}

bool MyEnet::ENet::HasMessage() const
{
	return !m_MessageQueue.empty();
}

PacketData* MyEnet::ENet::ConsumeMessage()
{
	PacketData* data = m_MessageQueue.front();
	m_MessageQueue.pop();

	return data;
}

void MyEnet::ENet::DisposeMessage(PacketData* _packetData)
{
	delete _packetData;
}


void MyEnet::ENet::RegisterPeer(ENetPeer* _peer)
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


void MyEnet::ENet::ReceiveData(const ENetEvent& event)
{
	ClientData* _client = static_cast<ClientData*>(event.peer->data);
	const char* _sender = _client ? _client->name.c_str() : "Server";

	PacketData* packetData = new PacketData(event.peer);
	packetData->Deserialize(event.packet->data, event.packet->dataLength);

	if (packetData->IsValid())
	{
		printf("A packet of length %u containing %s was received from %s on channel %u.\n",
			event.packet->dataLength,
			packetData->GetContent(),
			_sender,
			event.channelID);

		// Record the packet to be able to use it later
		PushPacket(packetData);
	}
	else
		printf("An invalid packet of length %u was received from %s on channel %u.\n",
			event.packet->dataLength,
			_sender,
			event.channelID);

	/* Clean up the packet now that we're done using it. */
	enet_packet_destroy(event.packet);
}
void MyEnet::ENet::UnregisterPeer(ENetPeer* _peer)
{
	if (_peer == nullptr) return;
	ClientData* _client = static_cast<ClientData*>(_peer->data);

	delete _client;

	_peer = nullptr;
}



int MyEnet::ENet::Main()
{
	ENet *_networkLayer = new ENet();

	if (!_networkLayer->Initialize())
		return EXIT_FAILURE;

	
	char choice = '2';
//	std::cin >> choice;
	switch (choice)
	{
	case '1':
		_networkLayer->SetupClient();
		_networkLayer->ConnectClient();
		break;

	case '2':
		_networkLayer->SetupServer();
		break;

	default:
		std::cout << "Unknow choice";
		break;
	}

	
	std::cout << "ESC key to exit\n";

	if (_networkLayer->IsClientConnected())
	{
		std::cout << "Press S to send a message to the server\n";
		std::cout << "Press Q to disconnect Client\n";
	}
	else
	{
		std::cout << "Press S to send a message to all the clients\n";
	}

	bool bContinue = true;
	
	do
	{
		_networkLayer->Update();

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

				_networkLayer->BroadcastPacket(false, myMsg.c_str());
			
			break;
		}
	} while (bContinue);


	delete _networkLayer;
	return 0;
}

void MyEnet::ENet::CleanUpClient()
{
	DisconnectClient();
	enet_host_destroy(m_host);
	m_mode = Mode::Unset;
	m_host = nullptr;
}

void MyEnet::ENet::CleanUpServer()
{
	enet_host_destroy(m_host);
	m_mode = Mode::Unset;
	m_host = nullptr;
}