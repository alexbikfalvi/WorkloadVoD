#ifndef __Workload_h__
#define __Workload_h__

#include "Popularity.h"
#include "Duration.h"
#include "Prefix.h"
#include "Arrival.h"
#include "Lifespan.h"
#include "Session.h"
#include "Bitrate.h"

class CWorkload
{
private:
	CPopularity*	popularity;
	CDuration*		duration;
	CPrefix*		prefix;
	CArrival*		arrival;
	CLifespan*		lifespan;
	CSession*		session;
	CBitrate*		bitrate;

public:
	CWorkload(
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
		);
	~CWorkload();

	inline CPopularity*	Popularity() { return this->popularity; }
	inline CDuration*	Duration() { return this->duration; }
	inline CBitrate*	Bitrate() { return this->bitrate; }
	inline CPrefix*		Prefix() { return this->prefix; }
	inline CArrival*	Arrival() { return this->arrival; }
	inline CLifespan*	Lifespan() { return this->lifespan; }
	inline CSession*	Session() { return this->session; }
};

#endif