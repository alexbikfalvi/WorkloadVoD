#include "stdafx.h"

#pragma warning(disable : 4996)

enum EEvent
{
	ARRIVE = 0,
	LEAVE = 1,
	SESSION = 2
};

struct SSession
{
	double			time;
	unsigned int	file;
	double			duration;
	double			start;
	double			prefix;
	unsigned int	peer;
};

struct SChurn
{
	double			time;
	unsigned int	peer;
	EEvent			op;
};

struct SEvent
{
	double			time;
	unsigned int	peer;
	EEvent			op;
	unsigned int	file;
	double			duration;
	double			start;
	double			prefix;
};

int main(int argc, char* argv[])
{
	printf("\nEvents generator (v. 1)\n\n");

	if(argc != 4)
	{
		printf("Syntax:\
			   \n\t1 - Peers file\
			   \n\t2 - Churn file\
			   \n\t3 - Output file\
 			   \n");
		return 0;
	}

	char* peersFile = argv[1];
	char* churnFile = argv[2];
	char* outputFile = argv[3];

	FILE* in;

	printf("\nThe parameters are:\n");

	printf("\tpeersFile  = %s\n", peersFile);
	printf("\tchurnFile  = %s\n", churnFile);
	printf("\toutputFile = %s\n\n", outputFile);

	// Read sessions from file
	in = fopen(peersFile, "rb");
	assert(in);

	unsigned int numSessions;
	unsigned int numPeers;
	fread(&numSessions, sizeof(numSessions), 1, in);
	fread(&numPeers, sizeof(numPeers), 1, in);

	printf("1. Reading %u sessions / %u peers...", numSessions, numPeers);

	SSession* sessions = new SSession[numSessions];
	
	fread(sessions, sizeof(SSession), numSessions, in);

	printf("done.\n");

	fclose(in);

	// Read churn from file
	in = fopen(churnFile, "rb");
	assert(in);

	unsigned int numChurn;
	fread(&numChurn, sizeof(numChurn), 1, in);

	printf("2. Reading %u churn events...", numChurn);

	SChurn* churn = new SChurn[numChurn];
	
	fread(churn, sizeof(SChurn), numChurn, in);

	printf("done.\n");

	fclose(in);

	// Generate events
	SEvent evt;
	multimap<double, SEvent> events;

	printf("3. Generating events", numChurn);

	int perc = -1;

	for(unsigned int index = 0; index < numSessions; index++)
	{
		if((int)(100*(index+1)/(numSessions+numChurn)) != perc)
		{
			perc = 100*(index+1)/(numSessions+numChurn);
			printf(".");
		}

		evt.time = sessions[index].time;
		evt.peer = sessions[index].peer;
		evt.op = SESSION;
		evt.file = sessions[index].file;
		evt.duration = sessions[index].duration;
		evt.start = sessions[index].start;
		evt.prefix = sessions[index].prefix;

		events.insert(pair<double, SEvent>(evt.time, evt));
	}

	for(unsigned int index = 0; index < numChurn; index++)
	{
		if((int)(100*(index+1)/(numSessions+numChurn)) != perc)
		{
			perc = 100*(index+1)/(numSessions+numChurn);
			printf(".");
		}

		evt.time = churn[index].time;
		evt.peer = churn[index].peer;
		evt.op = churn[index].op;
		evt.file = -1;
		evt.duration = -1;
		evt.start = -1;
		evt.prefix = -1;

		events.insert(pair<double, SEvent>(evt.time, evt));
	}

	printf("done.\n");

	// Write events to output file

	FILE *out = fopen(outputFile, "wb");
	assert(out);

	unsigned int numEvents = events.size();
	assert(numEvents == numSessions + numChurn);

	fwrite(&numEvents, sizeof(numEvents), 1, out);
	fwrite(&numSessions, sizeof(numSessions), 1, out);
	fwrite(&numPeers, sizeof(numPeers), 1, out);

	for(multimap<double, SEvent>::iterator iter = events.begin(); iter != events.end(); iter++)
		fwrite(&iter->second, sizeof(iter->second), 1, out);

	fclose(out);

	delete[] sessions;
	delete[] churn;

	_CrtDumpMemoryLeaks();

	return 0;
}


