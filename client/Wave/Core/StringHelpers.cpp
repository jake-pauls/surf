#include "StringHelpers.h"

bool core::StringHelpers::Equals(const std::string& a, const std::string& b)
{
	return a.compare(b) == 0;
}