#include "stdafx.h"
#include "Session.h"
#include "Rand.h"
#include "DistPareto.h"
#include "DistLogn.h"
#include "DistExp.h"
#include "Shuffle.h"

CSession::CSession(
	CPopularity*	popularity,
	CDuration*		duration,
	CPrefix*		prefix,
	CArrival*		arrival,
	CLifespan*		lifespan,
	double			maxLifespan,
	unsigned int	dailyBins,
	double*			dailyBinWidthRatio,
	double*			dailyBinAccessRatio,
	unsigned int	dailyAccessDist,
	double			dailyAccessParetoAlpha
	)
{
	assert(popularity);
	assert(duration);
	assert(prefix);
	assert(arrival);
	assert(lifespan);

	assert(dailyBins);
	assert(dailyBinWidthRatio);
	assert(dailyBinAccessRatio);

	this->popularity = popularity;
	this->duration = duration;
	this->prefix = prefix;
	this->arrival = arrival;
	this->lifespan = lifespan;

	this->maxLifespan = maxLifespan;
	this->dailyBins = dailyBins;
	this->dailyBinWidthRatio = dailyBinWidthRatio;
	this->dailyBinAccessRatio = dailyBinAccessRatio;
	this->dailyAccessDist = dailyAccessDist;
	this->dailyAccessParetoAlpha = dailyAccessParetoAlpha;

	this->Init();

	this->fileAccess = new map<unsigned int, unsigned int>[this->popularity->NumFiles()];
	assert(this->fileAccess);

	this->fileSession = new list<double>[this->popularity->NumFiles()];
	assert(this->fileSession);

	this->filePrefix = new double*[this->popularity->NumFiles()];
	assert(this->filePrefix);

	printf("7. Generating file sessions");
	int perc = -1;
	for(unsigned int index = 0; index < this->popularity->NumFiles(); index++)
	{
		if((int)(100*(index+1)/this->popularity->NumFiles()) != perc)
		{
			perc = 100*(index+1)/this->popularity->NumFiles();
			printf(".");
		}
		
		// Generate individual file access
		this->GenerateFileLifespan(index);

		// Generate prefix
		this->GenerateFilePrefix(index);

		// Generate sessions
		this->GenerateFileSessions(index);
	}
	printf("done.\n");
}

CSession::~CSession()
{
	delete[] this->fileAccess;
	delete[] this->fileSession;
	for(unsigned int index = 0; index < this->popularity->NumFiles(); index++)
		delete[] this->filePrefix[index];
	delete[] this->filePrefix;

	delete[] this->dailyBinStart;
	delete[] this->dailyBinWidth;
}

void CSession::Init()
{
	this->dailyBinStart = new double[this->dailyBins];
	this->dailyBinWidth = new double[this->dailyBins];

	assert(this->dailyBinStart);
	assert(this->dailyBinWidth);

	this->dailyBinStart[0] = 0;

	for(unsigned int index = 0; index < this->dailyBins-1; index++)
	{
		this->dailyBinWidth[index] = 86400.0 * this->dailyBinWidthRatio[index];
		this->dailyBinStart[index+1] = this->dailyBinStart[index] + this->dailyBinWidth[index];
	}

	this->dailyBinWidth[this->dailyBins-1] = 86400.0 - this->dailyBinStart[this->dailyBins-1];
}

