#pragma once
#include <enet.h>
#include <conio.h>
#include <queue>

#include <list>
#include "ClientData.h"

class PacketData;

inline int getch_noblock()
{
	if (_kbhit())
		return _getch();
	return -1;
}

namespace MyEnet
{

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

		int SetupServer();


		bool IsClient();
		bool IsServer();
		bool IsClientConnected();

	protected:
		std::list<ClientData*> clientDatas;
		void CleanUpClient();
		void CleanUpServer();


		Mode		m_mode;
		ENetHost* m_host;	// Client or Server, depending on the mode
		ENetPeer* m_peer;

		void RegisterPeer(ENetPeer* _peer);
		void UnregisterPeer(ENetPeer* _peer);
		
		void ReceiveData(const ENetEvent& event);
		void BroadcastPacket(bool _reliable, const char* _dataStr) const;
		void PushPacket(PacketData* _packetData);

		int	m_clientCount;
		std::queue< PacketData*> m_MessageQueue;

	public:
		static int Main();
		void Update();
		
		bool HasMessage() const;
		PacketData* ConsumeMessage();
		void DisposeMessage(PacketData* _packetData);

		void SendPacket(bool _reliable, const char* _dataStr);

		void BroadcastPacket(bool _reliable, const char* _dataStr);
		std::list<ClientData*> GetClientDatas() const;

	};
}