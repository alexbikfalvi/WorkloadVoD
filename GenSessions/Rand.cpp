#include "stdafx.h"
#include "Rand.h"

void CRand::Init()
{
#ifndef WIN32
	srand((unsigned int)(time(0) & 0xFFFFFFFF));
#endif
}

double CRand::Generate()
{
#ifdef WIN32
	unsigned int rvalue;
	rand_s(&rvalue);
	return ((double)rvalue) / UINT_MAX;
#else
	return ((double)rand()) / RAND_MAX;
#endif
}

double CRand::Generate(double min, double max)
{
#ifdef WIN32
	unsigned int rvalue;
	rand_s(&rvalue);
	return min + (((double)rvalue) / UINT_MAX) * (max-min);
#else
	return min + (((double)rand()) / RAND_MAX) * (max-min);
#endif
}


unsigned int CRand::Generate(unsigned int max)
{
#ifdef WIN32
	unsigned int rvalue;
	rand_s(&rvalue);
	return (unsigned int)(max * (((double)rvalue) / UINT_MAX));
#else
	return (unsigned int)floor(((double)rand())*max/((double)RAND_MAX+1));
#endif
}
