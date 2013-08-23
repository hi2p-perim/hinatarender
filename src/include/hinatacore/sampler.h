#ifndef __HINATA_CORE_SAMPLER_H__
#define __HINATA_CORE_SAMPLER_H__

#include "common.h"
#include <memory>

HINATA_NAMESPACE_BEGIN

class Random;

class Sampler
{
public:

	virtual ~Sampler() {}
	virtual double Next() = 0;
	virtual std::shared_ptr<Random> Rng() = 0;

};

class RandomSampler : public Sampler
{
public:

	RandomSampler();

public:

	double Next();
	std::shared_ptr<Random> Rng() { return rng; }

private:

	std::shared_ptr<Random> rng;

};

HINATA_NAMESPACE_END

#endif // __HINATA_CORE_SAMPLER_H__