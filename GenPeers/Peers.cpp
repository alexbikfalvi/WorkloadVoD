#include "stdafx.h"
#include "Peers.h"
#include "Rand.h"

CPeers::CPeers(
	unsigned int	numSessions,
	SSession*		sessions,
	unsigned char	dist,
	unsigned int	numMax,
	bool			forceMax,
	double			freePeerMargin
	)
{
	assert(numSessions);
	assert(sessions);

	this->numSessions = numSessions;
	this->sessions = sessions;
	this->numMax = numMax;
	this->forceMax = forceMax;
	this->freePeerMargin = freePeerMargin;

	this->peers = new unsigned int[numSessions];
	assert(this->peers);

	this->Initialize();

	switch(dist)
	{
	case 0: // Uniform selection
		this->GenerateUniformSelection();
		break;
	case 1: // Uniform load minimum
		this->GenerateUniformLoadMin();
		break;
	case 2: // Uniform load weighted
		this->GenerateUniformLoadWeighted();
		break;
	}
}

CPeers::~CPeers()
{
	delete[] this->peers;
	delete[] this->load;
	delete[] this->demand;
}

void CPeers::Initialize()
{
	printf("2. Calculating number of peers");

	// Calculate minimum required number of peers if maximum is not forced
	if(!this->forceMax)
	{
		unsigned int max = 0;
		list<double> pool;

		int perc = -1;
		for(unsigned int index = 0; index < this->numSessions; index++)
		{
			if((int)(100*(index+1)/this->numSessions) != perc)
			{
				perc = 100*(index+1)/this->numSessions;
				printf(".");
			}

			bool found = 0;
			for(list<double>::iterator iter = pool.begin(); iter != pool.end(); iter++)
				if((*iter) < this->sessions[index].time)
				{
					// An available peer has been found in the pool
					(*iter) = this->sessions[index].time + this->sessions[index].prefix + this->freePeerMargin;
					found = 1;
					break;
				}
			if(!found)
			{
				// An available peer has not been found in the pool
				pool.push_back(this->sessions[index].time + this->sessions[index].prefix + this->freePeerMargin);
				max++;
			}
		}

		// If the maximum is less than this, update the maximum
		if(max > this->numMax) this->numMax = max;
	}
	printf("%u max done.\n", this->numMax);

	// Allocate load array
	this->load = new double[this->numMax];
	assert(this->load);

	for(unsigned int index = 0; index < this->numMax; index++)
		this->load[index] = 0;
}

void CPeers::GenerateUniformSelection()
{
	// Allocate peers to sessions
	printf("3. Allocating peer to sessions (uniform selection)");

	map<unsigned int, unsigned int> freePool;
	map<unsigned int, unsigned int> busyPool;

	// Fill the free pool
	for(unsigned int index = 0; index < this->numMax; index++)
		freePool.insert(pair<unsigned int, unsigned int>(index,0));

	unsigned int idx = 0;
	unsigned int num = 0;
	this->demand = new SDemand[2*this->numSessions];
	multimap<double, unsigned int> busy;
	int perc = -1;

	for(unsigned int index = 0; index < this->numSessions; index++)
	{
		if((int)(100*(index+1)/this->numSessions) != perc)
		{
			perc = 100*(index+1)/this->numSessions;
			printf(".");
		}

		// Verify the number of peers in busy and free pools
		assert(freePool.size() == this->numMax - num);
		assert(busyPool.size() == num);

		// Check if there are sessions that finished before this time
		multimap<double, unsigned int>::iterator last;
		bool del = 0;
		for(multimap<double, unsigned int>::iterator iter = busy.begin(); iter != busy.end(); iter++)
		{
			if(iter->first <= this->sessions[index].time)
			{
				// Calculate peer demand - remove oldest sessions
				assert(num);
				num--;

				assert(idx < 2*this->numSessions);

				this->demand[idx].time = iter->first;
				this->demand[idx].peers = num;

				idx++;
				last = iter;
				del = 1;

				// Remove peer from busy pool
				busyPool.erase(iter->second);
				// Add peer to busy pool
				freePool.insert(pair<unsigned int, unsigned int>(iter->second,0));

				assert(freePool.size() == this->numMax - num);
				assert(busyPool.size() == num);
			}
			else break;
		}
		if(del) busy.erase(busy.begin(), ++last);

		// If some peers are available
		if(num < this->numMax)
		{
			// Get a random peer from free pool
			unsigned int rnd = CRand::Generate(freePool.size());
			assert(rnd < freePool.size());

			unsigned int idx1 = 0;
			unsigned int peer = 0;
			for(map<unsigned int, unsigned int>::iterator iter = freePool.begin(); iter != freePool.end(); iter++, idx1++)
			{
				if(rnd == idx1)
				{
					peer = iter->first;
					assert(peer < this->numMax);
					freePool.erase(iter);
					break;
				}
			}

			// Calculate peer demand - add new session
			assert(idx < 2*this->numSessions);
			
			num++;
			this->demand[idx].time = this->sessions[index].time;
			this->demand[idx].peers = num;

			idx++;
			busy.insert(pair<double,unsigned int>(this->sessions[index].time+this->sessions[index].prefix+this->freePeerMargin, peer));

			// Add peer to busy pool
			busyPool.insert(pair<unsigned int, unsigned int>(peer,0));

			// Save the peer for this session
			this->peers[index] = peer;

			// Update peer load
			this->load[peer] += this->sessions[index].prefix+this->freePeerMargin;
		}
	}

	for(multimap<double, unsigned int>::iterator iter = busy.begin(); iter != busy.end(); iter++)
	{
		num--;

		assert(idx < 2*this->numSessions);
		this->demand[idx].time = iter->first;
		this->demand[idx].peers = num;

		idx++;
	}

	printf("done.\n");
}

