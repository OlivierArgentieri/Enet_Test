#pragma once
#include <string>

class ClientData
{
public:

	ClientData(ENetPeer* _peer, const char* _name) { peer = _peer; name = _name; }
	std::string name;
	ENetPeer* peer;
	bool isReady;
};
