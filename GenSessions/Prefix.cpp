#include "stdafx.h"
#include "Prefix.h"
#include "Rand.h"
#include "DistNormal.h"

CPrefix::CPrefix(
	CPopularity*	popularity,
	CDuration*		duration,
	double			completedMaxRatioZipfMax,
	double			completedMaxRatioZipfAlpha,
	double			completedMaxRatioMaxCut,
	double			expRatioLowerBoundRatio,
	double			expMuNormMu,
	double			expMuNormSigma,
	double			expMuCutoff,
	double			expSessionCutoff
	)
{
	assert(popularity);
	assert(duration);

	this->popularity = popularity;
	this->duration = duration;

	this->completedMaxRatioZipfMax = completedMaxRatioZipfMax;
	this->completedMaxRatioZipfAlpha = completedMaxRatioZipfAlpha;
	this->completedMaxRatioMaxCut = completedMaxRatioMaxCut;

	this->expRatioLowerBoundRatio = expRatioLowerBoundRatio;
	this->expMuNormMu = expMuNormMu;
	this->expMuNormSigma = expMuNormSigma;
	this->expMuCutoff = expMuCutoff;
	this->expSessionCutoff = expSessionCutoff;

	this->completedRatio = new double[this->popularity->NumFiles()];
	assert(this->completedRatio);

	this->expRatio = new double[this->popularity->NumFiles()];
	assert(this->expRatio);

	this->expMu = new double[this->popularity->NumFiles()];
	assert(this->expMu);

	this->numBins = (unsigned int)ceil((double)this->duration->MaxDuration()/60.0);

	printf("4. Generating file prefix distribution...");

	// Generate completed ratio
	this->GenerateCompletedRatio();

	// Generate exponential ratio
	this->GenerateExpRatio();

	// Generate exponential mean
	this->GenerateExpMu();

	printf("done.\n");
}

CPrefix::~CPrefix()
{
	delete[] this->expRatio;
	delete[] this->expMu;
	delete[] this->completedRatio;
}

void CPrefix::GenerateCompletedRatio()
{
	// Calculate maximum ratio for each one-minute bin
	double*	maxRatio = new double[this->numBins];
	for(unsigned int index = 0; index < this->numBins; index++)
	{
		maxRatio[index] = this->completedMaxRatioZipfMax / pow(index+1, this->completedMaxRatioZipfAlpha);
		maxRatio[index] = (maxRatio[index] > this->completedMaxRatioMaxCut)? this->completedMaxRatioMaxCut : maxRatio[index];
	}

	// Map files to one-minute bins according to their duration
	list<unsigned int>* bins = new list<unsigned int>[this->numBins];
	for(unsigned int index = 0; index < this->popularity->NumFiles(); index++)
		bins[(unsigned int)floor((*this->duration)[index] / 60)].push_back(index);

	// For each file generate the complete ratio
	for(unsigned int index = 0; index < this->numBins; index++)
		for(list<unsigned int>::iterator iter = bins[index].begin(); iter != bins[index].end(); iter++)
			this->completedRatio[(*iter)] = CRand::Generate() * maxRatio[index];
	
	delete[] bins;
	delete[] maxRatio;
}

void CPrefix::GenerateExpRatio()
{
	double expRatioLower;
	double expRatioUpper;
	for(unsigned int index = 0; index < this->popularity->NumFiles(); index++)
	{
		if((*this->duration)[index] < this->expSessionCutoff)
			this->expRatio[index] = 1 - this->completedRatio[index];
		else
		{
			expRatioUpper = 1 - this->completedRatio[index];
			expRatioLower = expRatioUpper * (this->expRatioLowerBoundRatio + (1-this->expRatioLowerBoundRatio)*this->completedRatio[index]/this->completedMaxRatioMaxCut);

			this->expRatio[index] = CRand::Generate() * (expRatioUpper - expRatioLower) + expRatioLower;
		}
	}
}

void CPrefix::GenerateExpMu()
{
	// Generate the mean for the series of exponential distributions
	double minp = DistNormal(0, this->expMuNormMu, this->expMuNormSigma);
	double maxp = DistNormal(this->expMuCutoff, this->expMuNormMu, this->expMuNormSigma);
	
	for(unsigned int index = 0; index < this->popularity->NumFiles(); index++)
	{
		double p = CRand::Generate(minp, maxp);
		this->expMu[index] = DistNormalInv(p, this->expMuNormMu, this->expMuNormSigma);
		assert((this->expMu[index] >= 0) && (this->expMu[index] <= this->expMuCutoff));
	}
}
