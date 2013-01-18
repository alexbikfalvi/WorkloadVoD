#include "stdafx.h"
#include "Arrival.h"
#include "Rand.h"
#include "DistPareto.h"
#include "DistExp.h"
#include "DistNormal.h"

#define ARRIVAL_START_CENTER	43200.0
#define	ARRIVAL_START_MIN		0.0
#define	ARRIVAL_START_MAX		86400.0
#define ARRIVAL_DAY				86400.0

CArrival::CArrival(
	CPopularity*	popularity,
	unsigned char	introDailyDistChoice,
	double			introDailyDistChoiceShape,
	unsigned int	introDayGapMax,
	double			introDailyDistChoiceParetoK,
	double			dailyFileNumParetoK,
	double			dailyFileNumParetoAlpha,
	unsigned int	dailyMaxFileNum,
	double			dailyStartSigma,
	double			dailyGapParetoK,
	double			dailyGapParetoAlpha
	)
{
	assert(popularity);

	this->popularity = popularity;

	this->introDailyDistChoice = introDailyDistChoice;
	this->introDailyDistChoiceShape = introDailyDistChoiceShape;
	this->introDayGapMax = introDayGapMax;
	this->introDailyDistChoiceParetoK = introDailyDistChoiceParetoK;
	this->dailyFileNumParetoK = dailyFileNumParetoK;
	this->dailyFileNumParetoAlpha = dailyFileNumParetoAlpha;
	this->dailyMaxFileNum = dailyMaxFileNum;
	this->dailyStartSigma = dailyStartSigma;
	this->dailyGapParetoK = dailyGapParetoK;
	this->dailyGapParetoAlpha = dailyGapParetoAlpha;

	printf("5. Generating file introduction...");

	// Generate the file introduction days and the number of files
	this->GenerateDailyIntro();

	// Generate first introduction time for a day, and gaps for the rest of introductions in the same day
	this->GenerateIntroTime();

	printf("done.\n");
}

CArrival::~CArrival()
{
	delete[] this->fileIntroGaps;
	
	delete[] this->dailyStartTime;
	delete[] this->dailyGaps;

	delete this->introMapping;
}


void CArrival::GenerateDailyIntro()
{
	// Generate number of files per introduction day and the number
	// of introduction days

	double maxp = DistPareto(this->dailyMaxFileNum, this->dailyFileNumParetoK, this->dailyFileNumParetoAlpha);
	unsigned int numFiles;

	for(unsigned int num = 0; num < this->popularity->NumFiles();)
	{
		numFiles = (unsigned int)floor(DistParetoInv(CRand::Generate(0.0, maxp), this->dailyFileNumParetoK, this->dailyFileNumParetoAlpha));
		numFiles = (num + numFiles > this->popularity->NumFiles())?this->popularity->NumFiles() - num:numFiles;

		num += numFiles;
		assert(numFiles <= this->dailyMaxFileNum);

		this->fileIntroDay.push_back(numFiles);
	}
	assert(this->fileIntroDay.size() > 1);

	// Generate introduction gaps
	this->fileIntroGaps = new unsigned int[this->fileIntroDay.size()-1];
	assert(this->fileIntroGaps);

	switch(this->introDailyDistChoice)
	{
	case 1: // Pareto;
		this->GenerateDailyIntroGapPareto();
		break;
	case 2: // Exponential
		this->GenerateDailyIntroGapExp();
		break;
	case 3: // Fixed
		this->GenerateDailyIntroGapFixed();
		break;
	default: assert(0);
	}
}

void CArrival::GenerateDailyIntroGapPareto()
{
	double maxp = DistPareto(this->introDayGapMax, this->introDailyDistChoiceParetoK, this->introDailyDistChoiceShape);
	for(unsigned int index = 0; index < this->fileIntroDay.size()-1; index++)
	{
		this->fileIntroGaps[index] = (unsigned int)floor(DistParetoInv(CRand::Generate(0.0, maxp), this->introDailyDistChoiceParetoK, this->introDailyDistChoiceShape));
		assert(this->fileIntroGaps[index] <= this->introDayGapMax);
	}
}

