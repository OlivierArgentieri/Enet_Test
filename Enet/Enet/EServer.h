#pragma once
#include "EObject.h"
#include "ENet.h"
#include <string>
#include <list>



class EServer : public EObject
{

private:
	std::list<ClientData*> clientDatas;
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
	virtual void RegisterPeer(ENetPeer* _peer);
	virtual void UnRegisterPeer(ENetPeer* _peer);
	void Disconnect() override;
	void BroadcastPacket(bool _reliable, const char* _dataStr);
	void SendPacket(bool _reliable, const char* _dataStr, ENetPeer* _peer, ENetHost* _host) const ;

	void ShowConnectedUser();
	void SendTokenToClient(enet_uint32 _clientID);
	bool IsAllPlayersAreReady();
	void ReceiveData(const ENetEvent& _event) override;
};