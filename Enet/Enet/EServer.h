#pragma once
#include "EObject.h"
#include "ENet.h"
#include <vector>
#include <string>
#include <map>


struct EServerClientData
{
	enet_uint32 id;
	std::string ip;
	std::string name;
};


class EServer : public EObject
{

private:
	std::map<enet_uint32, EServerClientData> clients;
	ENetHost* host;
	ENetPeer* peer;


	void Init();
	void Setup();

	enet_uint32 RegisterClient(ENetEvent _event);
	void UnRegisterClient(ENetEvent _event);

public:
	EServer();
	~EServer()override;
	
	ENetHost* GetHost() const override;
	ENetPeer* GetPeer() const override;
	void Tick() override;
	void CleanUp() override;
	void Disconnect() override;
	void BroadcastPacket(bool _reliable, const char* _dataStr);
	void SendPacket(bool _reliable, const char* _dataStr, ENetPeer* _peer, ENetHost* _host) const ;

	void ShowConnectedUser();
	void SendTokenToClient(enet_uint32 _clientID);
};