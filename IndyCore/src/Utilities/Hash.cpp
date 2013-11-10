// Juul Joosten 2013

#include "Hash.h"

#include <cstring>


namespace Indy
{
	unsigned int HashFNV1( const char* string)
	{
		const unsigned int length = strlen(string) + 1;
		unsigned int hash = 2166136261u;

		for ( unsigned int i = 0; i < length; ++i)
		{
			hash ^= *string++;
			hash *= 16777619u;
		}

		return hash;
	}
}