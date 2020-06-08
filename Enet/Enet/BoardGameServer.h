#pragma once
#include "ENet.h"
#include <glm/vec3.hpp>

class BoardGameServer
{

private:
	MyEnet::ENet* m_networkLayer;
	std::vector<glm::vec3> m_ballsPositions;
	
public:
	BoardGameServer();
	~BoardGameServer();

	void ServerLoop();
	bool IsAllPlayersAreReady();
	void Initialize();
	void SetClientReady(PacketData* _message);
	glm::vec3 RandomPos();
	std::string CreateBalls();
	static void Test();
};