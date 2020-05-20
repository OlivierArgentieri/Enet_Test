#pragma once
#include <enet.h>

class ENet
{
	
public:
	ENet();
	~ENet();

	enum Mode {
		Unset,
		Server,
		Client
	};

public:
	int Initialize();

	int SetupClient();
	int ConnectClient();
	void DisconnectClient();

	int SetupServer();


	bool IsClient();
	bool IsServer();
	bool IsClientConnected();
	
protected:
	void CleanUpClient();
	void CleanUpServer();


	Mode        m_mode;
	ENetHost    *m_host;    // Client or Server, depending on the mode
	ENetPeer    *m_peer;
};