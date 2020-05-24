#include "Utils.h"
#include <sstream>
#include <iostream>

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