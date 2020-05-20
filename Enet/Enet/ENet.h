#pragma once
#include <enet.h>
#include <conio.h>


inline int getch_noblock()
{
	if (_kbhit())
		return _getch();
	else
		return -1;
}
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
	void SendPacket(bool _reliable, const char* _dataStr) const;
	void BroadcastPacket(bool _reliable, const char* _dataStr) const;
	int SetupServer();

	bool IsClient();
	bool IsServer();
	bool IsClientConnected();

	void Update();

	static int Main();
	
	
protected:
	void CleanUpClient();
	void CleanUpServer();


	Mode        m_mode;
	ENetHost    *m_host;    // Client or Server, depending on the mode
	ENetPeer    *m_peer;
};