#include "EPacketData.h"
#include <iostream>
#include <json.hpp>

EPacketData::EPacketData()
{
	type = Unset; // default
	isValid = false;
	data = nullptr;
}

EPacketData::~EPacketData()
{
	if (data != nullptr)
		free(data);
	data = nullptr;
}

void EPacketData::SetType(Type _type)
{
	type = _type;
}

EPacketData::Type EPacketData::GetType() const
{
	return type;
}

bool EPacketData::IsValid() const
{
	return isValid;
}

void EPacketData::SetStringContent(const char* _string)
{
	content = _string;
	type = Type::String;
	isValid = true;
}

void EPacketData::SetJsonContent(const char* _string)
{
	json::JSON obj;
	obj["packetData"] = json::Object();
	obj["packetData"]["Content"] = _string;

	content = obj.dump();
	type = Type::JSon;
	isValid = true;
}

const char* EPacketData::GetContent()
{
	return content.c_str();
}

void* EPacketData::Serialize(unsigned int& _size)
{
	// no content !
	if (!isValid)
	{
		_size = 0;
		return nullptr;
	}

	// compute the size of the packet over the network
	_size = sizeof(Type) + content.length() + 1;
	// allocate the content we are preparing to send
	data = malloc(_size);
	void* workBuffer = data;

	// write the packet type : json, string, whatever
	Type* ptrType = static_cast<Type*>(workBuffer);
	*ptrType = type;

	// shift buffer 
	workBuffer = (void*)((int)workBuffer + sizeof(Type));

	// then write the content
	char* contentStr = static_cast<char*>(workBuffer);
	strcpy_s(contentStr, content.length() + 1, content.c_str());

	// return it
	return data;
}

void EPacketData::Deserialize(void* _data, int _size)
{
	// read the packet type : json, string, ...
	Type* ptrType = static_cast<Type*>(_data);
	type = *ptrType;
	_data = (void*)((int)_data + sizeof(Type));

	if (type == Type::String)
	{
		// then read the content according to the packet type
		char* contentStr = static_cast<char*>(_data);
		content = contentStr;

		isValid = true;
	}

	if (type == Type::JSon)
	{
		// then read the content according to the packet type
		char* contentStr = static_cast<char*>(_data);
		json::JSON out = json::JSON::Load(contentStr);
		content = out["packetData"]["Content"].ToString();
		isValid = true;
	}
}