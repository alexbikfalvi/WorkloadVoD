#ifndef __Prefix_h__
#define __Prefix_h__

#include "Popularity.h"
#include "Duration.h"

class CPrefix
{
private:
	CPopularity*	popularity;
	CDuration*		duration;

	double			completedMaxRatioZipfMax;
	double			completedMaxRatioZipfAlpha;
	double			completedMaxRatioMaxCut;

	double			expRatioLowerBoundRatio;
	double			expMuNormMu;
	double			expMuNormSigma;
	double			expMuCutoff;
	double			expSessionCutoff;

	unsigned int	numBins;
	double*			expRatio;
	double*			expMu;
	double*			completedRatio;

public:
	CPrefix(
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
		);
	~CPrefix();

	inline double	ExpRatio(unsigned int index) { assert(index < this->popularity->NumFiles()); return this->expRatio[index]; }
	inline double	ExpMu(unsigned int index) { assert(index < this->popularity->NumFiles()); return this->expMu[index]; }
	inline double	CompletedRatio(unsigned int index) { assert(index < this->popularity->NumFiles()); return this->completedRatio[index]; }

	inline double	ExpSessionCutoff() { return this->expSessionCutoff; }

protected:
	void			GenerateCompletedRatio();
	void			GenerateExpRatio();
	void			GenerateExpMu();
};

#endif