void CSession::GenerateFileLifespan(unsigned int index)
{
	SSession session;

	session.file = index;
	session.prefix = 0;

	double arrivalDaySec = (*this->arrival)[index] - 86400*floor((*this->arrival)[index]/86400);
	unsigned int arrivalDay = (unsigned int)floor((*this->arrival)[index]/86400);

	// Generate day access
	switch(this->lifespan->Class(index))
	{
	case LSC_PARETO:
		{
			// Generate Pareto access days
			double maxp = DistPareto(this->maxLifespan, this->lifespan->ParetoK(index), this->lifespan->ParetoAlpha(index));
			unsigned int day;

			for(unsigned int idx = 0; idx < (*this->popularity)[index]; idx++)
			{
				day = (unsigned int) floor(DistParetoInv(CRand::Generate(0.0, maxp), this->lifespan->ParetoK(index), this->lifespan->ParetoAlpha(index)));
				assert(day <= this->maxLifespan);
				this->IncrementFileAccess(index, day);
			}
		}
		break;
	case LSC_LOGN:
		{
			// Generate lognormal access days
			double maxp = DistLogn(this->maxLifespan, this->lifespan->LognMu(index), this->lifespan->LognSigma(index));
			unsigned int day;

			for(unsigned int idx = 0; idx < (*this->popularity)[index]; idx++)
			{
				day = (unsigned int) ceil(DistLognInv(CRand::Generate(0.0, maxp), this->lifespan->LognMu(index), this->lifespan->LognSigma(index)));
				assert(day <= this->maxLifespan);
				this->IncrementFileAccess(index, day);
			}
		}
		break;
	}

	// Generate access within a day
		// Calculate number of accesses in each daily bin
	unsigned int* dailyBinNum = new unsigned int[this->dailyBins];
	assert(dailyBinNum);

		// For each distinct day
	for(map<unsigned int, unsigned int>::iterator iter = this->fileAccess[index].begin(); iter != this->fileAccess[index].end(); iter++)
	{
		unsigned int day = iter->first;
		unsigned int accesses = iter->second;

		// Clear the daily bin
		for(unsigned int idx1 = 0; idx1 < this->dailyBins; idx1++)
			dailyBinNum[idx1] = 0;

		// Calculate the number of accesses in each bin
		for(unsigned int idx1 = 0; idx1 < accesses; idx1++)
		{
			double rnd = CRand::Generate();

			for(unsigned int idx2 = 0; idx2 < this->dailyBins; idx2++)
			{
				if(DOUBLE_LT(rnd, this->dailyBinAccessRatio[idx2]))
				{
					dailyBinNum[idx2]++;
					break;
				}
				rnd -= this->dailyBinAccessRatio[idx2];
			}
		}

		// Generate accesses in each bin
		for(unsigned int bin = 0; bin < this->dailyBins; bin++)
		{
			if(dailyBinNum[bin] > 0)
			{
				// Calculate the bin gaps
				double* gaps = new double[dailyBinNum[bin]];

				// Bin gaps (including before and after the last access in the bin)
				double* relGaps = new double[dailyBinNum[bin]+1];
				double gapSum = 0;

				switch(this->dailyAccessDist)
				{
				case 1:
					{
						// Exponential access in a bin

						// Mean and maximum
						double mu = 1.0/dailyBinNum[bin];
						double max = 4*mu;

						// Maximum probility
						double maxp = DistExp(max, mu);

						// Generate bin gaps (divide by 2 the first and last gap)
						relGaps[0] = DistExpInv(CRand::Generate(0.0, maxp), mu) / 2;
						assert(relGaps[0] <= max);
						gapSum = relGaps[0];

						relGaps[dailyBinNum[bin]] = DistExpInv(CRand::Generate(0.0, maxp), mu) / 2;
						assert(relGaps[dailyBinNum[bin]] <= max);
						gapSum += relGaps[dailyBinNum[bin]];

						for(unsigned int num = 1; num < dailyBinNum[bin]; num++)
						{
							relGaps[num] = DistExpInv(CRand::Generate(0.0, maxp), mu);
							assert(relGaps[num] <= max);
							gapSum += relGaps[num];
						}
					}
					break;
				case 2:
					{
						// Pareto access in a bin

						// Mean and maximum
						double mu = this->dailyBinWidth[bin]/dailyBinNum[bin];
						double max = 4*mu;

						// Maximum probability
						double maxp = DistPareto(max, 1.0, this->dailyAccessParetoAlpha);

						// Generate bin gaps (divide by 2 the first and last gap)
						relGaps[0] = DistParetoInv(CRand::Generate(0.0, maxp), 1.0, this->dailyAccessParetoAlpha) / 2;
						assert(relGaps[0] <= max);
						gapSum = relGaps[0];

						relGaps[dailyBinNum[bin]] = DistParetoInv(CRand::Generate(0.0, maxp), 1.0, this->dailyAccessParetoAlpha) / 2;
						assert(relGaps[dailyBinNum[bin]] <= max);
						gapSum += relGaps[dailyBinNum[bin]];

						for(unsigned int num = 1; num < dailyBinNum[bin]; num++)
						{
							relGaps[num] = DistParetoInv(CRand::Generate(0.0, maxp), 1.0, this->dailyAccessParetoAlpha);
							assert(relGaps[num] <= max);
							gapSum += relGaps[num];
						}
					}
					break;
				}

				// Normalize the gaps by their sum (they must fit in the bin width)
				for(unsigned int num = 0; num < dailyBinNum[bin]; num++)
					gaps[num] = this->dailyBinWidth[bin]*relGaps[num]/gapSum;

				delete[] relGaps;

				// Calculate the start time for this file/day/bin
				double time =
					86400 * arrivalDay +		// Arrival day
					86400 * (day-1) +			// Day start
					this->dailyBinStart[bin] +	// Bin start
					gaps[0];					// Gap start

				// If the start time within the day is smaller than the daily introduction time, move it to the next day
				if(this->dailyBinStart[bin] + gaps[0] < arrivalDaySec) time += 86400;

				this->fileSession[index].push_back(time);
				for(unsigned int num = 1; num < dailyBinNum[bin]; num++)
				{
					time += gaps[num];
					this->fileSession[index].push_back(time);
				}

				delete[] gaps;
			}
		}
	}
	delete[] dailyBinNum;

	assert(this->fileSession[index].size() == (*this->popularity)[index]);
}

