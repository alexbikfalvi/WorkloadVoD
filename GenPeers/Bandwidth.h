#ifndef __Bandwidth_h__
#define __Bandwidth_h__

class CBandwidth
{
private:
	unsigned int	numPeers;
	unsigned int*	bw;

public:
	CBandwidth(
		unsigned int	numPeers,
		unsigned int	numGroups,
		unsigned int*	groups,
		double*			ratios
		);
	~CBandwidth();

	inline unsigned int		operator[] (unsigned int index) { assert(index < numPeers); return this->bw[index]; }
	inline unsigned int*	Bw() { return this->bw; }
};

#endif