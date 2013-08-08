#ifndef __HINATA_CORE_RANDOM_H__
#define __HINATA_CORE_RANDOM_H__

#include "common.h"
#include <random>

HINATA_NAMESPACE_BEGIN

/*!
	Random.
	Random number generator.
*/
class Random
{
public:

	Random(unsigned int seed);
	double Next();
	void SetSeed(unsigned int seed);

private:

	std::mt19937 engine;
	std::uniform_real_distribution<double> uniformReal;

};

HINATA_NAMESPACE_END

#endif // __HINATA_CORE_RANDOM_H__