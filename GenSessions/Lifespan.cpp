#include "stdafx.h"
#include "Lifespan.h"
#include "Rand.h"
#include "DistNormal.h"

CLifespan::CLifespan(
	CPopularity*	popularity,
	double			newsRatio,
	double			newsDist,
	double			newsDistAlphaMu,
	double			newsDistAlphaSigma,
	double			newsDistAlphaMin,
	double			newsDistAlphaMax,
	double			regularDistMuMu,
	double			regularDistMuSigma,
	double			regularDistMuMin,
	double			regularDistMuMax,
	double			regularDistSigmaMu,
	double			regularDistSigmaSigma,
	double			regularDistSigmaMin,
	double			regularDistSigmaMax
	)
{
	assert(popularity);

	this->popularity = popularity;

	this->newsRatio = newsRatio;
	this->newsDist = newsDist;
	this->newsDistAlphaMu = newsDistAlphaMu;
	this->newsDistAlphaSigma = newsDistAlphaSigma;
	this->newsDistAlphaMin = newsDistAlphaMin;
	this->newsDistAlphaMax = newsDistAlphaMax;
	this->regularDistMuMu = regularDistMuMu;
	this->regularDistMuSigma = regularDistMuSigma;
	this->regularDistMuMin = regularDistMuMin;
	this->regularDistMuMax = regularDistMuMax;
	this->regularDistSigmaMu = regularDistSigmaMu;
	this->regularDistSigmaSigma = regularDistSigmaSigma;
	this->regularDistSigmaMin = regularDistSigmaMin;
	this->regularDistSigmaMax = regularDistSigmaMax;

	this->cls = new ELifespanClass[this->popularity->NumFiles()];
	this->param1 = new double[this->popularity->NumFiles()];
	this->param2 = new double[this->popularity->NumFiles()];
	
	assert(this->cls);
	assert(this->param1);
	assert(this->param2);

	printf("6. Generating file lifespan...");

	// Generate file class
	this->GenerateClass();

	// Generate file parameters
	this->GenerateParameters();

	printf("done.\n");
}

CLifespan::~CLifespan()
{
	delete[] this->cls;
	delete[] this->param1;
	delete[] this->param2;
}

void CLifespan::GenerateClass()
{
	// Assign lognormal class to all files
	for(unsigned int index = 0; index < this->popularity->NumFiles(); index++)
		this->cls[index] = LSC_LOGN;

	this->numNews = (unsigned int)floor(this->popularity->NumFiles() * this->newsRatio);
	this->numRegular = this->popularity->NumFiles() - this->numNews;

	// Assign Pareto class to news files selected from all files using an exponential distribution
	for(unsigned int index = 0; index < this->numNews;)
	{
		unsigned int rank = (unsigned int)floor(-this->newsDist*log(CRand::Generate())*this->popularity->NumFiles());
		
		if(rank < this->popularity->NumFiles())
		{
			if(this->cls[rank] != LSC_PARETO)
			{
				this->cls[rank] = LSC_PARETO;
				this->clsNews.push_back(rank);
				index++;
			}
		}
	}

	for(unsigned int index = 0; index < this->popularity->NumFiles(); index++)
		if(this->cls[index] == LSC_LOGN)
			this->clsRegular.push_back(index);
}

void CLifespan::GenerateParameters()
{
	double minpMu = DistNormal(this->regularDistMuMin, this->regularDistMuMu, this->regularDistMuSigma);
	double maxpMu = DistNormal(this->regularDistMuMax, this->regularDistMuMu, this->regularDistMuSigma);
	double minpSigma = DistNormal(this->regularDistSigmaMin, this->regularDistSigmaMu, this->regularDistSigmaSigma);
	double maxpSigma = DistNormal(this->regularDistSigmaMax, this->regularDistSigmaMu, this->regularDistSigmaSigma);
	double minpAlpha = DistNormal(this->newsDistAlphaMin, this->newsDistAlphaMu, this->newsDistAlphaSigma);
	double maxpAlpha = DistNormal(this->newsDistAlphaMax, this->newsDistAlphaMu, this->newsDistAlphaSigma);

	for(unsigned int index = 0; index < this->popularity->NumFiles(); index++)
	{
		switch(this->cls[index])
		{
		case LSC_LOGN:
			// Lognormal lifespan
				// Mu
			this->param1[index] = DistNormalInv(CRand::Generate(minpMu, maxpMu), this->regularDistMuMu, this->regularDistMuSigma);
				// Sigma
			this->param2[index] = DistNormalInv(CRand::Generate(minpSigma, maxpSigma), this->regularDistSigmaMu, this->regularDistSigmaSigma);

			this->distRegularMu.push_back(this->param1[index]);
			this->distRegularSigma.push_back(this->param2[index]);
			break;
		case LSC_PARETO:
			// Pareto lifespan
				// K
			this->param1[index] = 1.0;
				// Alpha
			this->param2[index] = DistNormalInv(CRand::Generate(minpAlpha, maxpAlpha), this->newsDistAlphaMu, this->newsDistAlphaSigma);

			this->distNewsAlpha.push_back(this->param2[index]);
			break;
		}
	}

	assert(this->distNewsAlpha.size() == this->numNews);
	assert(this->distRegularMu.size() == this->numRegular);
	assert(this->distRegularSigma.size() == this->numRegular);
}

