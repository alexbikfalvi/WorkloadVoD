#ifndef __Session_h__
#define __Session_h__

#include "Popularity.h"
#include "Duration.h"
#include "Prefix.h"
#include "Arrival.h"
#include "Lifespan.h"

struct SSession
{
	unsigned int	file;
	double			prefix;
};

class CSession
{
private:
	CPopularity*						popularity;
	CDuration*							duration;
	CPrefix*							prefix;
	CArrival*							arrival;
	CLifespan*							lifespan;
	
	multimap<double, SSession>			sessions;
	double								maxLifespan;
	unsigned int						dailyBins;
	double*								dailyBinWidthRatio;
	double*								dailyBinAccessRatio;
	unsigned int						dailyAccessDist;
	double								dailyAccessParetoAlpha;

	double*								dailyBinStart;
	double*								dailyBinWidth;

	map<unsigned int, unsigned int>*	fileAccess;
	list<double>*						fileSession;
	double**							filePrefix;

public:
	CSession(
		CPopularity*	popularity,
		CDuration*		duration,
		CPrefix*		prefix,
		CArrival*		arrival,
		CLifespan*		lifespan,
		double			maxLifespan,
		unsigned int	dailyBins,
		double*			dailyBinWidth,
		double*			dailyBinAccessRatio,
		unsigned int	dailyAccessDist,
		double			dailyAccessParetoAlpha
		);
	~CSession();

	inline unsigned int						DailyBins() { return this->dailyBins; }
	inline double							DailyBinStart(unsigned int index) { assert(index < this->dailyBins); this->dailyBinStart[index]; }
	inline double							DailyBinWidth(unsigned int index) { assert(index < this->dailyBins); this->dailyBinWidth[index]; }
	inline map<unsigned int, unsigned int>*	FileAccess(unsigned int index) { assert(index < this->popularity->NumFiles()); return &this->fileAccess[index]; }
	inline list<double>*					FileSession(unsigned int index) { assert(index < this->popularity->NumFiles()); return &this->fileSession[index]; }
	inline double							FilePrefix(unsigned int file, unsigned int index) { assert(file < this->popularity->NumFiles()); assert(index < (*this->popularity)[index]); return this->filePrefix[file][index]; }
	inline multimap<double, SSession>*		Sessions() { return &this->sessions; }

protected:
	void	Init();
	void	GenerateFileLifespan(unsigned int index);
	void	GenerateFilePrefix(unsigned int index);
	void	GenerateFileSessions(unsigned int index);

	void	IncrementFileAccess(unsigned int file, unsigned int day);
};

#endif