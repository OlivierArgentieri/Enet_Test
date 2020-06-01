#pragma once
#include "ENet.h"

#define DELIMITER ':'
#define PORT 1234
class EObject
{
public :
	virtual ~EObject() = default;
	virtual ENetHost* GetHost() const = 0;
	virtual ENetPeer* GetPeer() const = 0;
	virtual void Tick() = 0;
	virtual void CleanUp() = 0;
	virtual void Disconnect() = 0;
	virtual void ReceiveData(const ENetEvent& _event) = 0;
	virtual void RegisterPeer(ENetPeer *_peer) = 0;
	virtual void UnRegisterPeer(ENetPeer* _peer) = 0;
};
