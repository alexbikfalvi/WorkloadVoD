#ifndef __Lifespan_h__
#define __Lifespan_h__

#include "Popularity.h"

enum ELifespanClass
{
	LSC_LOGN = 1,
	LSC_PARETO = 2
};

class CLifespan
{
private:
	CPopularity*	popularity;

	double					newsRatio;
	double					newsDist;
	double					newsDistAlphaMu;
	double					newsDistAlphaSigma;
	double					newsDistAlphaMin;
	double					newsDistAlphaMax;
	double					regularDistMuMu;
	double					regularDistMuSigma;
	double					regularDistMuMin;
	double					regularDistMuMax;
	double					regularDistSigmaMu;
	double					regularDistSigmaSigma;
	double					regularDistSigmaMin;
	double					regularDistSigmaMax;

	ELifespanClass*			cls;
	double*					param1;
	double*					param2;

	vector<unsigned int>	clsNews;
	vector<unsigned int>	clsRegular;
	unsigned int			numNews;
	unsigned int			numRegular;

	vector<double>			distNewsAlpha;
	vector<double>			distRegularMu;
	vector<double>			distRegularSigma;

public:
	CLifespan(
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
		);
	~CLifespan();

	inline unsigned int		NumNews() { return this->numNews; }
	inline unsigned int		NumRegular() { return this->numRegular; }
	inline ELifespanClass	Class(unsigned int index) { assert(index < this->popularity->NumFiles()); return this->cls[index]; }
	inline unsigned int		ClsNews(unsigned int index) { assert(index < this->numNews); return this->clsNews[index]; }
	inline unsigned int		ClsRegular(unsigned int index) { assert(index < this->numRegular); return this->clsRegular[index]; }
	inline double			DistNewsAlpha(unsigned int index) { assert(index < this->numNews); return this->distNewsAlpha[index]; }
	inline double			DistRegularMu(unsigned int index) { assert(index < this->numRegular); return this->distRegularMu[index]; }
	inline double			DistRegularSigma(unsigned int index) { assert(index < this->numRegular); return this->distRegularSigma[index]; }

	inline double			ParetoK(unsigned int index) { assert(index < this->popularity->NumFiles()); return this->param1[index]; }
	inline double			ParetoAlpha(unsigned int index) { assert(index < this->popularity->NumFiles()); return this->param2[index]; }

	inline double			LognMu(unsigned int index) { assert(index < this->popularity->NumFiles()); return this->param1[index]; }
	inline double			LognSigma(unsigned int index) { assert(index < this->popularity->NumFiles()); return this->param2[index]; }

protected:
	void					GenerateClass();
	void					GenerateParameters();
};

#endif