void CSession::IncrementFileAccess(unsigned int file, unsigned int day)
{
	map<unsigned int, unsigned int>::iterator iter = this->fileAccess[file].find(day);
	if(iter != this->fileAccess[file].end())
		iter->second++;
	else
		this->fileAccess[file].insert(pair<unsigned int, unsigned int>(day, 1));
}

void CSession::GenerateFilePrefix(unsigned int index)
{
	double ratio[3];
	unsigned int num[3] = {0, 0, 0};

	// Calculate the prefix ratio
	ratio[0] = this->prefix->ExpRatio(index);
	ratio[2] = this->prefix->CompletedRatio(index);
	ratio[1] = 1.0 - ratio[0] - ratio[2];

	// Calculate the number of sessions for each prefix type
	for(unsigned int idx = 0; idx < (*this->popularity)[index]; idx++)
	{
		double rnd = CRand::Generate();

		if(rnd <= ratio[0]) num[0]++;
		else if(rnd <= ratio[0] + ratio[1]) num[1]++;
		else num[2]++;
	}

	assert(num[0]+num[1]+num[2] == (*this->popularity)[index]);

	// Generate a prefix for each session
	this->filePrefix[index] = new double[(*this->popularity)[index]];
	double prefixCutoff = (this->prefix->ExpSessionCutoff() < (*this->duration)[index])?this->prefix->ExpSessionCutoff():(*this->duration)[index];

	unsigned int idx = 0;

		// Generate exponential distribution prefix
	double minp = exp((1.0-prefixCutoff)/this->prefix->ExpMu(index));
	for(; idx < num[0]; idx++)
	{
		this->filePrefix[index][idx] = 1.0 - this->prefix->ExpMu(index)*log(CRand::Generate(minp, 1.0));
		assert(this->filePrefix[index][idx] <= prefixCutoff);
	}

		// Generate uniform distribution prefix
	for(; idx < num[0]+num[1]; idx++)
	{
		this->filePrefix[index][idx] = CRand::Generate() * prefixCutoff;
		assert(this->filePrefix[index][idx] <= prefixCutoff);
	}

		// Generate complete prefix
	for(; idx < (*this->popularity)[index]; idx++)
		this->filePrefix[index][idx] = (*this->duration)[index];
}

void CSession::GenerateFileSessions(unsigned int index)
{
	assert(this->fileSession[index].size() == (*this->popularity)[index]);

	CShuffle shuffle((*this->popularity)[index]);

	unsigned int idx = 0;
	for(list<double>::iterator iter = this->fileSession[index].begin(); iter != this->fileSession[index].end(); iter++, idx++)
	{
		SSession session;

		session.file = index;
		session.prefix = this->filePrefix[index][shuffle[idx]];

		this->sessions.insert(pair<double, SSession>(*iter, session));
	}
}
