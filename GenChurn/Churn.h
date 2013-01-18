#ifndef __Churn_h__
#define __Churn_h__

struct SSession
{
	double			time;
	unsigned int	file;
	double			duration;
	double			start;
	double			prefix;
	unsigned int	peer;
};

enum EChurn
{
	ARRIVE = 0,
	LEAVE = 1
};

struct SChurn
{
	unsigned int	peer;
	EChurn			op;
};

struct SPeerChurn
{
	double			time;
	EChurn			op;
};

class CChurn
{
private:
	unsigned char					dist;

	unsigned int					numSessions;
	unsigned int					numPeers;

	SSession*						sessions;

	multimap<double, SChurn>		churn;
	vector<SPeerChurn>*				peerChurn;

public:
	CChurn(
		unsigned char	dist,
		unsigned int	numSessions,
		unsigned int	numPeers,
		SSession*		sessions
		);
	~CChurn();

	inline multimap<double, SChurn>*	Churn() { return &this->churn; }
	inline vector<SPeerChurn>*			PeerChurn(unsigned int index) { assert(index < this->numPeers); return &this->peerChurn[index]; }

private:
	void	GenerateNoChurn();
};

#endif