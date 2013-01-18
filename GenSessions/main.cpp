#include "stdafx.h"
#include "Workload.h"
#include "Rand.h"

#pragma warning(disable : 4996)

struct SSessionRecord
{
	double			time;
	unsigned int	file;
	double			duration;
	double			start;
	double			prefix;
};

struct SFileRecord
{
	unsigned int	file;
	double			duration;
	unsigned int	freq;
	double			arrival;
	ELifespanClass	lifespan;
	unsigned int	bitrate;
};

int main(int argc, char* argv[])
{
	printf("\nSession generator (v. 1)\n\n");

	if(argc != 5)
	{
		printf("Syntax:\
			   \n\t1 - Config file\
			   \n\t2 - Output sessions file\
			   \n\t3 - Output files file\
			   \n\t4 - Verbose\
 			   \n");
		return 0;
	}


	char* inputFile = argv[1];
	char* outputFileSessions = argv[2];
	char* outputFileFiles = argv[3];
	bool verbose = (atoi(argv[4]) != 0);

	printf("Syntax:\
		   \n\t1 - Config file          : %s\
		   \n\t2 - Output sessions file : %s\
		   \n\t3 - Output files file    : %s\
		   \n\t4 - Verbose              : %u\
		   \n",
		   inputFile,
		   outputFileSessions,
		   outputFileFiles,
		   verbose
		   );

	FILE* in = fopen(inputFile, "r");
	if(NULL == in)
	{
		printf("\nFile \"%s\" not found.", inputFile);
		return 0;
	}

	// Read parameters from input file
	double val;

	// Popularity
	unsigned int	numFiles;
	unsigned int	popMaxFreq;
	double			popZipfAlpha;
	double			popZipfK;

	// Duration
	unsigned int	durNumGroups;
	SDurationGroup*	durGroups;
	unsigned int	durMax;
	unsigned int	durMin;

	// Bitrate
	unsigned int	bitrateNumGroups;
	unsigned int*	bitrateGroups;
	double*			bitrateRatios;

	// Prefix
	double			prefixCompletedMaxRatioZipfMax;
	double			prefixCompletedMaxRatioZipfAlpha;
	double			prefixCompletedMaxRatioMaxCut;
	double			prefixExpRatioLowerBoundRatio;
	double			prefixExpMuNormMu;
	double			prefixExpMuNormSigma;
	double			prefixExpMuCutoff;
	double			prefixExpSessionCutoff;

	// File arrival
	unsigned int	arrivalIntroDailyDistChoice; // 1 - Pareto 2 - exponential 3 - fixed
	double			arrivalIntroDailyDistChoiceShape;
	double			arrivalIntroDailyDistChoiceParetoK;
	unsigned int	arrivalIntroDayGapMax;
	double			arrivalDailyFileNumParetoK;
	double			arrivalDailyFileNumParetoAlpha;
	unsigned int	arrivalDailyMaxFileNum;
	double			arrivalDailyStartSigma;
	double			arrivalIntroTimeGapInADayParetoK;
	double			arrivalIntroTimeGapInADayParetoAlpha;

	// Lifespan
	double			lifespanNewsRatio;
	double			lifespanNewsDist;
	double			lifespanNewsDistAlphaMu;
	double			lifespanNewsDistAlphaSigma;
	double			lifespanNewsDistAlphaMin;
	double			lifespanNewsDistAlphaMax;
	double			lifespanRegularDistMuMu;
	double			lifespanRegularDistMuSigma;
	double			lifespanRegularDistMuMin;
	double			lifespanRegularDistMuMax;
	double			lifespanRegularDistSigmaMu;
	double			lifespanRegularDistSigmaSigma;
	double			lifespanRegularDistSigmaMin;
	double			lifespanRegularDistSigmaMax;

	// Session
	double			sessionMaxLifespan;
	unsigned int	sessionDailyBins;
	double*			sessionDailyBinWidth;
	double*			sessionDailyBinAccessRatio;
	unsigned int	sessionDailyAccessDist;	// 1 - exponential 2 - Pareto
	double			sessionDailyAccessParetoAlpha;

	double			sum = 0;

	if(fscanf(in, "numFiles=%u\n", &numFiles) != 1) { printf("Error reading input file"); return -1; }
	if(fscanf(in, "popMaxFreq=%u\n", &popMaxFreq) != 1) { printf("Error reading input file"); return -1; }
	if(fscanf(in, "popZipfAlpha=%lf\n", &popZipfAlpha) != 1) { printf("Error reading input file"); return -1; }
	if(fscanf(in, "popZipfK=%lf\n", &popZipfK) != 1) { printf("Error reading input file"); return -1; }

	if(fscanf(in, "durNumGroups=%u\n", &durNumGroups) != 1) { printf("Error reading input file"); return -1; }
	if(fscanf(in, "durMax=%u\n", &durMax) != 1) { printf("Error reading input file"); return -1; }
	if(fscanf(in, "durMin=%u\n", &durMin) != 1) { printf("Error reading input file"); return -1; }

	durGroups = new SDurationGroup[durNumGroups];
	assert(durGroups);

	fscanf(in, "durGroupsMu={");
	for(unsigned int index = 0; index < durNumGroups; index++)
	{
		if(index) fscanf(in, ",");
		if(fscanf(in, "%lf", &durGroups[index].mu) != 1) { printf("Error reading input file"); return -1; }
	}
	fscanf(in, "}\n");
	fscanf(in, "durGroupsSigma={");
	for(unsigned int index = 0; index < durNumGroups; index++)
	{
		if(index) fscanf(in, ",");
		if(fscanf(in, "%lf", &durGroups[index].sigma) != 1) { printf("Error reading input file"); return -1; }
	}
	fscanf(in, "}\n");
	fscanf(in, "durGroupsNum={");
	for(unsigned int index = 0; index < durNumGroups; index++)
	{
		if(index) fscanf(in, ",");
		if(fscanf(in, "%lf", &val) != 1) { printf("Error reading input file"); return -1; }
		sum += val;
		durGroups[index].num = (unsigned int)(numFiles*val);
	}
	if(!DOUBLE_EQ(sum,1.0))
	{
		printf("\nError: The sum of \'durGroupsNum\' must be 1.\n");
		return -1;
	}

	fscanf(in, "}\n");

	if(fscanf(in, "bitrateNumGroups=%u\n", &bitrateNumGroups) != 1) { printf("Error reading input file"); return -1; }

	bitrateGroups = new unsigned int[bitrateNumGroups];
	bitrateRatios = new double[bitrateNumGroups];
	assert(bitrateGroups);
	assert(bitrateRatios);

	fscanf(in, "bitrateGroups={");
	for(unsigned int index = 0; index < bitrateNumGroups; index++)
	{
		if(index) fscanf(in, ",");
		if(fscanf(in, "%u", &bitrateGroups[index]) != 1) { printf("Error reading input file"); return -1; }
	}
	fscanf(in, "}\n");
	fscanf(in, "bitrateRatios={");
	for(unsigned int index = 0; index < bitrateNumGroups; index++)
	{
		if(index) fscanf(in, ",");
		if(fscanf(in, "%lf", &bitrateRatios[index]) != 1) { printf("Error reading input file"); return -1; }
	}
	fscanf(in, "}\n");

	if(fscanf(in, "prefixCompletedMaxRatioZipfMax=%lf\n", &prefixCompletedMaxRatioZipfMax) != 1) { printf("Error reading input file"); return -1; }
	if(fscanf(in, "prefixCompletedMaxRatioZipfAlpha=%lf\n", &prefixCompletedMaxRatioZipfAlpha) != 1) { printf("Error reading input file"); return -1; }
	if(fscanf(in, "prefixCompletedMaxRatioMaxCut=%lf\n", &prefixCompletedMaxRatioMaxCut) != 1) { printf("Error reading input file"); return -1; }
	if(fscanf(in, "prefixExpRatioLowerBoundRatio=%lf\n", &prefixExpRatioLowerBoundRatio) != 1) { printf("Error reading input file"); return -1; }
	if(fscanf(in, "prefixExpMuNormMu=%lf\n", &prefixExpMuNormMu) != 1) { printf("Error reading input file"); return -1; }
	if(fscanf(in, "prefixExpMuNormSigma=%lf\n", &prefixExpMuNormSigma) != 1) { printf("Error reading input file"); return -1; }
	if(fscanf(in, "prefixExpMuCutoff=%lf\n", &prefixExpMuCutoff) != 1) { printf("Error reading input file"); return -1; }
	if(fscanf(in, "prefixExpSessionCutoff=%lf\n", &prefixExpSessionCutoff) != 1) { printf("Error reading input file"); return -1; }

	if(fscanf(in, "arrivalIntroDailyDistChoice=%u\n", &arrivalIntroDailyDistChoice) != 1) { printf("Error reading input file"); return -1; }
	if(fscanf(in, "arrivalIntroDailyDistChoiceShape=%lf\n", &arrivalIntroDailyDistChoiceShape) != 1) { printf("Error reading input file"); return -1; }
	if(fscanf(in, "arrivalIntroDailyDistChoiceParetoK=%lf\n", &arrivalIntroDailyDistChoiceParetoK) != 1) { printf("Error reading input file"); return -1; }
	if(fscanf(in, "arrivalIntroDayGapMax=%u\n", &arrivalIntroDayGapMax) != 1) { printf("Error reading input file"); return -1; }
	if(fscanf(in, "arrivalDailyFileNumParetoK=%lf\n", &arrivalDailyFileNumParetoK) != 1) { printf("Error reading input file"); return -1; }
	if(fscanf(in, "arrivalDailyFileNumParetoAlpha=%lf\n", &arrivalDailyFileNumParetoAlpha) != 1) { printf("Error reading input file"); return -1; }
	if(fscanf(in, "arrivalDailyMaxFileNum=%u\n", &arrivalDailyMaxFileNum) != 1) { printf("Error reading input file"); return -1; }
	if(fscanf(in, "arrivalDailyStartSigma=%lf\n", &arrivalDailyStartSigma) != 1) { printf("Error reading input file"); return -1; }
	if(fscanf(in, "arrivalIntroTimeGapInADayParetoK=%lf\n", &arrivalIntroTimeGapInADayParetoK) != 1) { printf("Error reading input file"); return -1; }
	if(fscanf(in, "arrivalIntroTimeGapInADayParetoAlpha=%lf\n", &arrivalIntroTimeGapInADayParetoAlpha) != 1) { printf("Error reading input file"); return -1; }

	if(fscanf(in, "lifespanNewsRatio=%lf\n", &lifespanNewsRatio) != 1) { printf("Error reading input file"); return -1; }
	if(fscanf(in, "lifespanNewsDist=%lf\n", &lifespanNewsDist) != 1) { printf("Error reading input file"); return -1; }
	if(fscanf(in, "lifespanNewsDistAlphaMu=%lf\n", &lifespanNewsDistAlphaMu) != 1) { printf("Error reading input file"); return -1; }
	if(fscanf(in, "lifespanNewsDistAlphaSigma=%lf\n", &lifespanNewsDistAlphaSigma) != 1) { printf("Error reading input file"); return -1; }
	if(fscanf(in, "lifespanNewsDistAlphaMin=%lf\n", &lifespanNewsDistAlphaMin) != 1) { printf("Error reading input file"); return -1; }
	if(fscanf(in, "lifespanNewsDistAlphaMax=%lf\n", &lifespanNewsDistAlphaMax) != 1) { printf("Error reading input file"); return -1; }
	if(fscanf(in, "lifespanRegularDistMuMu=%lf\n", &lifespanRegularDistMuMu) != 1) { printf("Error reading input file"); return -1; }
	if(fscanf(in, "lifespanRegularDistMuSigma=%lf\n", &lifespanRegularDistMuSigma) != 1) { printf("Error reading input file"); return -1; }
	if(fscanf(in, "lifespanRegularDistMuMin=%lf\n", &lifespanRegularDistMuMin) != 1) { printf("Error reading input file"); return -1; }
	if(fscanf(in, "lifespanRegularDistMuMax=%lf\n", &lifespanRegularDistMuMax) != 1) { printf("Error reading input file"); return -1; }
	if(fscanf(in, "lifespanRegularDistSigmaMu=%lf\n", &lifespanRegularDistSigmaMu) != 1) { printf("Error reading input file"); return -1; }
	if(fscanf(in, "lifespanRegularDistSigmaSigma=%lf\n", &lifespanRegularDistSigmaSigma) != 1) { printf("Error reading input file"); return -1; }
	if(fscanf(in, "lifespanRegularDistSigmaMin=%lf\n", &lifespanRegularDistSigmaMin) != 1) { printf("Error reading input file"); return -1; }
	if(fscanf(in, "lifespanRegularDistSigmaMax=%lf\n", &lifespanRegularDistSigmaMax) != 1) { printf("Error reading input file"); return -1; }

	if(fscanf(in, "sessionMaxLifespan=%lf\n", &sessionMaxLifespan) != 1) { printf("Error reading input file"); return -1; }
	if(fscanf(in, "sessionDailyBins=%u\n", &sessionDailyBins) != 1) { printf("Error reading input file"); return -1; }

	sessionDailyBinWidth = new double[sessionDailyBins];
	assert(sessionDailyBinWidth);

	sessionDailyBinAccessRatio = new double[sessionDailyBins];
	assert(sessionDailyBinAccessRatio);

	fscanf(in, "sessionDailyBinWidth={\n");
	sum = 0;
	for(unsigned int index = 0; index < sessionDailyBins; index++)
	{
		if(index) fscanf(in, ",");
		if(fscanf(in, "%lf", &sessionDailyBinWidth[index]) != 1) { printf("Error reading input file"); return -1; }
		sum += sessionDailyBinWidth[index];
	}
	fscanf(in, "}\n");
	if(!DOUBLE_EQ(sum,1.0))
	{
		printf("\nError: The sum of \'sessionDailyBinWidth\' must be 1.\n");
		return -1;
	}

	fscanf(in, "sessionDailyBinAccessRatio={\n");
	sum = 0;
	for(unsigned int index = 0; index < sessionDailyBins; index++)
	{
		if(index) fscanf(in, ",");
		if(fscanf(in, "%lf", &sessionDailyBinAccessRatio[index]) != 1) { printf("Error reading input file"); return -1; }
		sum += sessionDailyBinAccessRatio[index];
	}
	fscanf(in, "}\n");
	if(!DOUBLE_EQ(sum,1.0))
	{
		printf("\nError: The sum of \'sessionDailyBinAccessRatio\' must be 1.\n");
		return -1;
	}

	if(fscanf(in, "sessionDailyAccessDist=%u\n", &sessionDailyAccessDist) != 1) { printf("Error reading input file"); return -1; }
	if(fscanf(in, "sessionDailyAccessParetoAlpha=%lf\n", &sessionDailyAccessParetoAlpha) != 1) { printf("Error reading input file"); return -1; }

	fclose(in);

	// Display parameters

	printf("\nThe parameters are:\n");

	printf("\tnumFiles=%u\n", numFiles);
	printf("\tpopMaxFreq=%u\n", popMaxFreq);
	printf("\tpopZipfAlpha=%lf\n", popZipfAlpha);
	printf("\tpopZipfK=%lf\n", popZipfK);

	printf("\tdurNumGroups=%u\n", durNumGroups);
	printf("\tdurMax=%u\n", durMax);
	printf("\tdurMin=%u\n", durMin);

	printf("\tdurGroupsMu={");
	for(unsigned int index = 0; index < durNumGroups; index++)
	{
		if(index) printf(",");
		printf("%lf", durGroups[index].mu);
	}
	printf("}\n");
	printf("\tdurGroupsSigma={");
	for(unsigned int index = 0; index < durNumGroups; index++)
	{
		if(index) printf(",");
		printf("%lf", durGroups[index].sigma);
	}
	printf("}\n");
	printf("\tdurGroupsNum={");
	for(unsigned int index = 0; index < durNumGroups; index++)
	{
		if(index) printf(",");
		printf("%u", durGroups[index].num);
	}
	printf("}\n");

	printf("\tbitrateNumGroups=%u\n", bitrateNumGroups);
	printf("\tbitrateGroups={");
	for(unsigned int index = 0; index < bitrateNumGroups; index++)
	{
		if(index) printf(",");
		printf("%u", bitrateGroups[index]);
	}
	printf("}\n");
	printf("\tbitrateRatios={");
	for(unsigned int index = 0; index < bitrateNumGroups; index++)
	{
		if(index) printf(",");
		printf("%lf", bitrateRatios[index]);
	}
	printf("}\n");

	printf("\tprefixCompletedMaxRatioZipfMax=%lf\n", prefixCompletedMaxRatioZipfMax);
	printf("\tprefixCompletedMaxRatioZipfAlpha=%lf\n", prefixCompletedMaxRatioZipfAlpha);
	printf("\tprefixCompletedMaxRatioMaxCut=%lf\n", prefixCompletedMaxRatioMaxCut);
	printf("\tprefixExpRatioLowerBoundRatio=%lf\n", prefixExpRatioLowerBoundRatio);
	printf("\tprefixExpMuNormMu=%lf\n", prefixExpMuNormMu);
	printf("\tprefixExpMuNormSigma=%lf\n", prefixExpMuNormSigma);
	printf("\tprefixExpMuCutoff=%lf\n", prefixExpMuCutoff);
	printf("\tprefixExpSessionCutoff=%lf\n", prefixExpSessionCutoff);

	printf("\tarrivalIntroDailyDistChoice=%u\n", arrivalIntroDailyDistChoice);
	printf("\tarrivalIntroDailyDistChoiceShape=%lf\n", arrivalIntroDailyDistChoiceShape);
	printf("\tarrivalIntroDailyDistChoiceParetoK=%lf\n", arrivalIntroDailyDistChoiceParetoK);
	printf("\tarrivalIntroDayGapMax=%u\n", arrivalIntroDayGapMax);
	printf("\tarrivalDailyFileNumParetoK=%lf\n", arrivalDailyFileNumParetoK);
	printf("\tarrivalDailyFileNumParetoAlpha=%lf\n", arrivalDailyFileNumParetoAlpha);
	printf("\tarrivalDailyMaxFileNum=%u\n", arrivalDailyMaxFileNum);
	printf("\tarrivalDailyStartSigma=%lf\n", arrivalDailyStartSigma);
	printf("\tarrivalIntroTimeGapInADayParetoK=%lf\n", arrivalIntroTimeGapInADayParetoK);
	printf("\tarrivalIntroTimeGapInADayParetoAlpha=%lf\n", arrivalIntroTimeGapInADayParetoAlpha);

	printf("\tlifespanNewsRatio=%lf\n", lifespanNewsRatio);
	printf("\tlifespanNewsDist=%lf\n", lifespanNewsDist);
	printf("\tlifespanNewsDistAlphaMu=%lf\n", lifespanNewsDistAlphaMu);
	printf("\tlifespanNewsDistAlphaSigma=%lf\n", lifespanNewsDistAlphaSigma);
	printf("\tlifespanNewsDistAlphaMin=%lf\n", lifespanNewsDistAlphaMin);
	printf("\tlifespanNewsDistAlphaMax=%lf\n", lifespanNewsDistAlphaMax);
	printf("\tlifespanRegularDistMuMu=%lf\n", lifespanRegularDistMuMu);
	printf("\tlifespanRegularDistMuSigma=%lf\n", lifespanRegularDistMuSigma);
	printf("\tlifespanRegularDistMuMin=%lf\n", lifespanRegularDistMuMin);
	printf("\tlifespanRegularDistMuMax=%lf\n", lifespanRegularDistMuMax);
	printf("\tlifespanRegularDistSigmaMu=%lf\n", lifespanRegularDistSigmaMu);
	printf("\tlifespanRegularDistSigmaSigma=%lf\n", lifespanRegularDistSigmaSigma);
	printf("\tlifespanRegularDistSigmaMin=%lf\n", lifespanRegularDistSigmaMin);
	printf("\tlifespanRegularDistSigmaMax=%lf\n", lifespanRegularDistSigmaMax);

	printf("\tsessionMaxLifespan=%lf\n", sessionMaxLifespan);
	printf("\tsessionDailyBins=%u\n", sessionDailyBins);

	printf("\tsessionDailyBinWidth={\n\t\t");
	for(unsigned int index = 0; index < sessionDailyBins; index++)
	{
		if(index) printf(",");
		if(index && (index%10 == 0)) printf("\n\t\t");
		printf("%lf", sessionDailyBinWidth[index]);
	}
	printf("\n\t\t}\n");

	printf("\tsessionDailyBinAccessRatio={\n\t\t");
	for(unsigned int index = 0; index < sessionDailyBins; index++)
	{
		if(index) printf(",");
		if(index && (index%10 == 0)) printf("\n\t\t");
		printf("%lf", sessionDailyBinAccessRatio[index]);
	}
	printf("\n\t\t}\n");

	printf("\tsessionDailyAccessDist=%u\n", sessionDailyAccessDist);
	printf("\tsessionDailyAccessParetoAlpha=%lf\n", sessionDailyAccessParetoAlpha);

	printf("\n\n");

	CRand::Init();

	CWorkload* workload =  new CWorkload(
		numFiles,
		popMaxFreq,
		popZipfAlpha,
		popZipfK,
		durNumGroups,
		durGroups,
		durMax,
		durMin,
		bitrateNumGroups,
		bitrateGroups,
		bitrateRatios,
		prefixCompletedMaxRatioZipfMax,
		prefixCompletedMaxRatioZipfAlpha,
		prefixCompletedMaxRatioMaxCut,
		prefixExpRatioLowerBoundRatio,
		prefixExpMuNormMu,
		prefixExpMuNormSigma,
		prefixExpMuCutoff,
		prefixExpSessionCutoff,
		(unsigned char)arrivalIntroDailyDistChoice,
		arrivalIntroDailyDistChoiceShape,
		arrivalIntroDailyDistChoiceParetoK,
		arrivalIntroDayGapMax,
		arrivalDailyFileNumParetoK,
		arrivalDailyFileNumParetoAlpha,
		arrivalDailyMaxFileNum,
		arrivalDailyStartSigma,
		arrivalIntroTimeGapInADayParetoK,
		arrivalIntroTimeGapInADayParetoAlpha,
		lifespanNewsRatio,
		lifespanNewsDist,
		lifespanNewsDistAlphaMu,
		lifespanNewsDistAlphaSigma,
		lifespanNewsDistAlphaMin,
		lifespanNewsDistAlphaMax,
		lifespanRegularDistMuMu,
		lifespanRegularDistMuSigma,
		lifespanRegularDistMuMin,
		lifespanRegularDistMuMax,
		lifespanRegularDistSigmaMu,
		lifespanRegularDistSigmaSigma,
		lifespanRegularDistSigmaMin,
		lifespanRegularDistSigmaMax,
		sessionMaxLifespan,
		sessionDailyBins,
		sessionDailyBinWidth,
		sessionDailyBinAccessRatio,
		sessionDailyAccessDist,
		sessionDailyAccessParetoAlpha
		);

	FILE* out;

	if(verbose)
	{
		out = fopen("out.txt", "w");
		assert(out);

		for(unsigned int index = 0; index < numFiles; index++)
			fprintf(out, "%u\t%lf\t%lf\t%lf\t%lf\t%lf\n",
			(*workload->Popularity())[index],
			(*workload->Duration())[index],
			workload->Prefix()->ExpRatio(index),
			workload->Prefix()->ExpMu(index),
			workload->Prefix()->CompletedRatio(index),
			(*workload->Arrival())[index]
			);

		fclose(out);

		out = fopen("file_intro_day.txt", "w");
		assert(out);

		for(unsigned int index = 0; index < workload->Arrival()->NumIntroDays(); index++)
			fprintf(out, "%u\n", workload->Arrival()->FileIntroDay(index));

		fclose(out);

		out = fopen("file_intro_gaps.txt", "w");
		assert(out);

		for(unsigned int index = 0; index < workload->Arrival()->NumIntroDays()-1; index++)
			fprintf(out, "%u\n", workload->Arrival()->FileIntroGaps(index));

		fclose(out);

		out = fopen("daily_start_time.txt", "w");
		assert(out);

		for(unsigned int index = 0; index < workload->Arrival()->NumIntroDays(); index++)
			fprintf(out, "%lf\n", workload->Arrival()->DailyStartTime(index));

		fclose(out);

		out = fopen("daily_gap.txt", "w");
		assert(out);

		for(unsigned int day = 0; day < workload->Arrival()->NumIntroDays(); day++)
		{
			for(unsigned int gap = 0; gap < workload->Arrival()->FileIntroDay(day)-1; gap++)
				fprintf(out, "%lf ", workload->Arrival()->DailyGap(day, gap));
			fprintf(out, "\n");
		}

		fclose(out);

		out = fopen("class_news.txt", "w");
		assert(out);

		for(unsigned int index = 0; index < workload->Lifespan()->NumNews(); index++)
			fprintf(out, "%u\n", workload->Lifespan()->ClsNews(index));

		fclose(out);

		out = fopen("class_regular.txt", "w");
		assert(out);

		for(unsigned int index = 0; index < workload->Lifespan()->NumRegular(); index++)
			fprintf(out, "%u\n", workload->Lifespan()->ClsRegular(index));

		fclose(out);

		out = fopen("dist_news_alpha.txt", "w");
		assert(out);

		for(unsigned int index = 0; index < workload->Lifespan()->NumNews(); index++)
			fprintf(out, "%lf\n", workload->Lifespan()->DistNewsAlpha(index));

		fclose(out);

		out = fopen("dist_regular_mu.txt", "w");
		assert(out);

		for(unsigned int index = 0; index < workload->Lifespan()->NumRegular(); index++)
			fprintf(out, "%lf\n", workload->Lifespan()->DistRegularMu(index));

		fclose(out);

		out = fopen("dist_regular_sigma.txt", "w");
		assert(out);

		for(unsigned int index = 0; index < workload->Lifespan()->NumRegular(); index++)
			fprintf(out, "%lf\n", workload->Lifespan()->DistRegularSigma(index));

		fclose(out);

		out = fopen("file_access.txt", "w");
		assert(out);

		for(unsigned int index = 0; index < workload->Popularity()->NumFiles(); index++)
		{
			unsigned int idx = 0;
			map<unsigned int, unsigned int>* access = workload->Session()->FileAccess(index);
			for(map<unsigned int, unsigned int>::iterator iter = access->begin(); iter != access->end(); iter++)
				for(unsigned int idx1 = 0; idx1 < iter->second; idx1++, idx++)
					fprintf(out, "%u ", iter->first);
			for(; idx < (*workload->Popularity())[0]; idx++)
				fprintf(out, "-1 ");
			fprintf(out, "\n");
		}

		fclose(out);

		out = fopen("file_session.txt", "w");
		assert(out);

		for(unsigned int index = 0; index < workload->Popularity()->NumFiles(); index++)
		{
			unsigned int idx = 0;
			list<double>* sess = workload->Session()->FileSession(index);
			for(list<double>::iterator iter = sess->begin(); iter != sess->end(); iter++, idx++)
				fprintf(out, "%lf ", *iter);
			for(; idx < (*workload->Popularity())[0]; idx++)
				fprintf(out, "-1 ");
			fprintf(out, "\n");
		}

		fclose(out);

		out = fopen("file_prefix.txt", "w");
		assert(out);

		for(unsigned int index = 0; index < workload->Popularity()->NumFiles(); index++)
		{
			unsigned int idx = 0;
			for(; idx < (*workload->Popularity())[index]; idx++)
				fprintf(out, "%lf ", workload->Session()->FilePrefix(index, idx));
			for(; idx < (*workload->Popularity())[0]; idx++)
				fprintf(out, "-1 ");
			fprintf(out, "\n");
		}

		fclose(out);

		out = fopen("file_workload.txt", "w");
		assert(out);

		{
			multimap<double, SSession>* sess = workload->Session()->Sessions();

			for(multimap<double, SSession>::iterator iter = sess->begin(); iter != sess->end(); iter++)
				fprintf(out, "%lf %u %lf %lf\n", iter->first, iter->second.file, (*workload->Duration())[iter->second.file], iter->second.prefix);
		}

		fclose(out);
	}

	out = fopen(outputFileSessions, "wb");
	assert(out);

	{
		multimap<double, SSession>* sess = workload->Session()->Sessions();

		unsigned int numSessions = sess->size();
		assert(numSessions == workload->Popularity()->TotalFreq());

		fwrite(&numSessions, sizeof(numSessions), 1, out);

		SSessionRecord rec;

		for(multimap<double, SSession>::iterator iter = sess->begin(); iter != sess->end(); iter++)
		{
			rec.time = iter->first;
			rec.file = iter->second.file;
			rec.duration = (*workload->Duration())[iter->second.file];
			rec.start = 0;
			rec.prefix = iter->second.prefix;

			fwrite(&rec, sizeof(rec), 1, out);
		}
	}

	fclose(out);

	out = fopen(outputFileFiles, "wb");

	{
		unsigned int numFiles = workload->Popularity()->NumFiles();

		fwrite(&numFiles, sizeof(numFiles), 1, out);

		SFileRecord rec;

		for(unsigned int index = 0; index < workload->Popularity()->NumFiles(); index++)
		{
			rec.file = index;
			rec.freq = (*workload->Popularity())[index];
			rec.duration = (*workload->Duration())[index];
			rec.arrival = (*workload->Arrival())[index];
			rec.lifespan = workload->Lifespan()->Class(index);
			rec.bitrate = (*workload->Bitrate())[index];

			fwrite(&rec, sizeof(rec), 1, out);
		}
	}

	fclose(out);

	printf("\nFinished : %u sessions", workload->Popularity()->TotalFreq());

	delete workload;

	delete[] durGroups;
	delete[] bitrateGroups;
	delete[] bitrateRatios;
	delete[] sessionDailyBinWidth;
	delete[] sessionDailyBinAccessRatio;

#ifdef WIN32
	_CrtDumpMemoryLeaks();
#endif

	return 0;
}
