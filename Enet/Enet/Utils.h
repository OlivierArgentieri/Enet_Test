#pragma once
#include <string>
#include <vector>

namespace Utils
{
	class Utils
	{
		public :
		static std::vector<std::string> SplitString(std::string _str, char _delimiter);
		static std::string GetLocalIP();
		static std::string HexaDumpToIP(unsigned _hexadump);
		static std::string HexaDumpReverseToIP(unsigned _reverseHexadump);
	};
}

