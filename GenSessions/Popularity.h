#ifndef __Popularity_h__
#define __Popularity_h__

class CPopularity
{
private:
	unsigned int*	freq;
	unsigned int	totalFreq;
	unsigned int	numFiles;
	unsigned int	firstZero;

public:
	CPopularity(
		unsigned int	numFiles,
		unsigned int	maxFreq,
		double			zipfAlpha,
		double			zipfK
		);
	~CPopularity();

	inline unsigned int		NumFiles() { return this->numFiles; }	
	inline unsigned int*	Freq() { return this->freq; }
	inline unsigned int		operator[](unsigned int index) { assert(index < this->numFiles); return this->freq[index]; }
	inline unsigned int		Freq(unsigned int index) { assert(index < this->numFiles); return this->freq[index]; }
	inline unsigned int		TotalFreq() { return this->totalFreq; }
};

#endif