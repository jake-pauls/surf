#ifndef Algorithm_h
#define Algorithm_h

/// @brief Implementation of the ElfHash algorithm
/// @param str String buffer containing data to represent as a hash
/// @param strLen Length of the string buffer
/// @return Computed ElfHash of the string buffer
/// @reference https://www.programmingalgorithms.com/algorithm/elf-hash/c/
inline unsigned int ElfHash(const char* str, unsigned int strLen)
{
	unsigned int hash = 0;
	unsigned int x = 0;
	unsigned int i = 0;

	for (i = 0; i < strLen; str++, i++)
	{
		hash = (hash << 4) + (*str);

		if ((x = hash & 0xF0000000L) != 0)
		{
			hash ^= (x >> 24);
		}

		hash &= ~x;
	}

	return hash;
}

/// @brief Quadratic resolution function for a hash table
inline unsigned int QuadraticResolution(int i)
{
	return ((i * i) + i) / 2;
}

#endif /* Algorithm_h */