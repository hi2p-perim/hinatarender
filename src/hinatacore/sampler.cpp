#include "pch.h"
#include <hinatacore/sampler.h>
#include <hinatacore/random.h>

HINATA_NAMESPACE_BEGIN

RandomSampler::RandomSampler()
{
	rng = std::make_shared<Random>((unsigned long)std::time(nullptr));
}

double RandomSampler::Next()
{
	return rng->Next();
}

HINATA_NAMESPACE_END


