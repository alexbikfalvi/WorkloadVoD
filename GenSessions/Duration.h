#ifndef __Duration_h__
#define __Duration_h__

#include "Popularity.h"

struct SDurationGroup
{
	double			mu;
	double			sigma;
	unsigned int	num;
};

class CDuration
{
private:
	unsigned int	numGroups;
	SDurationGroup*	groups;
	double			maxDuration;
	double			minDuration;
	vector<double>	duration;

public:
	CDuration(
		CPopularity*	popularity,
		unsigned int	numGroups,
		SDurationGroup*	groups,
		double			maxDuration,
		double			minDuration
		);
	~CDuration();

	inline vector<double>*	Durations() { return &this->duration; } 
	inline double			operator[] (unsigned int index) { assert(index < this->duration.size()); return this->duration[index]; }
	inline double			Duration(unsigned int index) { assert(index < this->duration.size()); return this->duration[index]; }
	inline double			MinDuration() { return this->minDuration; }
	inline double			MaxDuration() { return this->maxDuration; }

protected:
	void	GenerateGroup(
		unsigned int	index
		);
};

#endif