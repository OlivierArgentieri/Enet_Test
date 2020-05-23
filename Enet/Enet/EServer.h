#pragma once
#include "EObject.h"
#include "ENet.h"


class EServer : public EObject
{

private:
	ENetHost* host;
	ENetPeer* peer;


	void Init();
	void Setup();

	
	
public:
	EServer();
	~EServer()override;
	
	ENetHost* GetHost() const override;
	ENetPeer* GetPeer() const override;
	void Tick() override;
	void CleanUp() override;
	void Disconnect() override;
	void BroadcastPacket(bool _reliable, const char* _dataStr);
	
};

