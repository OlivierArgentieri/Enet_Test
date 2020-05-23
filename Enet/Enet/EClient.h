#pragma once
#include <string>
#include "ENet.h"
#include "EObject.h"

class EClient : public EObject
{	
private:
	std::string name;
	ENetHost* host;
	ENetPeer* peer;
	
	void Init();
	void Setup();

	void Tick() override;
	void SendPackets(bool _reliable, const char* _dataStr);


public:

	ENetHost* GetHost() const override;
	ENetPeer* GetPeer() const override;

	EClient(const char* _id);
	void Connect(const char* _hostname, int _port, int _timeout = 5000);

};