void CPeers::GenerateUniformLoadMin()
{
	// Allocate peers to sessions
	printf("3. Allocating peer to sessions (uniform load minimum)");

	map<unsigned int, double> freePool;
	map<unsigned int, double> busyPool;

	// Fill the free pool
	for(unsigned int index = 0; index < this->numMax; index++)
		freePool.insert(pair<unsigned int, double>(index,0));	// initial load: 0

	unsigned int idx = 0;
	unsigned int num = 0;
	this->demand = new SDemand[2*this->numSessions];
	multimap<double, unsigned int> busy;
	int perc = -1;

	for(unsigned int index = 0; index < this->numSessions; index++)
	{
		if((int)(100*(index+1)/this->numSessions) != perc)
		{
			perc = 100*(index+1)/this->numSessions;
			printf(".");
		}

		// Verify the number of peers in busy and free pools
		assert(freePool.size() == this->numMax - num);
		assert(busyPool.size() == num);

		// Check if there are sessions that finished before this time
		multimap<double, unsigned int>::iterator last;
		bool del = 0;
		for(multimap<double, unsigned int>::iterator iter = busy.begin(); iter != busy.end(); iter++)
		{
			if(iter->first <= this->sessions[index].time)
			{
				// Calculate peer demand - remove oldest sessions
				assert(num);
				num--;

				assert(idx < 2*this->numSessions);

				this->demand[idx].time = iter->first;
				this->demand[idx].peers = num;

				idx++;
				last = iter;
				del = 1;

				// Search in busy pool
				map<unsigned int, double>::iterator it = busyPool.find(iter->second);
				assert(it != busyPool.end());

				// Add peer to free pool
				freePool.insert(pair<unsigned int, double>(iter->second,it->second));
				
				// Remove peer from busy pool
				busyPool.erase(iter->second);

				assert(freePool.size() == this->numMax - num);
				assert(busyPool.size() == num);
			}
			else break;
		}
		if(del) busy.erase(busy.begin(), ++last);

		// If some peers are available
		if(num < this->numMax)
		{
			unsigned int peer = 0;
			double ld = 1E300;
			list<map<unsigned int, double>::iterator> lst;
			double rnd = CRand::Generate();

			for(map<unsigned int, double>::iterator iter = freePool.begin(); iter != freePool.end(); iter++)
			{
				if(iter->second < ld)
				{
					ld = iter->second;
					lst.clear();
					lst.push_back(iter);
				}
				else if(iter->second == ld)
					lst.push_back(iter);
			}

			assert(lst.size());
			double ratio = 1.0/lst.size();

			for(list<map<unsigned int, double>::iterator>::iterator iter = lst.begin(); iter != lst.end(); iter++)
			{
				if(DOUBLE_LT(rnd,ratio))
				{
					peer = (*iter)->first;
					assert(ld == (*iter)->second);

					assert(peer < this->numMax);

					// Remove peer from the free pool
					freePool.erase(*iter);
					break;
				}
				rnd -= ratio;
			}

		
			// Calculate peer demand - add new session
			assert(idx < 2*this->numSessions);
			
			num++;
			this->demand[idx].time = this->sessions[index].time;
			this->demand[idx].peers = num;

			idx++;
			busy.insert(pair<double,unsigned int>(this->sessions[index].time+this->sessions[index].prefix+this->freePeerMargin, peer));

			// Add peer to busy pool
			busyPool.insert(pair<unsigned int, double>(peer,ld+this->sessions[index].prefix+this->freePeerMargin));

			// Save the peer for this session
			this->peers[index] = peer;

			// Update peer load
			this->load[peer] += this->sessions[index].prefix+this->freePeerMargin;
		}
	}

	for(multimap<double, unsigned int>::iterator iter = busy.begin(); iter != busy.end(); iter++)
	{
		num--;

		assert(idx < 2*this->numSessions);
		this->demand[idx].time = iter->first;
		this->demand[idx].peers = num;

		idx++;
	}

	printf("done.\n");
}

