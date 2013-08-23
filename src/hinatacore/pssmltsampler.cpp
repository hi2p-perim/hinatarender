#include "pch.h"
#include <hinatacore/pssmltsampler.h>
#include <hinatacore/random.h>

HINATA_NAMESPACE_BEGIN

RestorableSampler::RestorableSampler()
{
	initialSeed = (unsigned long)std::time(nullptr);
	rng = std::make_shared<Random>(initialSeed);
	currentIndex = 0;
}

RestorableSampler::RestorableSampler( const RestorableSampler& o )
{
	initialSeed = o.initialSeed;
	rng = std::make_shared<Random>(initialSeed);
	currentIndex = 0;
}

double RestorableSampler::Next()
{
	currentIndex++;
	return rng->Next();
}

void RestorableSampler::SetIndex( int index )
{
	// Reset the initial seed
	//if (index < currentIndex)
	//{
	//	currentIndex = 0;
	//	rng->SetSeed(initialSeed);
	//}

	currentIndex = 0;
	rng->SetSeed(initialSeed);

	// Generate samples until the given index
	while (currentIndex < index)
	{
		// Discard the value
		currentIndex++;
		rng->Next();
	}
}

LazyPSSSampler::LazyPSSSampler( double s1, double s2 )
	: s1(s1)
	, s2(s2)
{
	logRatio = -std::log(s2 / s1);

	time = 0;
	largeStepTime = 0;
	largeStep = false;
	currentIndex = 0;
}

void LazyPSSSampler::Accept()
{
	if (largeStep)
	{
		// Update large step time
		largeStepTime = time;
	}

	time++;
	prevSamples.clear();
	currentIndex = 0;
}

void LazyPSSSampler::Reject()
{
	// Restore samples
	for (auto& v : prevSamples)
	{
		u[std::get<0>(v)] = std::get<1>(v);
	}

	prevSamples.clear();
	currentIndex = 0;
}

double LazyPSSSampler::Next()
{
	return PrimarySample(currentIndex++);
}

void LazyPSSSampler::SetLargeStep( bool largeStep )
{
	this->largeStep = largeStep;
}

bool LazyPSSSampler::LargeStep()
{
	return largeStep;
}

double LazyPSSSampler::PrimarySample( int i )
{
	// Not sampled yet
	while (i >= (int)u.size())
	{
		u.push_back(Sample(rng->Next()));
	}

	// If the modified time of the requested sample is not updated
	// it requires the lazy evaluation of mutations.
	if (u[i].modify < time)
	{
		if (largeStep)
		{
			// Large step case

			// Save sample in order to restore previous state
			prevSamples.push_back(std::make_tuple(i, u[i]));

			// Update the modified time and value
			u[i].modify = time;
			u[i].value = rng->Next();
		}
		else
		{
			// Small step case

			// If the modified time is not updated since the last accepted
			// large step mutation, then update sample to the state.
			// Note that there is no need to go back before largeStepTime
			// because these samples are independent of the sample on largeStepTime.
			if (u[i].modify < largeStepTime)
			{
				u[i].modify = largeStepTime;
				u[i].value = rng->Next();
			}

			// Lazy evaluation of Mutate
			while (u[i].modify < time - 1)
			{
				u[i].value = Mutate(u[i].value);
				u[i].modify++;
			}

			// Save state
			prevSamples.push_back(std::make_tuple(i, u[i]));

			// Update the modified time and value
			u[i].value = Mutate(u[i].value);
			u[i].modify++;
		}
	}

	return u[i].value;
}

double LazyPSSSampler::Mutate( double value )
{
	double u = rng->Next();
	bool positive = u < 0.5;

	// Convert to [0, 1]
	u = positive ? u * 2.0 : 2.0 * (u - 0.5); 

	double dv = s2 * std::exp(logRatio * u);

	if (positive)
	{
		value += dv;
		if (value > 1) value -= 1;
	}
	else
	{
		value -= dv;
		if (value < 0) value += 1;
	}

	return value;
}

HINATA_NAMESPACE_END




