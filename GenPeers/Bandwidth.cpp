#include "stdafx.h"
#include "Bandwidth.h"
#include "Rand.h"

CBandwidth::CBandwidth(
	unsigned int	numPeers,
	unsigned int	numGroups,
	unsigned int*	groups,
	double*			ratios
	)
{
	assert(numPeers);
	assert(numGroups);
	assert(groups);
	assert(ratios);

	this->numPeers = numPeers;

	this->bw = new unsigned int[numPeers];
	assert(this->bw);

	double rnd;
	for(unsigned int index = 0; index < numPeers; index++)
	{
		rnd = CRand::Generate();

		for(unsigned int idx = 0; idx < numGroups; idx++)
		{
			if(DOUBLE_LT(rnd, ratios[idx]))
			{
				this->bw[index] = groups[idx];
				break;
			}
			rnd -= ratios[idx];
		}
	}
}

CBandwidth::~CBandwidth()
{
	delete[] this->bw;
}