void CPeers::GenerateUniformLoadWeighted()
{
	// Allocate peers to sessions
	printf("3. Allocating peer to sessions (uniform load weighted)");

	map<unsigned int, double> freePool;
	map<unsigned int, double> busyPool;

	// Fill the free pool
	for(unsigned int index = 0; index < this->numMax; index++)
		freePool.insert(pair<unsigned int, double>(index,0));	// initial load: 0

	unsigned int idx = 0;
	unsigned int num = 0;
	this->demand = new SDemand[2*this->numSessions];
	multimap<double, unsigned int> busy;
	int perc = -1;

	for(unsigned int index = 0; index < this->numSessions; index++)
	{
		if((int)(100*(index+1)/this->numSessions) != perc)
		{
			perc = 100*(index+1)/this->numSessions;
			printf(".");
		}

		// Verify the number of peers in busy and free pools
		assert(freePool.size() == this->numMax - num);
		assert(busyPool.size() == num);

		// Check if there are sessions that finished before this time
		multimap<double, unsigned int>::iterator last;
		bool del = 0;
		for(multimap<double, unsigned int>::iterator iter = busy.begin(); iter != busy.end(); iter++)
		{
			if(iter->first <= this->sessions[index].time)
			{
				// Calculate peer demand - remove oldest sessions
				assert(num);
				num--;

				assert(idx < 2*this->numSessions);

				this->demand[idx].time = iter->first;
				this->demand[idx].peers = num;

				idx++;
				last = iter;
				del = 1;

				// Search in busy pool
				map<unsigned int, double>::iterator it = busyPool.find(iter->second);
				assert(it != busyPool.end());

				// Add peer to free pool
				freePool.insert(pair<unsigned int, double>(iter->second,it->second));
				
				// Remove peer from busy pool
				busyPool.erase(iter->second);

				assert(freePool.size() == this->numMax - num);
				assert(busyPool.size() == num);
			}
			else break;
		}
		if(del) busy.erase(busy.begin(), ++last);

		// If some peers are available
		if(num < this->numMax)
		{
			// Get a peer from free pool based on their weighted load 
			double rnd = CRand::Generate();
			unsigned int peer = 0;
			double ld = 0;
			double ldsum = 0;
			double ldzero = 0;

			// Calculate the sum of the load of peers in the free pool
			for(map<unsigned int, double>::iterator iter = freePool.begin(); iter != freePool.end(); iter++)
			{
				if(iter->second) ldsum += 1.0/iter->second;
				else ldzero += 1;
			}

			for(map<unsigned int, double>::iterator iter = freePool.begin(); iter != freePool.end(); iter++)
			{
				double ratio;
				
				if(iter->second) ratio = 1.0/(iter->second*ldsum);
				else
				{
					assert(ldzero);
					ratio = 1.0/ldzero;
				}

				if(DOUBLE_LT(rnd,ratio))
				{
					peer = iter->first;
					ld = iter->second;

					assert(peer < this->numMax);

					// Remove peer from the free pool
					freePool.erase(iter);
					break;
				}

				rnd -= ratio;
			}

			// Calculate peer demand - add new session
			assert(idx < 2*this->numSessions);
			
			num++;
			this->demand[idx].time = this->sessions[index].time;
			this->demand[idx].peers = num;

			idx++;
			busy.insert(pair<double,unsigned int>(this->sessions[index].time+this->sessions[index].prefix+this->freePeerMargin, peer));

			// Add peer to busy pool
			busyPool.insert(pair<unsigned int, double>(peer,ld+this->sessions[index].prefix+this->freePeerMargin));

			// Save the peer for this session
			this->peers[index] = peer;

			// Update peer load
			this->load[peer] += this->sessions[index].prefix+this->freePeerMargin;
		}
	}

	for(multimap<double, unsigned int>::iterator iter = busy.begin(); iter != busy.end(); iter++)
	{
		num--;

		assert(idx < 2*this->numSessions);
		this->demand[idx].time = iter->first;
		this->demand[idx].peers = num;

		idx++;
	}

	printf("done.\n");
}
