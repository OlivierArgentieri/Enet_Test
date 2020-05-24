#include "Utils.h"
#include <sstream>

std::vector<std::string> Utils::Utils::SplitString(std::string _str, char _delimiter)
{
	std::vector<std::string> _toReturn;

	std::istringstream _stream(_str);
	std::string _temp;
	while (std::getline(_stream, _temp, _delimiter))
		_toReturn.push_back(_temp);

	return _toReturn;
}
