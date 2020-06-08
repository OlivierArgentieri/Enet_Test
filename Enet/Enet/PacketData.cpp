#include "PacketData.h"
#include "Json/json.hpp"


PacketData::PacketData(ENetPeer* _peer)
{
	m_Type = Unset; // default
	m_Valid = false;
	m_Data = nullptr;
	m_peer = _peer;
}

PacketData::PacketData()
{
	m_Type = Unset; // default
	m_Valid = false;
	m_Data = nullptr;
	m_peer = nullptr;
}

PacketData::~PacketData()
{
	if (m_Data != nullptr)
		free(m_Data);
	m_Data = nullptr;
}

void PacketData::SetType(Type _type)
{
	m_Type = _type;
}

PacketData::Type PacketData::GetType() const
{
	return m_Type;
}

bool PacketData::IsValid() const
{
	return m_Valid;
}

void PacketData::SetStringContent(const char* _string)
{
	m_Content = _string;
	m_Type = Type::String;
	m_Valid = true;
}

void PacketData::SetJsonContent(const char* _string)
{
	json::JSON obj;
	obj["packetData"] = json::Object();
	obj["packetData"]["Content"] = _string;

	m_Content = obj.dump();
	m_Type = Type::JSon;
	m_Valid = true;
}

const char* PacketData::GetContent()
{
	return m_Content.c_str();
}

void* PacketData::Serialize(unsigned int& _size)
{
	// no content !
	if (!m_Valid)
	{
		_size = 0;
		return nullptr;
	}

	// compute the size of the packet over the network
	_size = sizeof(Type) + m_Content.length() + 1;
	// allocate the content we are preparing to send
	m_Data= malloc(_size);
	void* workBuffer = m_Data;

	// write the packet type : json, string, whatever
	Type* ptrType = static_cast<Type*>(workBuffer);
	*ptrType = m_Type;

	// shift buffer 
	workBuffer = (void*)((int)workBuffer + sizeof(Type));

	// then write the content
	char* contentStr = static_cast<char*>(workBuffer);
	strcpy_s(contentStr, m_Content.length() + 1, m_Content.c_str());

	// return it
	return m_Data;
}

void PacketData::Deserialize(void* _data, int _size)
{
	// read the packet type : json, string, ...
	Type* ptrType = static_cast<Type*>(_data);
	m_Type = *ptrType;
	_data = (void*)((int)_data + sizeof(Type));

	if (m_Type == Type::String)
	{
		// then read the content according to the packet type
		char* contentStr = static_cast<char*>(_data);
		m_Content = contentStr;

		m_Valid = true;
	}

	if (m_Type == Type::JSon)
	{
		// then read the content according to the packet type
		char* contentStr = static_cast<char*>(_data);
		json::JSON out = json::JSON::Load(contentStr);
		m_Content = out["packetData"]["Content"].ToString();
		m_Valid = true;
	}
}

void* PacketData::GetData()
{
	return m_peer->data;
}