#pragma once
#include <string>

class EPacketData
{
public :
	EPacketData();
	~EPacketData();

	enum Type {
		Unset,
		String,
		JSon
	};

	void SetType(Type _type);
	Type GetType() const;

	bool IsValid() const; // false is data setup is not correct

	void SetStringContent(const char* _string);
	void SetJsonContent(const char* _string);
	
	virtual const char* GetContent();	// returns the data hold in the packet, as a string

	virtual void* Serialize(unsigned int& _size);	// returns the data prepared for the network, according to our choices (compression, etc...)
	virtual void Deserialize(void* _data, int _size); // reads the data from the network

protected:
	Type		type;
	bool		isValid;
	std::string content;
	void* data;
};

