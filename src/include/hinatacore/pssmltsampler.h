#ifndef __HINATA_CORE_PSSMLT_SAMPLER_H__
#define __HINATA_CORE_PSSMLT_SAMPLER_H__

#include "sampler.h"
#include <memory>
#include <vector>
#include <tuple>

HINATA_NAMESPACE_BEGIN

class RestorableSampler : public Sampler
{
public:

	RestorableSampler();
	RestorableSampler(const RestorableSampler& o);

public:

	double Next();
	void SetIndex(int index);
	int Index() { return currentIndex; }
	std::shared_ptr<Random> Rng() { return rng; }

private:

	unsigned int initialSeed;
	std::shared_ptr<Random> rng;

	// Number of generated samples
	int currentIndex;

};

// Kelemen's sampling strategy
class LazyPSSSampler : public Sampler
{
public:

	struct Sample
	{
		Sample(double value)
			: value(value)
			, modify(0)
		{}

		double value;		// Sample value
		long long modify;	// Last modified time
	};

public:

	LazyPSSSampler(double s1, double s2);

public:

	void Accept();
	void Reject();
	double Next();
	void SetLargeStep(bool largeStep);
	bool LargeStep();
	void SetRng(const std::shared_ptr<Random>& rng) { this->rng = rng; }
	std::shared_ptr<Random> Rng() { return rng; }

private:

	double PrimarySample(int i);
	double Mutate(double value);

private:

	double s1, s2;
	double logRatio;

	std::shared_ptr<Random> rng;

	long long time;				// Number of accepted mutations
	long long largeStepTime;	// Time of the last accepted large step
	bool largeStep;				// Indicates the next mutation is the large step

	int currentIndex;
	std::vector<Sample> u;
	std::vector<std::tuple<int, Sample>> prevSamples;

};

HINATA_NAMESPACE_END

#endif // __HINATA_CORE_PSSMLT_SAMPLER_H__