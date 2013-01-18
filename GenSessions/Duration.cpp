#include "stdafx.h"
#include "Duration.h"
#include "Rand.h"
#include "DistNormal.h"

CDuration::CDuration(
	CPopularity*	popularity,
	unsigned int	numGroups,
	SDurationGroup*	groups,
	double			maxDuration,
	double			minDuration
	)
{
	assert(popularity);
	assert(numGroups);
	assert(groups);

	this->numGroups = numGroups;
	this->groups = groups;
	this->maxDuration = maxDuration;
	this->minDuration = minDuration;

	printf("2. Generating file duration...");

	// Generate file durations for each duration group
	for(unsigned int index = 0; index < this->numGroups; index++)
		this->GenerateGroup(index);

	assert(this->duration.size() == popularity->NumFiles());

	// Sort the durations
	sort(this->duration.begin(), this->duration.end());

	// Match the durations to files using a shuffle algorithm
	unsigned int idx;
	double aux;
	for(unsigned int index = 0; index < popularity->NumFiles()-1; index++)
	{
		idx = (unsigned int)((CRand::Generate() * (popularity->NumFiles() - index)) + index);

		aux = this->duration[index];
		this->duration[index] = this->duration[idx];
		this->duration[idx] = aux;
	}

	printf("done.\n");
}

CDuration::~CDuration()
{
}

void CDuration::GenerateGroup(
	unsigned int	index
	)
{
	double minp = DistNormal((double)this->minDuration, this->groups[index].mu, this->groups[index].sigma);
	double maxp = DistNormal((double)this->maxDuration, this->groups[index].mu, this->groups[index].sigma);

	for(unsigned int idx = 0; idx < this->groups[index].num; idx++)
	{
		double p = CRand::Generate(minp, maxp);
		double x = DistNormalInv(p, this->groups[index].mu, this->groups[index].sigma);
		assert((x >= (double)this->minDuration) && (x <= (double)this->maxDuration));
		this->duration.push_back(x);
	}
}
