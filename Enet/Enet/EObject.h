#pragma once
#include "ENet.h"

class EObject
{
public :
	virtual ~EObject() = default;
	virtual ENetHost* GetHost() const = 0;
	virtual ENetPeer* GetPeer() const = 0;
	virtual void Tick() = 0;
};
