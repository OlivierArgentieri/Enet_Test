#pragma once
#include <enet.h>
#include <conio.h>
#include "EObject.h"


inline int getch_noblock()
{
	if (_kbhit())
		return _getch();
	else
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


		static int Initialize();

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
		static void Tick(EObject* _object);
		static void SendPacket(EObject* _object, bool _reliable, const char* _dataStr);


		static int Main();


	protected:
		void CleanUpClient();
		void CleanUpServer();


		Mode        m_mode;
		ENetHost* m_host;    // Client or Server, depending on the mode
		ENetPeer* m_peer;
	};
}