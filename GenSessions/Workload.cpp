#include "stdafx.h"
#include "Workload.h"

CWorkload::CWorkload(
	unsigned int	numFiles,
	unsigned int	popMaxFreq,
	double			popZipfAlpha,
	double			popZipfK,
	unsigned int	durNumGroups,
	SDurationGroup*	durGroups,
	unsigned int	durMax,
	unsigned int	durMin,
	unsigned int	bitrateNumGroups,
	unsigned int*	bitrateGroups,
	double*			bitrateRatios,
	double			prefixCompletedMaxRatioZipfMax,
	double			prefixCompletedMaxRatioZipfAlpha,
	double			prefixCompletedMaxRatioMaxCut,
	double			prefixExpRatioLowerBoundRatio,
	double			prefixExpMuNormMu,
	double			prefixExpMuNormSigma,
	double			prefixExpMuCutoff,
	double			prefixExpSessionCutoff,
	unsigned char	arrivalIntroDailyDistChoice,
	double			arrivalIntroDailyDistChoiceShape,
	double			arrivalIntroDailyDistChoiceParetoK,
	unsigned int	arrivalIntroDayGapMax,
	double			arrivalDailyFileNumParetoK,
	double			arrivalDailyFileNumParetoAlpha,
	unsigned int	arrivalDailyMaxFileNum,
	double			arrivalDailyStartSigma,
	double			arrivalIntroTimeGapInADayParetoK,
	double			arrivalIntroTimeGapInADayParetoAlpha,
	double			lifespanNewsRatio,
	double			lifespanNewsDist,
	double			lifespanNewsDistAlphaMu,
	double			lifespanNewsDistAlphaSigma,
	double			lifespanNewsDistAlphaMin,
	double			lifespanNewsDistAlphaMax,
	double			lifespanRegularDistMuMu,
	double			lifespanRegularDistMuSigma,
	double			lifespanRegularDistMuMin,
	double			lifespanRegularDistMuMax,
	double			lifespanRegularDistSigmaMu,
	double			lifespanRegularDistSigmaSigma,
	double			lifespanRegularDistSigmaMin,
	double			lifespanRegularDistSigmaMax,
	double			sessionMaxLifespan,
	unsigned int	sessionDailyBins,
	double*			sessionDailyBinWidth,
	double*			sessionDailyBinAccessRatio,
	unsigned int	sessionDailyAccessDist,
	double			sessionDailyAccessParetoAlpha
	)
{
	this->popularity = new CPopularity(
		numFiles,
		popMaxFreq,
		popZipfAlpha,
		popZipfK);
	this->duration = new CDuration(
		popularity,
		durNumGroups,
		durGroups,
		durMax, 
		durMin);
	this->bitrate = new CBitrate(
		popularity,
		bitrateNumGroups,
		bitrateGroups,
		bitrateRatios
		);
	this->prefix = new CPrefix(
		popularity, 
		duration, 
		prefixCompletedMaxRatioZipfMax, 
		prefixCompletedMaxRatioZipfAlpha, 
		prefixCompletedMaxRatioMaxCut,
		prefixExpRatioLowerBoundRatio,
		prefixExpMuNormMu,
		prefixExpMuNormSigma,
		prefixExpMuCutoff,
		prefixExpSessionCutoff
		);
	this->arrival = new CArrival(
		popularity,
		arrivalIntroDailyDistChoice,
		arrivalIntroDailyDistChoiceShape,
		arrivalIntroDayGapMax,
		arrivalIntroDailyDistChoiceParetoK,
		arrivalDailyFileNumParetoK,
		arrivalDailyFileNumParetoAlpha,
		arrivalDailyMaxFileNum,
		arrivalDailyStartSigma,
		arrivalIntroTimeGapInADayParetoK,
		arrivalIntroTimeGapInADayParetoAlpha
		);
	this->lifespan = new CLifespan(
		popularity,
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
		lifespanRegularDistSigmaMax
		);
	this->session = new CSession(
		popularity,
		duration,
		prefix,
		arrival,
		lifespan,
		sessionMaxLifespan,
		sessionDailyBins,
		sessionDailyBinWidth,
		sessionDailyBinAccessRatio,
		sessionDailyAccessDist,
		sessionDailyAccessParetoAlpha
		);
}

CWorkload::~CWorkload()
{
	delete this->session;
	delete this->lifespan;
	delete this->arrival;
	delete this->prefix;
	delete this->bitrate;
	delete this->duration;
	delete this->popularity;
}