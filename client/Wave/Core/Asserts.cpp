#include "Asserts.h"

bool VkCheckError(VkResult error, const char* function, const char* file, int line)
{
	if (error)
	{
		core::Log(ELogType::Error, "[Vulkan Error #{}] {} @ {} {}", static_cast<int32_t>(error), function, file, line);
		return false;
	}

	return true;
}
