// Juul Joosten 2013

#include "Random.h"

namespace Indy
{
	float Randf0to1( void)
	{
		return  (float)rand() / (float)RAND_MAX;
	}
}
