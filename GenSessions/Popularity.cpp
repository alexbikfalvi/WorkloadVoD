#include "stdafx.h"
#include "Popularity.h"

CPopularity::CPopularity(
	unsigned int	numFiles,
	unsigned int	maxFreq,
	double			zipfAlpha,
	double			zipfK
	)
{
	assert(numFiles);
	assert(zipfK);

	printf("1. Generating file popularity...");

	this->numFiles = numFiles;

	this->freq = new unsigned int[numFiles];
	assert(this->freq);

	this->freq[0] = maxFreq;
	this->totalFreq = this->freq[0];
	
	for(unsigned int index = 1; index < numFiles; index++)
	{
		this->freq[index] = (unsigned int)floor(((maxFreq/zipfK + 1)/pow(index/zipfK + 1, zipfAlpha) - 1) * zipfK + 1);
		this->totalFreq += this->freq[index];
	}

	printf("done.\n");
}

CPopularity::~CPopularity()
{
	delete[] this->freq;
}
