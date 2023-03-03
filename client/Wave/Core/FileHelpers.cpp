#include "FileHelpers.h"

#include <fstream>

std::vector<char> core::FileHelpers::ReadBinaryFile(const std::string& fileName)
{
	std::ifstream fileStream = std::ifstream(fileName, std::ios::ate | std::ios::binary);

	if (!fileStream.is_open())
	{
		const char* error = "Failed to open stream for file";
		core::Log(ELogType::Error, "{}: {}", error, fileName);
		throw std::runtime_error(error);
	}

	// Use EOF position to determine buffer size
	size_t fileSize = (size_t) fileStream.tellg();
	std::vector<char> buffer = std::vector<char>(fileSize);

	fileStream.seekg(0);
	fileStream.read(buffer.data(), fileSize);
	fileStream.close();

	return buffer;
}