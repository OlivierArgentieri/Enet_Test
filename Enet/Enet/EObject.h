#pragma once
#include "ENet.h"

#define DELIMITER ':'
class EObject
{
public :
	virtual ~EObject() = default;
	virtual ENetHost* GetHost() const = 0;
	virtual ENetPeer* GetPeer() const = 0;
	virtual void Tick() = 0;
	virtual void CleanUp() = 0;
	virtual void Disconnect() = 0;
};
