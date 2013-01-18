#include "stdafx.h"
#include "Bitrate.h"
#include "Rand.h"

CBitrate::CBitrate(
	CPopularity*	popularity,
	unsigned int	numGroups,
	unsigned int*	groups,
	double*			ratios
	)
{
	assert(popularity);
	assert(numGroups);
	assert(groups);
	assert(ratios);

	this->popularity = popularity;

	printf("3. Generating file bitrate...");

	this->bitrates = new unsigned int[this->popularity->NumFiles()];
	assert(this->bitrates);

	double rnd = 0;
	for(unsigned int index = 0; index < this->popularity->NumFiles(); index++)
	{
		rnd = CRand::Generate();
		for(unsigned idx = 0; idx < numGroups; idx++)
		{
			if(DOUBLE_LT(rnd,ratios[idx]))
			{
				this->bitrates[index] = groups[idx];
				break;
			}
			rnd -= ratios[idx];
		}
	}

	printf("done.\n");
}

CBitrate::~CBitrate()
{
	delete[] this->bitrates;
}
