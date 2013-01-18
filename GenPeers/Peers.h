#ifndef __Peers_h__
#define __Peers_h__

struct SSession
{
	double			time;
	unsigned int	file;
	double			duration;
	double			start;
	double			prefix;
};

struct SDemand
{
	double			time;
	unsigned int	peers;
};

class CPeers
{
private:
	unsigned int	numSessions;
	SSession*		sessions;

	unsigned int	numMax;
	bool			forceMax;
	double			freePeerMargin;
	
	unsigned int*	peers;
	SDemand*		demand;
	double*			load;

public:
	CPeers(
		unsigned int	numSessions,
		SSession*		sessions,
		unsigned char	dist,
		unsigned int	numMax,
		bool			forceMax,
		double			freeMeerMargin
		);
	~CPeers();

	inline unsigned int	Max() { return this->numMax; }
	inline unsigned int	operator[] (unsigned int index) { assert(index < numSessions); return this->peers[index]; }
	inline SDemand		Demand(unsigned int index) { assert(index < 2*numSessions); return this->demand[index]; }
	inline double		Load(unsigned int index) { assert(index < this->numMax); return this->load[index]; }

protected:
	void	Initialize();
	void	GenerateUniformSelection();
	void	GenerateUniformLoadMin();
	void	GenerateUniformLoadWeighted();
};

#endif