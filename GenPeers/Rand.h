#ifndef __Rand_h__
#define __Rand_h__

class CRand
{
public:
	static double				Generate();
	static double				Generate(double min, double max);
	static unsigned int			Generate(unsigned int max);
};

#endif