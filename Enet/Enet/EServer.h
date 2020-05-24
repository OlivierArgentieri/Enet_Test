#pragma once
#include "EObject.h"
#include "ENet.h"
#include <vector>
#include <string>
#include <map>


struct EServerClientData
{
	std::string id;
	std::string name;
};


class EServer : public EObject
{

private:
	std::map<std::string, EServerClientData> clients;
	ENetHost* host;
	ENetPeer* peer;


	void Init();
	void Setup();

	void RegisterClient(std::string _clientData);
	void UnRegisterClient(std::string _clientData);

public:
	EServer();
	~EServer()override;
	
	ENetHost* GetHost() const override;
	ENetPeer* GetPeer() const override;
	void Tick() override;
	void CleanUp() override;
	void Disconnect() override;
	void BroadcastPacket(bool _reliable, const char* _dataStr);

	void ShowConnectedUser();

};

