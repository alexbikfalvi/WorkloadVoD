#include "stdafx.h"
#include "Churn.h"

#pragma warning(disable : 4996)

struct SChurnRecord
{
	double			time;
	unsigned int	peer;
	EChurn			op;
};

int main(int argc, char* argv[])
{
	printf("\nChurn generator (v. 1)\n\n");

	if(argc != 4)
	{
		printf("Syntax:\
			   \n\t1 - Config file\
			   \n\t2 - Peers file\
			   \n\t3 - Output file\
 			   \n");
		return 0;
	}

	char* inputFile = argv[1];
	char* peersFile = argv[2];
	char* outputFile = argv[3];

	unsigned int churnDist;

	FILE* in;

	in = fopen(inputFile, "r");

	if(fscanf(in, "churnDist=%u\n", &churnDist) != 1) { printf("Error reading input file"); return -1; }

	fclose(in);

	printf("\nThe parameters are:\n");

	printf("\tconfigFile = %s\n", inputFile);
	printf("\tpeersFile  = %s\n", peersFile);
	printf("\toutputFile = %s\n", outputFile);
	printf("\tchurnDist  = %u\n\n", churnDist);

	// Read sessions from file
	in = fopen(peersFile, "rb");
	assert(in);

	unsigned int numSessions;
	unsigned int numPeers;
	fread(&numSessions, sizeof(numSessions), 1, in);
	fread(&numPeers, sizeof(numPeers), 1, in);

	printf("1. Reading %u sessions / %u peers...", numSessions, numPeers);

	SSession* sessions = new SSession[numSessions];
	
	// Determine the number of sessions
	fread(sessions, sizeof(SSession), numSessions, in);

	printf("done.\n");

	fclose(in);

	CChurn* churn = new CChurn(
		churnDist,
		numSessions,
		numPeers,
		sessions
		);
	assert(churn);

	FILE* out;

	multimap<double, SChurn>* mapChurn = churn->Churn();
	unsigned int numChurn = mapChurn->size();

	out = fopen(outputFile, "wb");

	fwrite(&numChurn, sizeof(numChurn), 1, out);

	SChurnRecord rec;

	for(multimap<double, SChurn>::iterator iter = mapChurn->begin(); iter != mapChurn->end(); iter++)
	{
		rec.time = iter->first;
		rec.peer = iter->second.peer;
		rec.op = iter->second.op;

		fwrite(&rec, sizeof(rec), 1, out);
	}
	
	fclose(out);

	delete churn;
	delete[] sessions;

	_CrtDumpMemoryLeaks();

	return 0;
}


