#pragma once
#include <string>
#include "ENet.h"
#include "EObject.h"

class EClient : public EObject
{
public:
	
private:
	std::string name;
	ENetHost* host;
	ENetPeer* peer;
	enet_uint32 token;
	
	void Init();
	void Setup();

	void Tick() override;

	void SetToken(std::string _strData);

public:

	ENetHost* GetHost() const override;
	ENetPeer* GetPeer() const override;

	EClient(const char* _id);
	~EClient();
	void Connect(const char* _hostname, int _port, int _timeout = 5000);
	void SendPacket(bool _reliable, const char* _dataStr);
	void CleanUp() override;
	void Disconnect() override;

	bool IsConnected();

	void ReceiveData(const ENetEvent& _event) override;
	void RegisterPeer(ENetPeer* _peer) override;
	void UnRegisterPeer(ENetPeer* _peer) override;
};