void CArrival::GenerateDailyIntroGapExp()
{
	double maxp = DistExp(this->introDayGapMax, this->introDailyDistChoiceShape);
	for(unsigned int index = 0; index < this->fileIntroDay.size()-1; index++)
	{
		this->fileIntroGaps[index] = 1 + (unsigned int)floor(DistExpInv(CRand::Generate(0.0, maxp), this->introDailyDistChoiceShape));
		assert(this->fileIntroGaps[index] <= this->introDayGapMax);
	}
}

void CArrival::GenerateDailyIntroGapFixed()
{
	unsigned int gap = (floor(this->introDailyDistChoiceShape) > this->introDayGapMax)?this->introDayGapMax:(unsigned int)floor(this->introDailyDistChoiceShape);
	gap = (gap == 0)?1:gap;
	for(unsigned int index = 0; index < this->fileIntroDay.size()-1; index++)
		this->fileIntroGaps[index] = gap;
}

void CArrival::GenerateIntroTime()
{
	this->dailyStartTime = new double[this->fileIntroDay.size()];
	assert(this->dailyStartTime);

	this->dailyGaps = new vector<double>[this->fileIntroDay.size()];
	assert(this->dailyGaps);

	for(unsigned int index = 0; index < this->fileIntroDay.size(); index++)
	{
		this->GenerateDailyStartTime(index);
		this->GenerateDailyGaps(index);
	}

	// Generate global introduction time
	unsigned int day = 0;
	for(unsigned int index = 0; index < this->fileIntroDay.size(); index++)
	{
		double time = this->dailyStartTime[index];

		this->introTime.push_back(day * ARRIVAL_DAY + time);
		for(unsigned int gap = 0; gap < this->fileIntroDay[index]-1; gap++)
		{
			time += this->dailyGaps[index][gap];
			this->introTime.push_back(day * ARRIVAL_DAY + time);
		}

		if(index < this->fileIntroDay.size()-1)
			day += this->fileIntroGaps[index];
	}

	assert(this->introTime.size() == this->popularity->NumFiles());

	// Sort global introduction
	sort(this->introTime.begin(), this->introTime.end());

	// Generate file to introduction mapping
	this->introMapping = new CShuffle(this->popularity->NumFiles());
}

void CArrival::GenerateDailyStartTime(unsigned int index)
{
	double minp = DistNormal(ARRIVAL_START_MIN, ARRIVAL_START_CENTER, this->dailyStartSigma);
	double maxp = DistNormal(ARRIVAL_START_MAX, ARRIVAL_START_CENTER, this->dailyStartSigma);

	// Generate normal distributed random variable
	this->dailyStartTime[index] = DistNormalInv(CRand::Generate(minp, maxp), ARRIVAL_START_CENTER, this->dailyStartSigma);

	// Rotate the start time
	this->dailyStartTime[index] = this->dailyStartTime[index] + ARRIVAL_START_CENTER;
	this->dailyStartTime[index] = (this->dailyStartTime[index] >= ARRIVAL_START_MAX)?(this->dailyStartTime[index] - ARRIVAL_START_MAX):this->dailyStartTime[index];
}

void CArrival::GenerateDailyGaps(unsigned int index)
{
	double maxp = DistPareto(ARRIVAL_START_MAX, this->dailyGapParetoK, this->dailyGapParetoAlpha);
	double gap;

	for(unsigned int idx = 0; idx < this->fileIntroDay[index]-1; idx++)
	{
		gap = DistParetoInv(CRand::Generate(0, maxp), this->dailyGapParetoK, this->dailyGapParetoAlpha) - this->dailyGapParetoK;
		this->dailyGaps[index].push_back(gap);
	}
}