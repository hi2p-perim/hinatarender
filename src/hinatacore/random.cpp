#include "pch.h"
#include <hinatacore/random.h>

HINATA_NAMESPACE_BEGIN

Random::Random( unsigned int seed )
{
	SetSeed(seed);
}

double Random::Next()
{
	return uniformReal(engine);
}

void Random::SetSeed( unsigned int seed )
{
	engine.seed(seed);
	uniformReal.reset();
}

HINATA_NAMESPACE_END