#pragma once

#include <vector>
#include <string>

namespace core
{
	class FileHelpers
	{
	public:
		/// @brief Reads binary file into an array of chars
		/// @param fileName Name of the binary file to open and read
		/// @return Array of characters in the binary
		static std::vector<char> ReadBinaryFile(const std::string& fileName);
	};
}