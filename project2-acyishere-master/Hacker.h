#include <vector>
using namespace std;

#ifndef HACKER_H
#define HACKER_H

class Hacker {

public:

	float arriveTime;
	bool isValidforHoodie = false;
	int id;
	int totalCommits;
	int validCommits;
	int numOfGifts;
	bool isValidforEntrance = false;
	bool inQueue = false;
	float queueEntranceTime;
	float waitingInQueues;

	vector<float> giftTimes;
	vector<float> commitTimes;
	vector<int> commitLines;
	vector<float> stqueueEntranceTimes;
	vector<float> hdqueueLeaveTimes;

	Hacker(int id,float arriveTime); 

	~Hacker();

};

#endif