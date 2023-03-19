#include "surf/Algorithm.h"

unsigned int ElfHash(const char* str, unsigned int strLen)
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

unsigned int QuadraticResolution(int i)
{
	return ((i * i) + i) / 2;
}