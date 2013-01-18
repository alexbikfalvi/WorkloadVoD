#include "stdafx.h"
#include "Churn.h"

CChurn::CChurn(
	unsigned char	dist,
	unsigned int	numSessions,
	unsigned int	numPeers,
	SSession*		sessions
	)
{
	assert(numSessions);
	assert(numPeers);
	assert(sessions);

	this->numSessions = numSessions;
	this->numPeers = numPeers;
	this->sessions = sessions;

	this->peerChurn = new vector<SPeerChurn>[this->numPeers];
	assert(this->peerChurn);

	switch(dist)
	{
	case 0: // no churn
		this->GenerateNoChurn();
		break;
	}
}

CChurn::~CChurn()
{
	delete[] this->peerChurn;
}

void CChurn::GenerateNoChurn()
{
	printf("2. Generating churn...");

	for(unsigned int index = 0; index < this->numPeers; index++)
	{
		SChurn rec;

		rec.peer = index;
		rec.op = ARRIVE;

		this->churn.insert(pair<double, SChurn>(0.0, rec));

		SPeerChurn prec;

		prec.time = 0;
		prec.op = ARRIVE;

		this->peerChurn[index].push_back(prec);
	}

	printf("done.\n");
}
