#include "stdafx.h"
#include "Peers.h"
#include "Bandwidth.h"

#pragma warning(disable : 4996)

struct SSessionRecord
{
	double			time;
	unsigned int	file;
	double			duration;
	double			start;
	double			prefix;
	unsigned int	peer;
};

int main(int argc, char* argv[])
{
	printf("\nPeers generator (v. 1)\n\n");

	if(argc != 6)
	{
		printf("Syntax:\
			   \n\t1 - Config file\
			   \n\t2 - Sessions file\
			   \n\t3 - Output peers file\
			   \n\t4 - Output mapping file\
			   \n\t5 - Verbose\
 			   \n");
		return 0;
	}

	char* inputFile = argv[1];
	char* sessionsFile = argv[2];
	char* outputPeersFile = argv[3];
	char* outputMappingFile = argv[4];
	bool verbose = (atoi(argv[5]) != 0);

	unsigned int peerDist;
	unsigned int peerNumMax;
	unsigned int peerForceMax;
	double peerFreeMargin;

	unsigned int bwNumGroups;
	unsigned int* bwGroups;
	double* bwRatios;

	FILE* in;

	in = fopen(inputFile, "r");

	if(fscanf(in, "peerDist=%u\n", &peerDist) != 1) { printf("Error reading input file"); return -1; }
	if(fscanf(in, "peerNumMax=%u\n", &peerNumMax) != 1) { printf("Error reading input file"); return -1; }
	if(fscanf(in, "peerForceMax=%u\n", &peerForceMax) != 1) { printf("Error reading input file"); return -1; }
	if(fscanf(in, "peerFreeMargin=%lf\n", &peerFreeMargin) != 1) { printf("Error reading input file"); return -1; }

	if(fscanf(in, "bwNumGroups=%u\n", &bwNumGroups) != 1) { printf("Error reading input file"); return -1; }

	bwGroups = new unsigned int[bwNumGroups];
	bwRatios = new double[bwNumGroups];

	assert(bwGroups);
	assert(bwRatios);

	fscanf(in, "bwGroups={");
	for(unsigned int index = 0; index < bwNumGroups; index++)
	{
		if(index) fscanf(in, ",");
		if(fscanf(in, "%u", &bwGroups[index]) != 1) { printf("Error reading input file"); return -1; }
	}
	fscanf(in, "}\n");

	fscanf(in, "bwRatios={");
	for(unsigned int index = 0; index < bwNumGroups; index++)
	{
		if(index) fscanf(in, ",");
		if(fscanf(in, "%lf", &bwRatios[index]) != 1) { printf("Error reading input file"); return -1; }
	}
	fscanf(in, "}\n");

	fclose(in);

	printf("\nThe parameters are:\n");

	printf("\tconfigFile        = %s\n", inputFile);
	printf("\tsessionsFile      = %s\n", sessionsFile);
	printf("\toutputPeersFile   = %s\n", outputPeersFile);
	printf("\toutputMappingFile = %s\n", outputMappingFile);
	printf("\tverbose           = %u\n", verbose);
	printf("\tpeerDist          = %u\n", peerDist);
	printf("\tpeerNumMax        = %u\n", peerNumMax);
	printf("\tpeerForceMax      = %u\n", peerForceMax);
	printf("\tpeerFreeMargin    = %lf\n", peerFreeMargin);
	printf("\tbwNumGroups       = %u\n", bwNumGroups);
	printf("\tbwGroups          = {");
	for(unsigned int index = 0; index < bwNumGroups; index++)
	{
		if(index) printf(",");
		printf("%u", bwGroups[index]);
	}
	printf("}\n");

	printf("\tbwRatios          = {");
	for(unsigned int index = 0; index < bwNumGroups; index++)
	{
		if(index) printf(",");
		printf("%lf", bwRatios[index]);
	}
	printf("}\n\n\n");

	// Read sessions from file
	in = fopen(sessionsFile, "rb");
	assert(in);

	unsigned int numSessions;
	unsigned int numPeers;
	fread(&numSessions, sizeof(numSessions), 1, in);

	printf("1. Reading %u sessions...", numSessions);

	SSession* sessions = new SSession[numSessions];
	
	// Determine the number of sessions
	fread(sessions, sizeof(SSession), numSessions, in);

	printf("done.\n");

	fclose(in);

	CPeers* peers = new CPeers(
		numSessions,
		sessions,
		(unsigned char) peerDist,
		peerNumMax,
		peerForceMax != 0,
		peerFreeMargin
		);
	assert(peers);

	CBandwidth* bandwidth = new CBandwidth(
		peers->Max(),
		bwNumGroups,
		bwGroups,
		bwRatios);
	assert(bandwidth);

	FILE* out;

	if(verbose)
	{
		// Peer demand
		out = fopen("peer_demand.txt", "w");
		for(unsigned int index = 0; index < 2*numSessions; index++)
		{
			SDemand demand = peers->Demand(index);

			fprintf(out, "%lf %u\n", demand.time, demand.peers);
		}
		fclose(out);

		// Peer load
		out = fopen("peer_load.txt", "w");
		for(unsigned int index = 0; index < peers->Max(); index++)
			fprintf(out, "%lf ", peers->Load(index));
		fclose(out);
	}

	numPeers = peers->Max();

	out = fopen(outputMappingFile, "wb");

	fwrite(&numSessions, sizeof(numSessions), 1, out);
	fwrite(&numPeers, sizeof(numPeers), 1, out);

	SSessionRecord rec;

	for(unsigned int index = 0; index < numSessions; index++)
	{
		rec.time = sessions[index].time;
		rec.file = sessions[index].file;
		rec.duration = sessions[index].duration;
		rec.start = sessions[index].start;
		rec.prefix = sessions[index].prefix;
		rec.peer = (*peers)[index];

		fwrite(&rec, sizeof(rec), 1, out);
	}

	fclose(out);

	out = fopen(outputPeersFile, "wb");

	fwrite(&numPeers, sizeof(numPeers), 1, out);
	fwrite(bandwidth->Bw(), sizeof(unsigned int), numPeers, out);

	fclose(out);

	delete peers;
	delete bandwidth;
	delete[] sessions;
	
	delete[] bwGroups;
	delete[] bwRatios;

	_CrtDumpMemoryLeaks();

	return 0;
}
