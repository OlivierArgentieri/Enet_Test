#include "Utils.h"
#include <sstream>
#include <iostream>
//#include <cryptocpp/hex.h>
//#include <cryptocpp/sha.h>
//#include <cryptocpp/filters.h>

std::vector<std::string> Utils::Utils::SplitString(std::string _str, char _delimiter)
{
	std::vector<std::string> _toReturn;

	std::istringstream _stream(_str);
	std::string _temp;
	while (std::getline(_stream, _temp, _delimiter))
		_toReturn.push_back(_temp);

	return _toReturn;
}

std::string Utils::Utils::GetLocalIP()
{
	return "";
}

std::string Utils::Utils::HexaDumpToIP(unsigned int _hexadump)
{
	std::string _ip = std::to_string(((_hexadump & 0xFF000000) >> 24)) + "."
		+ std::to_string(((_hexadump & 0x00FF0000) >> 16)) + "."
		+ std::to_string(((_hexadump & 0x0000FF00) >> 8)) + "."
		+ std::to_string(((_hexadump & 0x000000FF)));
	
	return _ip;
}

std::string Utils::Utils::HexaDumpReverseToIP(unsigned int _reverseHexadump)
{
	
	std::string _ip = std::to_string(((_reverseHexadump & 0x000000FF))) + "."
		+ std::to_string(((_reverseHexadump & 0x0000FF00) >> 8)) + "."
		+ std::to_string(((_reverseHexadump & 0x00FF0000) >> 16)) + "."
		+ std::to_string(((_reverseHexadump & 0xFF000000)) >> 24);

	return _ip;
}

/*
std::string Utils::Utils::StringToSha1(std::string _str)
{
	std::string string(""); // UTF-8 encoded

	CryptoPP::SHA1 sha1;
	std::string hash;

	CryptoPP::StringSource(_str, true, new CryptoPP::HashFilter(sha1, new CryptoPP::HexEncoder(new CryptoPP::StringSink(hash))));


	std::cout << hash << std::endl;
	return hash;
}*/