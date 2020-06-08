#pragma once
#include <string>
#include "ENet.h"

class PacketData
{
public:
	PacketData(ENetPeer* _peer);
	PacketData();
	~PacketData();

	enum Type {
		Unset,
		String,
		JSon
	};

	void SetType(Type _type);
	Type GetType() const;

	bool IsValid() const;
	void SetStringContent(const char* _string);
	void SetJsonContent(const char* _string);

	virtual const char* GetContent();	// returns the data hold in the packet, as a string

	virtual void* Serialize(unsigned int& _size);	// returns the data prepared for the network, according to our choices (compression, etc...)
	virtual void Deserialize(void* _data, int _size); // reads the data from the network
	virtual void* GetData();
	
protected:
	Type		m_Type;
	bool		m_Valid;
	std::string m_Content;
	void* m_Data;
	ENetPeer* m_peer;
};