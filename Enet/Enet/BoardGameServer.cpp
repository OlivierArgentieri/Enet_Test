#include "BoardGameServer.h"
#include "PacketData.h"
#include <iostream>
#include <glm/glm/vec3.hpp>

#include <Json/json.hpp>
void BoardGameServer::ServerLoop()
{
	m_networkLayer->Update();
	
	if (m_networkLayer->HasMessage())
	{
		PacketData* message = m_networkLayer->ConsumeMessage();
		
		std::cout << "oui : " << message->GetContent() << std::endl;
		if (!strcmp(message->GetContent(), "ready"))
		{
			RandomPos();
			
			SetClientReady(message);
			std::cout << "receive ready" << std::endl;
			if(IsAllPlayersAreReady())
			{
				std::cout << "all ready" << std::endl;
			}
		}
		
		m_networkLayer->DisposeMessage(message);
	}
}

bool BoardGameServer::IsAllPlayersAreReady()
{
	for (ClientData* _client : m_networkLayer->GetClientDatas())
	{
		if (!_client->isReady)
			return false;
	}
	return true;
}

void BoardGameServer::Initialize()
{
	m_networkLayer->Initialize();
	m_networkLayer->SetupServer();
}

BoardGameServer::BoardGameServer()
{
	m_networkLayer = new MyEnet::ENet();
}

BoardGameServer::~BoardGameServer()
{
	delete m_networkLayer;
}

void BoardGameServer::SetClientReady(PacketData* _message)
{
	ClientData* _client = static_cast<ClientData*>(_message->GetData());
	_client->isReady = true;
}


glm::vec3 BoardGameServer::RandomPos()
{
	int _Min_Max = 45;
	int _posX = (rand() * (_Min_Max - (-_Min_Max)) / RAND_MAX) + (-_Min_Max);
	int _posZ = (rand() * (_Min_Max - (-_Min_Max)) / RAND_MAX) + (-_Min_Max);
	return glm::vec3(_posX, 0, _posZ);
}

std::string BoardGameServer::CreateBalls()
{
	/**/
	// Player
	json::JSON _jsonObj;

	_jsonObj["packetData"] = json::Object();
	_jsonObj["packetData"]["Type"] = "setup";

	
	for (int i = 0; i < 10; i++)
	{
		glm::vec3 _pos = RandomPos();
		_jsonObj["packetData"]["Content"]["Ball" + std::to_string(i)]["x"] = _pos.x;
		_jsonObj["packetData"]["Content"]["Ball" + std::to_string(i)]["y"] = _pos.y;
		_jsonObj["packetData"]["Content"]["Ball" + std::to_string(i)]["z"] = _pos.z;

		/*
		while (!CheckPos(_pos))
			_pos = RandomPos();
		*/
		
	}
	std::cout << _jsonObj.dump() << std::endl;
	return _jsonObj.dump();
}

/*
bool BoardGameServer::CheckPos(glm::vec3 _pos)
{
	for (size_t i = 0; i < m_ballsPositions.size(); i++)
	{
		float _dist = glm::length(_pos - m_ballsPositions[i]);
		float _scale = glm::length(m_ballsPositions[i]->GetScale());
		if (_dist < _scale)
			return false;
	}
	return true;
}*/


void BoardGameServer::Test()
{
	json::JSON _jsonObject;
/**/
	_jsonObject["packetData"] = json::Object();
	_jsonObject["packetData"]["Type"] = "setup";
	_jsonObject["packetData"]["Content"] = "test";
	_jsonObject["packetData"]["Content"]["Ball1"]["x"] = "test";
	_jsonObject["packetData"]["Content"]["Ball1"]["y"] = "test";
	_jsonObject["packetData"]["Content"]["Ball1"]["z"] = "test";
	
	_jsonObject["packetData"]["Content"]["Ball2"]["x"] = "test";
	_jsonObject["packetData"]["Content"]["Ball2"]["y"] = "test";
	_jsonObject["packetData"]["Content"]["Ball2"]["z"] = "test";
	
	_jsonObject["packetData"]["Content"]["Ball3"]["x"] = "test";
	_jsonObject["packetData"]["Content"]["Ball3"]["y"] = "test";
	_jsonObject["packetData"]["Content"]["Ball3"]["z"] = "test";

	std::cout << _jsonObject.dump() << std::endl;
}