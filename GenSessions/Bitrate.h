#ifndef __Bitrate_h__
#define __Bitrate_h__

#include "Popularity.h"

class CBitrate
{
private:
	CPopularity*	popularity;
	unsigned int*	bitrates;

public:
	CBitrate(
		CPopularity*	popularity,
		unsigned int	numGroups,
		unsigned int*	groups,
		double*			ratios
		);
	~CBitrate();

	inline unsigned int	operator[] (unsigned int index) { assert(index < this->popularity->NumFiles()); return this->bitrates[index]; }
};

#endif