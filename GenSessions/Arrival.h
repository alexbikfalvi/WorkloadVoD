#ifndef __Arrival_h__
#define __Arrival_h__

#include "Popularity.h"
#include "Shuffle.h"

class CArrival
{
private:
	CPopularity*			popularity;

	unsigned char			introDailyDistChoice;
	double					introDailyDistChoiceShape;
	unsigned int			introDayGapMax;
	double					introDailyDistChoiceParetoK;
	double					dailyFileNumParetoK;
	double					dailyFileNumParetoAlpha;
	unsigned int			dailyMaxFileNum;
	double					dailyStartSigma;
	double					dailyGapParetoK;
	double					dailyGapParetoAlpha;

	vector<unsigned int>	fileIntroDay;
	unsigned int*			fileIntroGaps;

	double*					dailyStartTime;
	vector<double>*			dailyGaps;
	
	vector<double>			introTime;
	CShuffle*				introMapping;

public:
	CArrival(
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
		);
	~CArrival();

	inline unsigned int NumIntroDays() { return this->fileIntroDay.size(); }
	
	inline unsigned int	FileIntroDay(unsigned int index) { assert(index < this->fileIntroDay.size()); return this->fileIntroDay[index]; }
	inline unsigned int	FileIntroGaps(unsigned int index) { assert(index < this->fileIntroDay.size()-1); return this->fileIntroGaps[index]; }
	
	inline double		DailyStartTime(unsigned int index) { assert(index < this->fileIntroDay.size()); return this->dailyStartTime[index]; }
	inline double		DailyGap(unsigned int day, unsigned int gap) { assert(day < this->fileIntroDay.size()); assert(gap < this->fileIntroDay[day]-1); return this->dailyGaps[day][gap]; }

	inline double		operator[](unsigned int index) { assert(index < this->popularity->NumFiles()); return this->introTime[(*this->introMapping)[index]]; }
	inline double		IntroTime(unsigned int index) { assert(index < this->popularity->NumFiles()); return this->introTime[index]; }
	inline double		IntroMapping(unsigned int index) { assert(index < this->popularity->NumFiles()); return (*this->introMapping)[index]; }

protected:
	void				GenerateDailyIntro();
	void				GenerateDailyIntroGapPareto();
	void				GenerateDailyIntroGapExp();
	void				GenerateDailyIntroGapFixed();
	void				GenerateIntroTime();
	
	void				GenerateDailyStartTime(unsigned int index);
	void				GenerateDailyGaps(unsigned int index);
};

#endif