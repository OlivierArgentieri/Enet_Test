#pragma once
#include <string>
#include "ENet.h"
#include "EObject.h"

class EClient : public EObject
{
public :

	ENetHost* GetHost() const override;
	ENetPeer* GetPeer() const override;
	
private:
	std::string name;
	ENetHost* host;
	ENetPeer* peer;
	
	void Init();
	void Setup();
	void Connect(const char* _hostname, int _port, int _timeout);

	void Tick();
	void SendPackets(bool _reliable, const char* _dataStr);

};

